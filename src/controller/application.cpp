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
    m_window = initializeWindow();
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
};

void Application::loop()
{
    std::cout << "{INFO} Starting Loop...\n";
    while(!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();
        /* System Updates Happen Here*/
    }
};

void Application::tearDown()
{
    std::cout << "{INFO} Starting TearDown...\n";
    if (ENABLE_VALIDATION_LAYERS)
    {
        DestroyDebugUtilsMessengerEXT(m_vulkanContext.instance, m_vulkanContext.debugMessenger, nullptr);
    }

    vkDestroySurfaceKHR(m_vulkanContext.instance, m_vulkanContext.surface, nullptr);
    vkDestroyInstance(m_vulkanContext.instance, nullptr);

    glfwDestroyWindow(m_window);
    glfwTerminate();
};