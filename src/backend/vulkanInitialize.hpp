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

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

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
void createDescriptorSetLayout(vulkanContext& context, Scene* scene);
void createGraphicsPipelineLayout(vulkanContext& context, Scene* scene);
void createGraphicsPipeline(vulkanContext& context, Scene* scene, Entity ent);
void createCommandPool(vulkanContext& context);
void createDepthResources(vulkanContext& context);
void createFramebuffers(vulkanContext& context);

void createDescriptorPool(vulkanContext& context, Scene* scene);
void createDescriptorSets(vulkanContext& context, Scene* scene);

void createCommandBuffers(vulkanContext& context);
void createSyncObjects(vulkanContext& context);

void createBoidsDescriptorSets(vulkanContext& context, Scene* scene);

#endif