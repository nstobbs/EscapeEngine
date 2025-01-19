#ifndef BACKEND_VULKAN_TEXTURE
#define BACKEND_VULKAN_TEXTURE

#include "../config.hpp"
#include "vulkanContext.hpp"
#include "vulkanBuffer.hpp"

#include "../../lib/stb_image.h"


void createImage(vulkanContext& context, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
                VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
void createTextureImage(vulkanContext& context, const std::string texturePath);
VkImageView createImageView(vulkanContext& context, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
void createTextureImageView(vulkanContext& context);
void createTextureSampler(vulkanContext& context);

#endif