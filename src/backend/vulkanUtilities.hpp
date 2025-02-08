#ifndef BACKEND_VULKAN_UTILITIES
#define BACKEND_VULKAN_UTILITIES

#include "../config.hpp"

#include "vulkanContext.hpp"

void checkVulkanResult(VkResult result);
VkVertexInputBindingDescription getBindingDescription();
std::array<VkVertexInputAttributeDescription, 1> getAttributeDescriptions();
VkCommandBuffer beginSingleTimeCommands(vulkanContext& context);
void endSingleTimeCommands(vulkanContext& context, VkCommandBuffer commandBuffer);
void transitionImageLayout(vulkanContext& context, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
bool hasStencilComponent(VkFormat format);

#endif