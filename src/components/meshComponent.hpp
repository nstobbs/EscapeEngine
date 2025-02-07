#ifndef MESH_COMPONENT_H
#define MESH_COMPONENT_H

#include "../config.cpp"

struct Vertex {
    glm::vec3 pos;
    glm::vec2 texCoord;

    /* These Functions Describes how the vertex buffer is formatted */
    static VkVertexInputBindingDescription getBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    };

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);
        
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    };

};

struct ObjectUniformBuffer
{
    glm::mat4 model;
};

struct MeshDetails {
    uint32_t firstVertex;
    uint32_t firstIndex;
};

struct MeshComponent {
    /* Data */
    uint32_t verticesCount; // check if I need this
    uint32_t indicesCount;
    
    // maybe dont store this data on the cpu the whole time 
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    /* Filepath */
    std::string filepath;

    /* ID FOR BACKEND */
    uint32_t ID;
    MeshDetails details;
};

#endif