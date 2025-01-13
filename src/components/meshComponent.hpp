#ifndef MESH_COMPONENT_H
#define MESH_COMPONENT_H

#include "../config.cpp"

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color; // Don't really need this
    glm::vec3 texCoord;
};

struct MeshDetails {
    uint32_t firstVertex;
    uint32_t firstIndex;
};

struct MeshComponent {
    uint32_t verticesCount;
    uint32_t indicesCount;
    std::vector<float> vertices; // Temp Set to Vertex
    std::vector<uint32_t> indices;

    /* ID FOR BACKEND */
    uint32_t ID;
    MeshDetails details;
};

#endif