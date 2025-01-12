#ifndef BACKEND_VULKAN_SHADER
#define BACKEND_VULKAN_SHADER

#include "../config.hpp"

std::vector<char> readShaderSourceFile(const std::string& filename);
VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice device);

#endif