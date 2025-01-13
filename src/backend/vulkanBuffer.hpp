#ifndef BACKEND_VULKAN_BUFFER
#define BACKEND_VULKAN_BUFFER

#include "../config.hpp"

#include "vulkanContext.hpp"
#include "vulkanUtilities.hpp"

uint32_t findMemoryType(vulkanContext& context, uint32_t typeFiler, VkMemoryPropertyFlags properties);
void createBuffer(vulkanContext& context, VkDeviceSize size, VkBufferUsageFlags usage, 
                  VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
void copyBuffer(vulkanContext& context,VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

#endif