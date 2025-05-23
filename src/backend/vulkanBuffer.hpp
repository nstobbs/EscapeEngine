#ifndef BACKEND_VULKAN_BUFFER
#define BACKEND_VULKAN_BUFFER

#include "../config.hpp"
#include "../controller/scene.hpp"

#include "vulkanContext.hpp"
#include "vulkanUtilities.hpp"

struct SceneUniformBuffer
{
    glm::mat4 view;
    glm::mat4 proj;
    // should manager the sampler too.
};

struct ObjectUniformBuffer
{
    glm::mat4 model;
};

struct TextureIndexPush 
{
    int textureIndex;
};

uint32_t findMemoryType(vulkanContext& context, uint32_t typeFiler, VkMemoryPropertyFlags properties);
void createBuffer(vulkanContext& context, VkDeviceSize size, VkBufferUsageFlags usage, 
                  VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
void copyBuffer(vulkanContext& context,VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
void copyBufferToImage(vulkanContext& context, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
void updateUniformBuffer(vulkanContext& context, Scene* scene, TransformComponent& transform);
std::pair<std::vector<Vertex>, std::vector<uint32_t>> layoutMeshesForVertexBuffer(vulkanContext& context, Scene* scene);
void createVertexBuffer(vulkanContext& context, std::vector<Vertex>& verticesInput);
void createIndexBuffer(vulkanContext& context, std::vector<uint32_t>& indicesInput);
void createUniformBuffer(vulkanContext& context, uniformLayout layout, VkDeviceSize bufferSize);
void createBoidsStorageBuffer(vulkanContext& context, std::vector<Boid>& boids);
void updateBoidsUniformBuffer(vulkanContext& context, BoidsComponent& data);

#endif