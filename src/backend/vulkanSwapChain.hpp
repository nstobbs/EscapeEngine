#ifndef BACKEND_VULKAN_SWAPCHAIN
#define BACKEND_VULKAN_SWAPCHAIN

#include "../config.hpp"
#include "vulkanContext.hpp"

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

SwapChainSupportDetails querySwapChainSupport(VkSurfaceKHR surface, VkPhysicalDevice device);

#endif