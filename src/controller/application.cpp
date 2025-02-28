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

    createCommandPool(m_vulkanContext);

    /* Texture Loading Happens Here - WIP */
    /* This is bad to have two loops fix later*/
    m_vulkanContext.textureCount = static_cast<uint32_t>(0);
    for (auto& [entityID, textures] : m_Scene->m_TextureComponents)
    {
        for (auto& texture : textures)
        {
            m_vulkanContext.textureCount++;
            uint32_t textureID = m_vulkanContext.textureCount;
            texture.ID = textureID;
            m_Scene->m_ShaderComponents.at(entityID).textureIDs.push_back(textureID);
            createTextureImage(m_vulkanContext,texture.texturePath);
        };
    };
    createTextureImageView(m_vulkanContext);
    createTextureSampler(m_vulkanContext);
    
    /* All ShaderComponents should be setup
    before creating the GraphicsPiplines */
    // all of these create functions should be running under the same loop together
    // instead of each looping over the shaderComponents maps 
    // when you can do it all once in one loop 
    m_vulkanContext.shaderCount = 0;
    createDescriptorSetLayout(m_vulkanContext, m_Scene); // We should know how many textures samplers we are going to use
    createGraphicsPipelineLayout(m_vulkanContext, m_Scene);
    for (auto& [entityID, shader] : m_Scene->m_ShaderComponents)
    {
        m_vulkanContext.shaderCount++;
        shader.ID = m_vulkanContext.shaderCount;
        createGraphicsPipeline(m_vulkanContext, m_Scene, entityID); 
    };

    
    createDepthResources(m_vulkanContext);
    createFramebuffers(m_vulkanContext);

    /* All MeshComponents should be setup
    before creating the VertexBuffers */
    // Move this into it's own functoin
    auto data = layoutMeshesForVertexBuffer(m_vulkanContext, m_Scene);
    createVertexBuffer(m_vulkanContext, data.first);
    createIndexBuffer(m_vulkanContext, data.second);

    /* Uniform Buffers and Textures */
    //createUniformBuffer(m_vulkanContext, sizeof(UniformBufferObject)); // OLD
    createUniformBuffer(m_vulkanContext, sceneType, sizeof(SceneUniformBuffer));
    createUniformBuffer(m_vulkanContext, objectType, sizeof(ObjectUniformBuffer));
    createDescriptorPool(m_vulkanContext, m_Scene);
    createDescriptorSets(m_vulkanContext, m_Scene);

    /* Command Buffer & Sync Objects */
    createCommandBuffers(m_vulkanContext);
    createSyncObjects(m_vulkanContext);

    /* Start up ImGui ..*/
    initializeImgui(m_vulkanContext, m_window);
};

void Application::loop()
{
    std::cout << "{INFO} Starting Loop...\n";
    CameraSystem cameraSystem(m_Scene, m_window);
    RenderSystem renderSystem(m_vulkanContext, m_Scene, m_window);
    renderSystem.start();
    cameraSystem.start();
    float delta = 0.0f;

    //TODO Double check the timing maths here 
    while(!glfwWindowShouldClose(m_window))
    {
        // Start Timer
        auto start = std::chrono::high_resolution_clock::now();

        glfwPollEvents();
        renderSystem.update();
        cameraSystem.update(); // crash when it's placed before the renderSystem ??


        auto end = std::chrono::high_resolution_clock::now();
        delta = std::chrono::duration<float, std::chrono::seconds::period>(end - start).count();
        //std::cout << "frame timing: " << delta << std::endl;
        //std::cout << "framerate: " << fps << std::endl;
    }
};

void Application::tearDown()
{
    std::cout << "{INFO} Starting TearDown...\n";
    vkDeviceWaitIdle(m_vulkanContext.device);
    cleanupSwapChain(m_vulkanContext);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroyBuffer(m_vulkanContext.device, m_vulkanContext.uniformBuffers.at(sceneType)[i], nullptr);
        vkFreeMemory(m_vulkanContext.device, m_vulkanContext.uniformBuffersMemory.at(sceneType)[i], nullptr);
        vkDestroyBuffer(m_vulkanContext.device, m_vulkanContext.uniformBuffers.at(objectType)[i], nullptr);
        vkFreeMemory(m_vulkanContext.device, m_vulkanContext.uniformBuffersMemory.at(objectType)[i], nullptr);
    };

    vkDestroyDescriptorPool(m_vulkanContext.device, m_vulkanContext.descriptorPool, nullptr);
    for (auto& [ent, descriptorSetLayouts] : m_vulkanContext.descriptorSetLayoutsLists)
    {
        for (auto& descriptorSetLayout : descriptorSetLayouts)
        {
             vkDestroyDescriptorSetLayout(m_vulkanContext.device, descriptorSetLayout, nullptr);
        }
    };

    vkDestroyBuffer(m_vulkanContext.device, m_vulkanContext.indexBuffer, nullptr);
    vkFreeMemory(m_vulkanContext.device, m_vulkanContext.vertexBufferMemory, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(m_vulkanContext.device, m_vulkanContext.imageAvailableSemaphores[i], nullptr);
        vkDestroySemaphore(m_vulkanContext.device, m_vulkanContext.renderFinishedSemaphores[i], nullptr);
        vkDestroyFence(m_vulkanContext.device, m_vulkanContext.inFlightFences[i], nullptr);
    };

    vkDestroyCommandPool(m_vulkanContext.device, m_vulkanContext.commandPool, nullptr);

    for (auto& [ent, graphicPipeline] : m_vulkanContext.graphicsPiplines)
    {
        vkDestroyPipeline(m_vulkanContext.device, graphicPipeline, nullptr);
    };

    for (auto& [ent, pipelineLayout] : m_vulkanContext.pipelineLayouts)
    {
        vkDestroyPipelineLayout(m_vulkanContext.device, pipelineLayout, nullptr);
    };

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