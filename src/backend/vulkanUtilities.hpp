#ifndef BACKEND_VULKAN_UTILITIES
#define BACKEND_VULKAN_UTILITIES

#include "../config.hpp"

struct UniformBufferObject {
    glm::mat4 modelTransforms; /* Should Use from current Entity transformComponent */
    glm::mat4 cameraView; /* Should Use Scene::Camera */
    glm::mat4 cameraProjection; 
};

void checkVulkanResult(VkResult result);
VkVertexInputBindingDescription getBindingDescription();
std::array<VkVertexInputAttributeDescription, 1> getAttributeDescriptions();

#endif