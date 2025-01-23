#ifndef MESH_COMPONENT_H
#define MESH_COMPONENT_H

#include "../config.cpp"

struct Vertex {
    glm::vec3 pos;
    glm::vec2 texCoord;
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