#ifndef BACKEND_VULKAN_BUFFER
#define BACKEND_VULKAN_BUFFER

#include "../config.hpp"

#include "vulkanContext.hpp"

uint32_t findMemoryType(vulkanContext& context, uint32_t typeFiler, VkMemoryPropertyFlags properties);

#endif