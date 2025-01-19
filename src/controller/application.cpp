#include "application.hpp"

Application::Application(Scene& currentScene)
{
    m_Scene = &currentScene;
};

void Application::run()
{
    startUp();
    loop();
    tearDown();
};

void Application::startUp()
{
    m_window = initializeWindow(m_vulkanContext);
    createVulkanInstance(m_vulkanContext);
    setupDebugMessenger(m_vulkanContext);
    createSurface(m_vulkanContext, m_window);
    pickPhysicalDevice(m_vulkanContext);
    createLogicalDevice(m_vulkanContext);
    createSwapChain(m_vulkanContext, m_window);
    createImageViews(m_vulkanContext);
    createRenderPass(m_vulkanContext);
    createDescriptorSetLayout(m_vulkanContext);

    /* All ShaderComponents should be setup
    before creating the GraphicsPiplines */
    createGraphicsPipelineLayout(m_vulkanContext); /* Might Need to add this to the loop if shaders need different UniformBuffers */
    m_vulkanContext.shaderCount = 0;
    for (auto& [entityID, shader] : m_Scene->m_ShaderComponents)
    {
        m_vulkanContext.shaderCount++;
        shader.ID = m_vulkanContext.shaderCount;
        createGraphicsPipeline(m_vulkanContext, shader.fragmentSourcePath, shader.vertexSourcePath);
    };

    createCommandPool(m_vulkanContext);
    createDepthResources(m_vulkanContext);
    createFramebuffers(m_vulkanContext);
    
    /* Texture Loading Happens Here - WIP */
    /* This is bad to have two loops fix later*/
    for (auto& [entityID, textures] : m_Scene->m_TextureComponents)
    {
        for (auto& texture : textures)
        {
            createTextureImage(m_vulkanContext,texture.texturePath);
        };
    };
    createTextureImageView(m_vulkanContext);
    createTextureSampler(m_vulkanContext);

    /* All MeshComponents should be setup
    before creating the VertexBuffers */
    m_vulkanContext.meshCount = 0;
    uint32_t firstVertex = 0;
    uint32_t firstIndex = 0;
    std::vector<float> meshData;
    std::vector<uint32_t> indicesData;
    uint32_t indexOffset = 0;
    for (auto& [entityID, mesh] : m_Scene->m_MeshComponents)
    {
        m_vulkanContext.meshCount++;
        mesh.ID = m_vulkanContext.meshCount;
        for (float& vertex : mesh.vertices)
        {
            meshData.push_back(vertex); // not really a vertex here, but each float. So each thrid float would be one whole vertex.
        };
        mesh.details.firstVertex = firstVertex;
        firstVertex = firstVertex + mesh.verticesCount;

        for (uint32_t& index : mesh.indices)
        {
            indicesData.push_back((index + indexOffset));
        };
        indexOffset += mesh.verticesCount;
        mesh.details.firstIndex = firstIndex;
        firstIndex = firstIndex + mesh.indicesCount;
    };
    createVertexBuffer(m_vulkanContext, meshData);
    createIndexBuffer(m_vulkanContext, indicesData);
    createUniformBuffer(m_vulkanContext);
    createDescriptorPool(m_vulkanContext);
    createDescriptorSets(m_vulkanContext);
    createCommandBuffers(m_vulkanContext);
    createSyncObjects(m_vulkanContext);
};

void Application::loop()
{
    std::cout << "{INFO} Starting Loop...\n";
    RenderSystem RSystem(m_vulkanContext, m_Scene, m_window);
    RSystem.start();
    while(!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();
        /* System Updates Happen Here*/
        RSystem.update();
    }
};

void Application::tearDown()
{
    std::cout << "{INFO} Starting TearDown...\n";
    vkDeviceWaitIdle(m_vulkanContext.device);
    cleanupSwapChain(m_vulkanContext);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroyBuffer(m_vulkanContext.device, m_vulkanContext.uniformBuffers[i], nullptr);
        vkFreeMemory(m_vulkanContext.device, m_vulkanContext.uniformBuffersMemory[i], nullptr);
    };

    vkDestroyDescriptorPool(m_vulkanContext.device, m_vulkanContext.descriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(m_vulkanContext.device, m_vulkanContext.descriptorSetLayout, nullptr);

    vkDestroyBuffer(m_vulkanContext.device, m_vulkanContext.indexBuffer, nullptr);
    vkFreeMemory(m_vulkanContext.device, m_vulkanContext.vertexBufferMemory, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(m_vulkanContext.device, m_vulkanContext.imageAvailableSemaphores[i], nullptr);
        vkDestroySemaphore(m_vulkanContext.device, m_vulkanContext.renderFinishedSemaphores[i], nullptr);
        vkDestroyFence(m_vulkanContext.device, m_vulkanContext.inFlightFences[i], nullptr);
    };

    vkDestroyCommandPool(m_vulkanContext.device, m_vulkanContext.commandPool, nullptr);

    for (auto& graphicPipeline : m_vulkanContext.graphicsPiplines)
    {
        vkDestroyPipeline(m_vulkanContext.device, graphicPipeline, nullptr);
    };

    vkDestroyPipelineLayout(m_vulkanContext.device, m_vulkanContext.pipelineLayout, nullptr);
    vkDestroyRenderPass(m_vulkanContext.device, m_vulkanContext.renderPass, nullptr);

    if (ENABLE_VALIDATION_LAYERS)
    {
        DestroyDebugUtilsMessengerEXT(m_vulkanContext.instance, m_vulkanContext.debugMessenger, nullptr);
    };

    vkDestroyDevice(m_vulkanContext.device, nullptr);

    vkDestroySurfaceKHR(m_vulkanContext.instance, m_vulkanContext.surface, nullptr);
    vkDestroyInstance(m_vulkanContext.instance, nullptr);

    glfwDestroyWindow(m_window);
    glfwTerminate();
};