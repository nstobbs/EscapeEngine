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


//TODO Might be about time to start thinking about separating these functions for 
void updateUniformBuffer(vulkanContext& context, Scene* scene, TransformComponent& transform)
{
    SceneUniformBuffer sceneUBO{};

    sceneUBO.view = scene->getCameraView();
    sceneUBO.proj = scene->getCameraProj(context);
    //sceneUBO.proj[1][1] *= -1;

    ObjectUniformBuffer objectUBO{};
    //objectUBO.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    objectUBO.model = glm::mat4(1.0f);

    memcpy(context.uniformBufferMapped.at(sceneType)[context.currentFrame], &sceneUBO, sizeof(sceneUBO));
    memcpy(context.uniformBufferMapped.at(objectType)[context.currentFrame], &objectUBO, sizeof(objectUBO));
};

// TODO This whole function needs redoing or something
std::pair<std::vector<Vertex>, std::vector<uint32_t>> layoutMeshesForVertexBuffer(vulkanContext& context, Scene* scene)
{
    context.meshCount = 0;
    uint32_t firstVertex = 0;
    uint32_t firstIndex = 0;
    std::vector<Vertex> meshData;
    std::vector<uint32_t> indicesData;
    uint32_t indexOffset = 0;
    for (auto& [entityID, mesh] : scene->m_MeshComponents)
    {
        context.meshCount++;
        mesh.ID = context.meshCount;
        for (Vertex& vertex : mesh.vertices)
        {
            meshData.push_back(vertex);
        };
        mesh.details.firstVertex = firstVertex;
        firstVertex += mesh.verticesCount;

        for (uint32_t& index : mesh.indices)
        {
            indicesData.push_back((index + indexOffset));
        };
        indexOffset += mesh.verticesCount;
        mesh.details.firstIndex = firstIndex;
        firstIndex = firstIndex + mesh.indicesCount;
    };
    
    std::pair<std::vector<Vertex>, std::vector<uint32_t>> data;
    data.first = meshData;
    data.second = indicesData;
    return data;
};

void createVertexBuffer(vulkanContext& context, std::vector<Vertex>& verticesInput)
{
    VkDeviceSize bufferSize = sizeof(verticesInput[0]) * verticesInput.size();

    /* Staging Buffer */
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(context, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(context.device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, verticesInput.data(), (size_t) bufferSize);
    vkUnmapMemory(context.device, stagingBufferMemory);

    /* Transfer Stage Buffer to Stage Buffer */
    createBuffer(context, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, context.vertexBuffer, context.vertexBufferMemory);
    copyBuffer(context, stagingBuffer, context.vertexBuffer, bufferSize);

    vkDestroyBuffer(context.device, stagingBuffer, nullptr);
    vkFreeMemory(context.device, stagingBufferMemory, nullptr);
};

void createIndexBuffer(vulkanContext& context, std::vector<uint32_t>& indicesInput)
{
    VkDeviceSize bufferSize = sizeof(indicesInput[0]) * indicesInput.size();

    /* Staging Buffer */
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    createBuffer(context, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(context.device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indicesInput.data(), (size_t) bufferSize);
    vkUnmapMemory(context.device, stagingBufferMemory);

    /* Transfer Stage Buffer to Stage Buffer */
    createBuffer(context, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, context.indexBuffer, context.indexBufferMemory);
    copyBuffer(context, stagingBuffer, context.indexBuffer, bufferSize);

    vkDestroyBuffer(context.device, stagingBuffer, nullptr);
    vkFreeMemory(context.device, stagingBufferMemory, nullptr);
};

void createUniformBuffer(vulkanContext& context, uniformLayout layout, VkDeviceSize bufferSize)
{
    std::vector<VkBuffer> buffers;
    std::vector<VkDeviceMemory> bufferMemorys;
    std::vector<void*> bufferMappings;
   
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        VkBuffer buffer;
        VkDeviceMemory bufferMemory;
        void* bufferMapping;

        createBuffer(context, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     buffer, bufferMemory);
        vkMapMemory(context.device, bufferMemory, 0, bufferSize, 0, &bufferMapping);

        buffers.push_back(buffer);
        bufferMemorys.push_back(bufferMemory);
        bufferMappings.push_back(bufferMapping);
    };

    if (layout == sceneType)
    {
        context.uniformBuffers[sceneType] = buffers;
        context.uniformBuffersMemory[sceneType] = bufferMemorys;
        context.uniformBufferMapped[sceneType] = bufferMappings;
    } 
    else if (layout  == objectType)
    {
        context.uniformBuffers[objectType] = buffers;
        context.uniformBuffersMemory[objectType] = bufferMemorys;
        context.uniformBufferMapped[objectType] = bufferMappings;
    }
    else if (layout == boidType)
    {
        context.boidsBuffersUBO = buffers;
        context.boidsBufferMemorysUBO = bufferMemorys;
        context.boidsBufferMappingsUBO = bufferMappings;
    }
};

void createBoidsStorageBuffer(vulkanContext& context, std::vector<Boid>& boids)
{
    VkDeviceSize bufferSize = sizeof(Boid) * boids.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    void* data;

    createBuffer(context, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer, stagingBufferMemory);
    auto result = vkMapMemory(context.device, stagingBufferMemory, 0, bufferSize, 0, &data);
    ASSERT_VK_RESULT(result, VK_SUCCESS, "Mapping Boids Storage Buffer");
    memcpy(data, boids.data(), bufferSize);
    vkUnmapMemory(context.device, stagingBufferMemory);
    
    for (int i = 0; i < 2; i++)
    {
        VkBuffer tempBuffer;
        VkDeviceMemory tempDeviceMemory;

        createBuffer(context, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, tempBuffer, tempDeviceMemory);
        copyBuffer(context, stagingBuffer, tempBuffer, bufferSize);
        context.boidsBuffers.push_back(tempBuffer);
        context.boidsBufferMemorys.push_back(tempDeviceMemory);
    };    

    vkDestroyBuffer(context.device, stagingBuffer, nullptr);
    vkFreeMemory(context.device, stagingBufferMemory, nullptr);
};

void updateBoidsUniformBuffer(vulkanContext& context, BoidsComponent& data)
{
    static auto startTime = std::chrono::high_resolution_clock::now();

    BoidsSim sim;
    sim.sim_separation = data.sim_separation;
    sim.sim_cohesion = data.sim_cohesion;
    sim.sim_alignment = data.sim_alignment;

    sim.sim_protected_range = data.sim_protected_range;
    sim.sim_visible_range = data.sim_visible_range;
    sim.sim_turn_factor = data.sim_turn_factor;
    sim.sim_max_speed = data.sim_max_speed;
    sim.sim_min_speed = data.sim_min_speed;
    sim.sim_update_rate = data.sim_update_rate;
    
    sim.height = data.height;
    sim.width = data.width;
    sim.depth = data.depth;
    
    sim.sim_boidsCount = data.boidsCount;

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    sim.deltaTime = time;

    memcpy(context.boidsBufferMappingsUBO[context.currentFrame], &sim, sizeof(BoidsSim));
};