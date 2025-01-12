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
};

void Application::loop()
{
    std::cout << "{INFO} Starting Loop...\n";
    while(!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();
    }
};

void Application::tearDown()
{
    std::cout << "{INFO} Starting TearDown...\n";
    glfwDestroyWindow(m_window);
    glfwTerminate();
};