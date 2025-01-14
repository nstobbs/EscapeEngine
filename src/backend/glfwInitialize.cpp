#include "glfwInitialize.hpp"

const int WIDTH = 800;
const int HEIGHT = 600;

GLFWwindow* initializeWindow(vulkanContext& context)
{
    std::cout << "{INFO} initializeWindow started...\n";
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window;
    window = glfwCreateWindow(WIDTH, HEIGHT, "ESC", nullptr, nullptr); // TODO: MAKE CONST IN GLOBE FILE
    if (window == NULL)
    {
        throw std::runtime_error("{ERROR} FAILED TO CREATE WINDOW");
    };
    glfwSetWindowUserPointer(window, &context);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

    return window;
};

static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    auto context = reinterpret_cast<vulkanContext*>(glfwGetWindowUserPointer(window));
    context->frameBufferResized = true;
};
