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