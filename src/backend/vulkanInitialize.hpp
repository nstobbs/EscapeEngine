#ifndef BACKEND_VULKAN_INIT
#define BACKEND_VULKAN_INIT

#include "../config.hpp"
#include "vulkanContext.hpp"
#include "vulkanDebug.hpp"
#include "vulkanUtilities.hpp"
#include "vulkanSwapChain.hpp"
#include "vulkanShader.hpp"

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

void createVulkanInstance(vulkanContext& context);
std::vector<const char*> getRequiredExtensions();
void createSurface(vulkanContext& context, GLFWwindow* window);
void pickPhysicalDevice(vulkanContext& context);
bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
bool checkDeviceExtensionSupport(VkPhysicalDevice device);
void createLogicalDevice(vulkanContext& context);
void createSwapChain(vulkanContext& context, GLFWwindow* window); // Could move into vulkanSwapChain.hpp
void createImageViews(vulkanContext& context);
VkImageView createImageView(vulkanContext& context, uint32_t imageIndex, VkImageAspectFlags aspectFlags);
void createRenderPass(vulkanContext& context);
VkFormat findDepthFormat(vulkanContext& context);
VkFormat findSupportFormat(vulkanContext& context, std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
void createDescriptorSetLayout(vulkanContext& context);
void createGraphicsPipelineLayout(vulkanContext& context);
void createGraphicsPipeline(vulkanContext& context, std::string& fragmentPath, std::string& vertexPath);
void createCommandPool(vulkanContext& context);

#endif