#include "vulkanInitialize.hpp"

/* Creates a Vulkan Instance for a VulkanContext */
void createVulkanInstance(vulkanContext& context)
{
    if (ENABLE_VALIDATION_LAYERS && !checkValidationLayerSupport())
    {
        throw std::runtime_error("{ERROR} Validation Layers were requested but not found.");
    }

    /* Vulkan Application Info */
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "ESC"; // TODO: MAKE CONST IN GLOBE FILE
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    /* Vulkan Instance Info */
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    /* Set Extensions for Vulkan Instance*/
    auto extension = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extension.size());
    createInfo.ppEnabledExtensionNames = extension.data();

    /* Create Validations Messenger Info */
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (ENABLE_VALIDATION_LAYERS)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    } else {
        createInfo.enabledExtensionCount = 0;
    }

    /*Create Vulkan Instance*/
    if (vkCreateInstance(&createInfo, nullptr, &context.instance) != VK_SUCCESS)
    {
        throw std::runtime_error("{ERROR} FAILED TO CREATE VK_INSTANCE.");
    }
};

/* Get Required Extensions for GLFW Window & Validations Layers */
std::vector<const char*> getRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (ENABLE_VALIDATION_LAYERS)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
};

/* Creates a Vulkan Surface for a VulkanContext */
void createSurface(vulkanContext& context, GLFWwindow* window)
{
    if (glfwCreateWindowSurface(context.instance, window, nullptr, &context.surface) != VK_SUCCESS)
    {
        throw std::runtime_error("{ERROR} FAILED TO CREATE SURFACE.");
    };
};

/* Picks and sets the Phyiscal Device in a VulkanContext */
void pickPhysicalDevice(vulkanContext& context)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(context.instance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        throw std::runtime_error("{ERROR} FAILED TO FIND ANY GPU WITH VULKAN SUPPORT.");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(context.instance, &deviceCount, devices.data());

    for (const auto& device : devices)
    {
        VkPhysicalDeviceProperties deviceInfo{};
        vkGetPhysicalDeviceProperties(device, &deviceInfo);

        if (isDeviceSuitable(device, context.surface))
        {
            context.physicalDevice = device;
            std::cout << "{INFO} GPU: " << deviceInfo.deviceName << " was selected as the phyiscalDevice.\n";
            break;
        }
    }

    if (context.physicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error("{ERROR} FAILED TO FIND A PHYISCAL DEVICE.");
    }
};

/* Returns True if the GPU is supported */
bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    QueueFamilyIndices indices = findQueueFamilies(device, surface);
    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported)
    {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(surface, device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

        if (presentSupport)
        {
            indices.presentFamily = i;
        }

        if(indices.isComplete())
        {
            break;
        }

        i++;
    }

    return indices;
};

bool checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtension(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtension.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtension)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
};

/* Creates a Vulkan Device for a VulkanContext */
void createLogicalDevice(vulkanContext& context)
{
    QueueFamilyIndices indices = findQueueFamilies(context.physicalDevice, context.surface);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{}; // Do I really need this ??
    if (ENABLE_VALIDATION_LAYERS)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(context.physicalDevice, &createInfo, nullptr, &context.device) != VK_SUCCESS)
    {
        throw std::runtime_error("{ERROR} FAILED TO CREATE LOGICAL DEVICE.");
    }

    /* Check Limits and Print to Terminal*/
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(context.physicalDevice, &properties);
    VkPhysicalDeviceLimits deviceLimits = properties.limits;
    std::cout << "####### Device Limits #######" << std::endl;
    std::cout << "maxBoundDescriptorSets: " << deviceLimits.maxBoundDescriptorSets << std::endl;
    std::cout << "maxDescriptorSetUniformBuffers: " << deviceLimits.maxDescriptorSetUniformBuffers << std::endl;


    vkGetDeviceQueue(context.device, indices.graphicsFamily.value(), 0, &context.graphicQueue);
    vkGetDeviceQueue(context.device, indices.presentFamily.value(), 0, &context.presentQueue);
};

/* Creates the Vulkan SwapChains for a VulkanContext */
void createSwapChain(vulkanContext& context, GLFWwindow* window)
{
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(context.surface, context.physicalDevice);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, window);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = context.surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(context.physicalDevice, context.surface);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(context.device, &createInfo, nullptr, &context.swapChain) != VK_SUCCESS)
    {
        throw std::runtime_error("{ERROR} FAILED TO CREATE VK SWAPCHAIN.");
    }

    vkGetSwapchainImagesKHR(context.device, context.swapChain, &imageCount, nullptr);
    context.swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(context.device, context.swapChain, &imageCount, context.swapChainImages.data());

    context.swapChainImageFormat = surfaceFormat.format;
    context.swapChainExtent = extent;
};

/* Need to move this to vulkanTextures.hpp*/
void createImage(vulkanContext& context, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
                 VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
{
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.usage = usage;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0;

    if (vkCreateImage(context.device, &imageInfo, nullptr, &image) != VK_SUCCESS)
    {
        throw std::runtime_error("{ERROR} FAILED TO CREATE IMAGE.");
    };

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(context.device, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(context, memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(context.device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("{ERROR} FAILED TO ALLOCATE IMAGE MEMORY.");
    };

    vkBindImageMemory(context.device, image, imageMemory, 0);
};

/* Creates the Vulkan ImageViews for a VulkanContext */
void createImageViews(vulkanContext& context)
{
    context.swapChainImageViews.resize(context.swapChainImages.size());

    for (uint32_t i = 0; i < context.swapChainImages.size(); i++)
    {
        context.swapChainImageViews[i] = createImageView(context, context.swapChainImages[i], context.swapChainImageFormat,  VK_IMAGE_ASPECT_COLOR_BIT);
    }
};

/* Creates a VkImageView on a VulkanContext*/
/* Need to move this to vulkanTextures.hpp*/
VkImageView createImageView(vulkanContext& context, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(context.device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
    {
        throw std::runtime_error("{ERROR} FAILED TO CREATE IMAGE VIEW.");
    }

    return imageView;
};

/* Creates the Vulkan RenderPass for a VulkanContext */
void createRenderPass(vulkanContext& context)
{
    /* Color Attachment */
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = context.swapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    /* Depth Attachment */
    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = findDepthFormat(context);
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    std::array<VkAttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(context.device, &renderPassInfo, nullptr, &context.renderPass) != VK_SUCCESS)
    {
        throw std::runtime_error("{ERROR} FAILED TO CREATE RENDER PASS.");
    };
};

VkFormat findDepthFormat(vulkanContext& context)
{
    std::vector<VkFormat> wantedFormats = {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT};
    return findSupportFormat(context,
                            wantedFormats,
                            VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
};
VkFormat findSupportFormat(vulkanContext& context, std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
    for (VkFormat format : candidates)
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(context.physicalDevice, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.optimalTilingFeatures & features) == features)
        {
            return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
        {
            return format;
        }
    }

    throw std::runtime_error("{ERROR} FAILED TO FIND SUPPORTED FORMAT");
};

void createDescriptorSetLayout(vulkanContext& context, Scene* scene)
{
    for (auto& ent : scene->m_Entities)
    {
        if (scene->m_ShaderComponents.find(ent) != scene->m_ShaderComponents.end()) // TODO double check this
        {
            std::vector<VkDescriptorSetLayoutBinding> bindings;
            ShaderComponent currentShader = scene->m_ShaderComponents.at(ent);
            
            /* Scene UBO with Texture Sampler  DescriptorSetLayout 0 */
            VkDescriptorSetLayoutBinding sceneBufferBinding{};
            sceneBufferBinding.binding = 0;
            sceneBufferBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            sceneBufferBinding.descriptorCount = 1; //?? TODO check this
            sceneBufferBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            sceneBufferBinding.pImmutableSamplers = nullptr;

            VkSampler samplers[1] = {context.textureSampler};
            VkDescriptorSetLayoutBinding samplerBindings{};
            samplerBindings.binding = 1;
            samplerBindings.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
            samplerBindings.descriptorCount = 1;
            samplerBindings.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            samplerBindings.pImmutableSamplers = samplers;

            /* Object UBO DescriptorSetLayout 1 */
            VkDescriptorSetLayoutBinding objectBufferBinding{};
            objectBufferBinding.binding = 0;
            objectBufferBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            objectBufferBinding.descriptorCount = 1;
            objectBufferBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            objectBufferBinding.pImmutableSamplers = nullptr;
            
            // https://web.engr.oregonstate.edu/~mjb/vulkan/Handouts/DescriptorSets.1pp.pdf
            // TODO need to create more that one descriptor sets 
            // SceneLayout set + Include the Sampler Here too

            /* Set == 0*/
            std::vector<VkDescriptorSetLayout> layout;

            VkDescriptorSetLayout sceneDescriptorLayout;
            std::vector<VkDescriptorSetLayoutBinding> sceneBindings;
            sceneBindings.push_back(sceneBufferBinding);
            sceneBindings.push_back(samplerBindings);

            VkDescriptorSetLayoutCreateInfo sceneDescriptorLayoutInfo{};
            sceneDescriptorLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            sceneDescriptorLayoutInfo.bindingCount = static_cast<uint32_t>(sceneBindings.size());
            sceneDescriptorLayoutInfo.pBindings = sceneBindings.data();

            ASSERT_VK_RESULT(vkCreateDescriptorSetLayout(context.device, &sceneDescriptorLayoutInfo, nullptr, &sceneDescriptorLayout), VK_SUCCESS, "Scene Descriptor Set");
            layout.push_back(sceneDescriptorLayout);

            // ObjectLayout set
            /* Set == 1*/
            VkDescriptorSetLayout objectDescriptorLayout;
            std::vector<VkDescriptorSetLayoutBinding> objectBindings;
            objectBindings.push_back(objectBufferBinding);

            VkDescriptorSetLayoutCreateInfo objectDescriptorLayoutInfo{};
            objectDescriptorLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            objectDescriptorLayoutInfo.bindingCount = static_cast<uint32_t>(objectBindings.size());
            objectDescriptorLayoutInfo.pBindings = objectBindings.data();

            ASSERT_VK_RESULT(vkCreateDescriptorSetLayout(context.device, &objectDescriptorLayoutInfo, nullptr, &objectDescriptorLayout), VK_SUCCESS, "Object Descriptor Set");
            layout.push_back(objectDescriptorLayout);

            // TextureLayout set 
            /* Set == 2*/
            VkDescriptorSetLayout textureDescriptorLayout;
            std::vector<VkDescriptorSetLayoutBinding> textureBindings;
            /* Texture  DescriptorSetLayout 1 */

            // Find every texture and create an sampled image for it.
            // Can look into texture Arrays in the future.
            if (scene->m_TextureComponents.find(ent) != scene->m_TextureComponents.end())
            {
                uint32_t texCount = 0;
                for (auto& texture : scene->m_TextureComponents.at(ent))
                {
                    texCount++;
                    VkDescriptorSetLayoutBinding imageBindings{};
                    imageBindings.binding = texCount;
                    imageBindings.descriptorCount = 1;
                    imageBindings.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
                    imageBindings.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
                    imageBindings.pImmutableSamplers = &context.textureSampler; //TODO double chcek this??
                    textureBindings.push_back(imageBindings);
                };
            };

            VkDescriptorSetLayoutCreateInfo textureDescriptorLayoutInfo{};
            textureDescriptorLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            textureDescriptorLayoutInfo.bindingCount = static_cast<uint32_t>(textureBindings.size());
            textureDescriptorLayoutInfo.pBindings = textureBindings.data();
            
            if (textureBindings.size() != 0)
            {
                ASSERT_VK_RESULT(vkCreateDescriptorSetLayout(context.device, &textureDescriptorLayoutInfo, nullptr, &textureDescriptorLayout), VK_SUCCESS, "Texture Descriptor Set");
                layout.push_back(textureDescriptorLayout);
            };

            context.descriptorSetLayoutsLists[ent] = layout;
        }
    }
};

void createGraphicsPipelineLayout(vulkanContext& context, Scene* scene)
{
    for (auto& ent : scene->m_Entities)
    {
        if (scene->m_ShaderComponents.find(ent) != scene->m_ShaderComponents.end())
        {
            /* Hopefully this wont be needed ever soon */
            VkPushConstantRange  pushConstantRange{};
            pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            pushConstantRange.offset = 0;
            pushConstantRange.size = sizeof(PushConstantTextureIndex);

            /* Will need to points to all of the descriptor sets we want to use
            and count how many we have*/
            VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
            pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(context.descriptorSetLayoutsLists.at(ent).size());
            pipelineLayoutInfo.pSetLayouts = context.descriptorSetLayoutsLists.at(ent).data(); // TODO double check later
            pipelineLayoutInfo.pushConstantRangeCount = 1;
            pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
            //pipelineLayoutInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_PER_STAGE_BIT_NV; //TODO TRYING OUT
            
            /*This will need to be unique  for each entity */
            VkPipelineLayout pipelineLayout;
            ASSERT_VK_RESULT(vkCreatePipelineLayout(context.device, &pipelineLayoutInfo, nullptr, &pipelineLayout), VK_SUCCESS, "Graphics Pipeline Layout");
            context.pipelineLayouts[ent] = pipelineLayout;
        }
    }
};

//TODO this function is hot trash, we shouldnt be looping with this functions and have 
// different loops running in other functions for the creation process of the shaders
// seems like just a waste of time doing that.  
void createGraphicsPipeline(vulkanContext& context, Scene* scene, Entity ent)// TODO START BACK HERE
{
    ShaderComponent shader = scene->m_ShaderComponents.at(ent);

    auto vertShaderCode = readShaderSourceFile(shader.vertexSourcePath);
    auto fragShaderCode = readShaderSourceFile(shader.fragmentSourcePath);

    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode, context.device);
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode, context.device);
    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) context.swapChainExtent.width;
    viewport.height = (float) context.swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = context.swapChainExtent;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_NONE;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;

    VkPipelineMultisampleStateCreateInfo multiSampling{};
    multiSampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multiSampling.sampleShadingEnable = VK_FALSE;
    multiSampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f;
    depthStencil.maxDepthBounds = 1.0f;
    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.front = {};
    depthStencil.back = {};

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;

    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multiSampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;

    pipelineInfo.layout = context.pipelineLayouts.at(ent);
    pipelineInfo.renderPass = context.renderPass;
    pipelineInfo.subpass = 0;

    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    VkPipeline graphicsPipline;
    if (vkCreateGraphicsPipelines(context.device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipline) != VK_SUCCESS)
    {
        throw std::runtime_error("{ERROR} FAILED TO CREATE GRAPHIC PIPELINE");
    };
    context.graphicsPiplines[ent] = graphicsPipline;

    vkDestroyShaderModule(context.device, vertShaderModule, nullptr);
    vkDestroyShaderModule(context.device, fragShaderModule, nullptr);
};

// TODO ADD SUPPORT FOR COMPUTE Command Pool :)
void createCommandPool(vulkanContext& context)
{
    QueueFamilyIndices QueueFamilyIndices = findQueueFamilies(context.physicalDevice, context.surface);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = QueueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(context.device, &poolInfo, nullptr, &context.commandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("{ERROR} FAILED TO CREATE COMMAND POOL.");
    }

};

void createDepthResources(vulkanContext& context)
{
    VkFormat depthFormat = findDepthFormat(context);

    createImage(context, context.swapChainExtent.width, context.swapChainExtent.height, depthFormat,
                VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, context.depthImage, context.depthImageMemory);
    context.depthImageView = createImageView(context, context.depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

    transitionImageLayout(context, context.depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED,
                        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
};

void createFramebuffers(vulkanContext& context)
{
    context.swapChainFramesBuffers.resize(context.swapChainImageViews.size());

    for (size_t i = 0; i < context.swapChainImageViews.size(); i++)
    {
        std::array<VkImageView, 2> attachments = {
            context.swapChainImageViews[i],
            context.depthImageView};

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = context.renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = context.swapChainExtent.width;
        framebufferInfo.height = context.swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(context.device, &framebufferInfo, nullptr, &context.swapChainFramesBuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("{ERROR} FAILED TO CREATE FRAMEBUFFERS.");
        };
    };
};

void createVertexBuffer(vulkanContext& context, std::vector<Vertex>& verticesInput)
{
    VkDeviceSize bufferSize = sizeof(verticesInput[0]) * verticesInput.size();

    /* Staging Buffer */
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(context, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(context.device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, verticesInput.data(), (size_t) bufferSize);
    vkUnmapMemory(context.device, stagingBufferMemory);

    /* Transfer Stage Buffer to Stage Buffer */
    createBuffer(context, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, context.vertexBuffer, context.vertexBufferMemory);
    copyBuffer(context, stagingBuffer, context.vertexBuffer, bufferSize);

    vkDestroyBuffer(context.device, stagingBuffer, nullptr);
    vkFreeMemory(context.device, stagingBufferMemory, nullptr);
};

void createIndexBuffer(vulkanContext& context, std::vector<uint32_t>& indicesInput)
{
    VkDeviceSize bufferSize = sizeof(indicesInput[0]) * indicesInput.size();

    /* Staging Buffer */
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    createBuffer(context, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(context.device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indicesInput.data(), (size_t) bufferSize);
    vkUnmapMemory(context.device, stagingBufferMemory);

    /* Transfer Stage Buffer to Stage Buffer */
    createBuffer(context, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, context.indexBuffer, context.indexBufferMemory);
    copyBuffer(context, stagingBuffer, context.indexBuffer, bufferSize);

    vkDestroyBuffer(context.device, stagingBuffer, nullptr);
    vkFreeMemory(context.device, stagingBufferMemory, nullptr);
};

/* Should be in vulkanBuffer.hpp seems pretty random being here?*/
void createUniformBuffer(vulkanContext& context, uniformLayout layout, VkDeviceSize bufferSize)
{
    std::vector<VkBuffer> buffers;
    std::vector<VkDeviceMemory> bufferMemorys;
    std::vector<void*> bufferMappings;
   
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        VkBuffer buffer;
        VkDeviceMemory bufferMemory;
        void* bufferMapping;

        createBuffer(context, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     buffer, bufferMemory);
        vkMapMemory(context.device, bufferMemory, 0, bufferSize, 0, &bufferMapping);

        buffers.push_back(buffer);
        bufferMemorys.push_back(bufferMemory);
        bufferMappings.push_back(bufferMapping);
    };

    if (layout == sceneType)
    {
        context.uniformBuffers[sceneType] = buffers;
        context.uniformBuffersMemory[sceneType] = bufferMemorys;
        context.uniformBufferMapped[sceneType] = bufferMappings;
    } 
    else if (layout  == objectType)
    {
        context.uniformBuffers[objectType] = buffers;
        context.uniformBuffersMemory[objectType] = bufferMemorys;
        context.uniformBufferMapped[objectType] = bufferMappings;
    }
};

void createDescriptorPool(vulkanContext& context, Scene* scene)
{
    int extraSpace = 2; 

    std::vector<VkDescriptorPoolSize> poolSize{};

    VkDescriptorPoolSize sceneBufferPoolSize{};
    sceneBufferPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    sceneBufferPoolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * extraSpace);
    poolSize.push_back(sceneBufferPoolSize);

    VkDescriptorPoolSize objectBufferPoolSize{};
    objectBufferPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    objectBufferPoolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * extraSpace);
    poolSize.push_back(sceneBufferPoolSize);
    
    VkDescriptorPoolSize textureSamplerPoolSize{};
    textureSamplerPoolSize.type = VK_DESCRIPTOR_TYPE_SAMPLER;
    textureSamplerPoolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * extraSpace);
    poolSize.push_back(textureSamplerPoolSize);

    /* Texture Images Sorted out here*/
    // TODO Double check here, could be wrong
   for (auto& ent : scene->m_Entities)
   {
        if (scene->m_TextureComponents.find(ent) != scene->m_TextureComponents.end()) // double check this
        {
            VkDescriptorPoolSize texturePoolSize{};
            texturePoolSize.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
            texturePoolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * extraSpace);
            poolSize.push_back(texturePoolSize);
        } 
   }

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSize.size());
    poolInfo.pPoolSizes = poolSize.data();
    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * extraSpace);

    auto result = vkCreateDescriptorPool(context.device, &poolInfo, nullptr, &context.descriptorPool);
    ASSERT_VK_RESULT(result, VK_SUCCESS, "Create Descriptor Pool");
};


void createDescriptorSets(vulkanContext& context, Scene* scene)
{
    for (auto& ent : scene->m_Entities)
    {
        if (scene->m_ShaderComponents.find(ent) != scene->m_ShaderComponents.end())
        {
            ShaderComponent shader = scene->m_ShaderComponents.at(ent);

            VkDescriptorSetAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = context.descriptorPool;
            allocInfo.descriptorSetCount = static_cast<uint32_t>(context.descriptorSetLayoutsLists.at(ent).size()); // double check this as well 
            allocInfo.pSetLayouts = context.descriptorSetLayoutsLists.at(ent).data();

            // TODO NOT SURE IF THIS IS RIGHT 
            std::vector<VkDescriptorSet> tempDescriptorSet(MAX_FRAMES_IN_FLIGHT * 2);

            auto result = vkAllocateDescriptorSets(context.device, &allocInfo, &tempDescriptorSet[0]);
            ASSERT_VK_RESULT(result, VK_SUCCESS, "Allocate Descriptor Sets");
            context.descriptorSets[ent] = tempDescriptorSet;

            // TODO YEAH IM A LITTLE STUCK HERE !!!!
            
            // Create Descriptor Buffer Info Per Binding*
            for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
            {
                // SceneBufferInfo
                VkDescriptorBufferInfo sceneBufferInfo{};
                sceneBufferInfo.buffer = context.uniformBuffers.at(sceneType)[i]; 
                sceneBufferInfo.offset = 0; 
                sceneBufferInfo.range = sizeof(SceneUniformBuffer);

                // ObjectBufferInfo
                VkDescriptorBufferInfo objectBufferInfo{};
                objectBufferInfo.buffer = context.uniformBuffers.at(objectType)[i];
                objectBufferInfo.offset = 0;
                objectBufferInfo.range = sizeof(ObjectUniformBuffer);

                // Create Write Descriptor Set 
                std::vector<VkWriteDescriptorSet> setWrites;

                // SceneWriteInfo
                VkWriteDescriptorSet sceneWriteSet{};
                sceneWriteSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                sceneWriteSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                sceneWriteSet.descriptorCount = 1; // TODO not sure ???
                sceneWriteSet.dstSet = context.descriptorSets.at(ent)[0]; // TODO double check this ??
                sceneWriteSet.dstBinding = 0;
                sceneWriteSet.dstArrayElement = 0;
                sceneWriteSet.pBufferInfo = &sceneBufferInfo;
                setWrites.push_back(sceneWriteSet);

                VkWriteDescriptorSet textureSamplerWriteSet{};
                textureSamplerWriteSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                textureSamplerWriteSet.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
                textureSamplerWriteSet.descriptorCount = 1;
                textureSamplerWriteSet.dstSet = context.descriptorSets.at(ent)[0];
                textureSamplerWriteSet.dstBinding = 1;
                textureSamplerWriteSet.dstArrayElement = 0;
                setWrites.push_back(textureSamplerWriteSet);

                // ObjectWriteInfo
                VkWriteDescriptorSet objectWriteSet{};
                objectWriteSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                objectWriteSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                objectWriteSet.descriptorCount = 1;
                objectWriteSet.dstSet = context.descriptorSets.at(ent)[1]; // TODO i really dont like doing this, seems really wrong
                objectWriteSet.dstBinding = 0;
                objectWriteSet.dstArrayElement = 0;
                objectWriteSet.pBufferInfo = &objectBufferInfo;

                //  ImageInfo for each Texture
                std::vector<VkDescriptorImageInfo> imageInfos;
                uint32_t imageCount = 0;
                for (auto& ent : scene->m_Entities)
                {
                    if (scene->m_TextureComponents.find(ent) != scene->m_TextureComponents.end())
                    {
                        for (auto& texture : scene->m_TextureComponents.at(ent))
                        {
                            imageCount++;
                            VkDescriptorImageInfo imageInfo{};
                            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                            imageInfo.imageView = context.textureImageViews[texture.ID - 1]; // TODO HOT TRASH!! 
                            imageInfo.sampler = context.textureSampler;
                            imageInfos.push_back(imageInfo);
                        }
                    }
                }
                // TextureWriteInfo
                VkWriteDescriptorSet texturesWriteSet{};
                texturesWriteSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                texturesWriteSet.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
                texturesWriteSet.descriptorCount = imageCount;
                texturesWriteSet.pImageInfo = imageInfos.data(); 
                texturesWriteSet.dstBinding = 2;
                texturesWriteSet.dstSet = context.descriptorSets.at(ent)[2];

                // Update Descriptor Sets x3
                vkUpdateDescriptorSets(context.device, 2, setWrites.data(), 0, nullptr);
                vkUpdateDescriptorSets(context.device, 1, &objectWriteSet, 0, nullptr);
                vkUpdateDescriptorSets(context.device, 1, &texturesWriteSet, 0, nullptr);
            };
        }
    }
};

void createCommandBuffers(vulkanContext& context)
{
    context.commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = context.commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) context.commandBuffers.size();

    if (vkAllocateCommandBuffers(context.device, &allocInfo, context.commandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("{ERROR} FAILED TO ALLOCATE COMMAND BUFFERS");
    };
};

void createSyncObjects(vulkanContext& context)
{
    context.imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    context.renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    context.inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        if (vkCreateSemaphore(context.device, &semaphoreInfo, nullptr, &context.imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(context.device, &semaphoreInfo, nullptr, &context.renderFinishedSemaphores[i])!= VK_SUCCESS ||
            vkCreateFence(context.device, &fenceInfo, nullptr, &context.inFlightFences[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("{ERRRO} FAILED TO CREATE SYNC OBJECTS");
        };
    };
};