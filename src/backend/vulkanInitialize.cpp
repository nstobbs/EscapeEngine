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