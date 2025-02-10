#ifndef BACKEND_VULKAN_UTILITIES
#define BACKEND_VULKAN_UTILITIES

#include "../config.hpp"

#include <vulkan/vk_enum_string_helper.h>
#include "vulkanContext.hpp"

void ASSERT_VK_RESULT(VkResult result, VkResult expected, std::string msg);
VkVertexInputBindingDescription getBindingDescription();
std::array<VkVertexInputAttributeDescription, 1> getAttributeDescriptions();
VkCommandBuffer beginSingleTimeCommands(vulkanContext& context);
void endSingleTimeCommands(vulkanContext& context, VkCommandBuffer commandBuffer);
void transitionImageLayout(vulkanContext& context, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
bool hasStencilComponent(VkFormat format);

#endif