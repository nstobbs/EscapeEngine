#include "vulkanUtilities.hpp"

void checkVulkanResult(VkResult result)
{
    if (!result == VK_SUCCESS)
    {
        throw std::runtime_error("{ERROR} Vulkan Function failed.");
    }
}