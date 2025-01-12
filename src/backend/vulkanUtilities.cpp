#include "vulkanUtilities.hpp"

void checkVulkanResult(VkResult result)
{
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("{ERROR} Vulkan Function failed.");
    }
};
/* TODO TEMP Function. Will need to be moved to meshComponent or VulkanBackend stuff */
/* These Functions Describes how the vertex buffer is format */
VkVertexInputBindingDescription getBindingDescription()
{
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(float) * 3;
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
};

std::array<VkVertexInputAttributeDescription, 1> getAttributeDescriptions()
{
    std::array<VkVertexInputAttributeDescription, 1> attributeDescriptions{};
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = 0;

    return attributeDescriptions;
};