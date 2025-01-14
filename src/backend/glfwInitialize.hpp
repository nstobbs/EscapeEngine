#ifndef BACKEND_GLFW_INIT
#define BACKEND_GLFW_INIT

#include "../config.hpp"
#include "vulkanContext.hpp"

GLFWwindow* initializeWindow(vulkanContext& context);
static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

#endif
