#ifndef MESH_COMPONENT_H
#define MESH_COMPONENT_H

#include "../config.cpp"

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color; // Don't really need this
    glm::vec3 texCoord;
};

struct MeshComponent {
    uint32_t verticesCount;
    std::vector<float> vertices; // Temp Set to Vertex
    std::vector<uint32_t> indices;
};
#endif