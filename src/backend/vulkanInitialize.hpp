#ifndef BACKEND_VULKAN_INIT
#define BACKEND_VULKAN_INIT

#include "../config.hpp"
#include "vulkanContext.hpp"
#include "vulkanDebug.hpp"
#include "vulkanUtilities.hpp"
#include "vulkanSwapChain.hpp"
#include "vulkanShader.hpp"
#include "vulkanBuffer.hpp"
#include "vulkanTexture.hpp"

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete()
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

const int MAX_FRAMES_IN_FLIGHT = 2;

void createVulkanInstance(vulkanContext& context);
std::vector<const char*> getRequiredExtensions();
void createSurface(vulkanContext& context, GLFWwindow* window);
void pickPhysicalDevice(vulkanContext& context);
bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
bool checkDeviceExtensionSupport(VkPhysicalDevice device);
void createLogicalDevice(vulkanContext& context);
void createSwapChain(vulkanContext& context, GLFWwindow* window); // Could move into vulkanSwapChain.hpp
void createImage(vulkanContext& context, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
                 VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
void createImageViews(vulkanContext& context);
VkImageView createImageView(vulkanContext& context, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
void createRenderPass(vulkanContext& context);
VkFormat findDepthFormat(vulkanContext& context);
VkFormat findSupportFormat(vulkanContext& context, std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
void createDescriptorSetLayout(vulkanContext& context);
void createGraphicsPipelineLayout(vulkanContext& context);
void createGraphicsPipeline(vulkanContext& context, std::string& fragmentPath, std::string& vertexPath);
void createCommandPool(vulkanContext& context);
void createDepthResources(vulkanContext& context);
void createFramebuffers(vulkanContext& context);
/* TEMP CREATE VERTEX/INDEX BUFFER, WILL NEED TO SWITCH TO USING VERTEX INSTEAD OF FLOAT */
void createVertexBuffer(vulkanContext& context, std::vector<Vertex>& verticesInput); // Could move into vulkanBuffer.hpp
void createIndexBuffer(vulkanContext& context, std::vector<uint32_t>& indicesInput);
void createUniformBuffer(vulkanContext& context); // TODO Instead of one UBO create one for each shaderComponent
void createDescriptorPool(vulkanContext& context);
void createDescriptorSets(vulkanContext& context, Scene* scene);
void createCommandBuffers(vulkanContext& context);
void createSyncObjects(vulkanContext& context);

#endif