#include "objLoader.hpp"

void objLoader(MeshComponent& mesh)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, mesh.filepath.c_str()))
    {
        std::cout << "{ERROR} ";
        throw std::runtime_error(warn + err);
    }

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            Vertex vertex{};

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            mesh.vertices.push_back(vertex);
            mesh.indices.push_back(mesh.indices.size());
        }
    }

    mesh.indicesCount = static_cast<uint32_t>(mesh.indices.size());
    mesh.verticesCount = static_cast<uint32_t>(mesh.vertices.size());
};