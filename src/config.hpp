#ifndef CONFIG_H
#define CONFIG_H

#include <vulkan/vulkan.h>

/* If there is any errors
related to GLFW and Vulkan
this move this part back into
config.cpp*/
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

#endif
