#ifndef BACKEND_IMGUI_INIT
#define BACKEND_IMGUI_INIT

#include "../config.hpp"

#include "vulkanContext.hpp"
#include "vulkanUtilities.hpp"

void initializeImgui(vulkanContext &context, GLFWwindow *pWindow);

#endif