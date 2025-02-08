#ifndef BACKEND_VULKAN_TEXTURE
#define BACKEND_VULKAN_TEXTURE

#include "../config.hpp"
#include "vulkanContext.hpp"
#include "vulkanBuffer.hpp"
#include "vulkanInitialize.hpp"

void createTextureImage(vulkanContext& context, const std::string texturePath);
void createTextureImageView(vulkanContext& context);
void createTextureSampler(vulkanContext& context);

#endif