#ifndef BACKEND_VULKAN_SHADER
#define BACKEND_VULKAN_SHADER

#include "../config.hpp"

#include "vulkanContext.hpp"
#include "vulkanUtilities.hpp"


std::vector<char> readShaderSourceFile(const std::string& filename); // TODO not really a vulkan related function and should be moved
VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice device);
void createBoidsDescriptorSetLayout(vulkanContext& context);
void createBoidsComputePipelineLayout(vulkanContext& context);
void createBoidsComputePipeline(vulkanContext& context);

#endif