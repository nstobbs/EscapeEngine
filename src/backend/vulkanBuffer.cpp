#include "vulkanBuffer.hpp"

uint32_t findMemoryType(vulkanContext& context, uint32_t typeFiler, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(context.physicalDevice, &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFiler & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    throw std::runtime_error("{ERROR} FAILED TO FIND SUITABLE MEMORY TYPE.");

};

void createBuffer(vulkanContext& context, VkDeviceSize size, VkBufferUsageFlags usage, 
                  VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(context.device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    {
        std::runtime_error("{ERROR} FAILED TO CREATE BUFFER.");
    };

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(context.device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(context, memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(context.device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("{ERROR} FAILED TO ALLOCATE BUFFER MEMORY.");
    };

    vkBindBufferMemory(context.device, buffer, bufferMemory, 0);

};

void copyBuffer(vulkanContext& context,VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkCommandBuffer commandBuffer = beginSingleTimeCommands(context);

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(context, commandBuffer);
};

void copyBufferToImage(vulkanContext& context, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
    VkCommandBuffer commandBuffer = beginSingleTimeCommands(context);
    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = {
        width,
        height,
        1
    };

    vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
    endSingleTimeCommands(context, commandBuffer);
};

void updateUniformBuffer(vulkanContext& context, Scene* scene, TransformComponent& transform)
{
    /* Don't need this clock right now but could be useful later... */
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    SceneUniformBuffer sceneUBO{};
    //sceneUBO.view = glm::lookAt(scene->m_ActiveCamera.position, scene->m_ActiveCamera.position + scene->m_ActiveCamera.front, scene->m_ActiveCamera.up);
    //sceneUBO.proj = glm::perspective(glm::radians(scene->getActiveCamera().focalLength),
    //                        (float) context.swapChainExtent.width / (float) context.swapChainExtent.height, 0.1f, 100.0f);
    //sceneUBO.proj[1][1] *= -1;
    sceneUBO.view = glm::mat4(1.0f);
    sceneUBO.proj = glm::mat4(1.0f);


    ObjectUniformBuffer objectUBO{};
    //objectUBO.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    objectUBO.model = glm::mat4(1.0f);


    /* TODO This doesn't seems to make any sense.
    Vulkan creates a void* pointer for us to use
    when we create the uniform buffer in the first place.
    We NEED to memcpy to the address that we created and
    assigned before with vulkan instead of trying to create
    a new one.
    */

    memcpy(context.uniformBufferMapped.at(sceneType)[context.currentFrame], &sceneUBO, sizeof(sceneUBO));
    memcpy(context.uniformBufferMapped.at(objectType)[context.currentFrame], &objectUBO, sizeof(objectUBO));
};