#ifndef BACKEND_VULKAN_SWAPCHAIN
#define BACKEND_VULKAN_SWAPCHAIN

#include "../config.hpp"
#include "vulkanContext.hpp"
#include "vulkanInitialize.hpp"

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

SwapChainSupportDetails querySwapChainSupport(VkSurfaceKHR surface, VkPhysicalDevice device);
VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
void recreateSwapChain(vulkanContext& context, GLFWwindow* window);
void cleanupSwapChain(vulkanContext& context);


#endif