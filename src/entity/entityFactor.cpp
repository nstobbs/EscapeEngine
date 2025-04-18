#include "entityFactor.hpp"

EntityFactor::EntityFactor(Scene* scene)
{
    m_scene = scene;
}

Entity EntityFactor::createGrid()
{
    Entity ent = m_scene->makeEntity();
    MeshComponent mesh;
    TransformComponent transform;
    ShaderComponent shader;
    TextureComponent texture;

    std::vector<Vertex> vertices(4);

    float gridScale = 100.0f;
    float uvScale = 20.0f;

    vertices[0].pos = glm::vec3(-gridScale, gridScale, 0.0f);
    vertices[0].texCoord = glm::vec2(0.0f, uvScale);
    vertices[1].pos = glm::vec3(gridScale, gridScale, 0.0f);
    vertices[1].texCoord = glm::vec2(uvScale, uvScale);
    vertices[2].pos = glm::vec3(-gridScale, -gridScale, 0.0f);
    vertices[2].texCoord = glm::vec2(0.0f, 0.0f);
    vertices[3].pos = glm::vec3(gridScale, -gridScale, 0.0f);
    vertices[3].texCoord = glm::vec2(uvScale, 0.0f);

    mesh.vertices = vertices;
    mesh.verticesCount = 4;
    mesh.indices = {0, 2, 1, 1, 2, 3};
    mesh.indicesCount = 6;

    transform.position = glm::mat4(1.0f);

    shader.fragmentSourcePath = "../../src/shaders/GridFrag.spv";
    shader.vertexSourcePath = "../../src/shaders/GridVert.spv";

    texture.texturePath = "../../textures/testTexture.png";

    m_scene->setEntityMeshComponent(ent, mesh);
    m_scene->setEntityTransformComponent(ent, transform);
    m_scene->setEntityShaderComponent(ent, shader);
    m_scene->pushEntityTextureComponent(ent, texture);

    return ent;
};

Entity EntityFactor::createOBJMesh(std::string objFilePath, std::vector<std::string> textureFilepaths)
{
    Entity ent = m_scene->makeEntity();
    MeshComponent mesh;
    TransformComponent transform;
    ShaderComponent shader;
    std::vector<TextureComponent> textures;
    
    mesh.filepath = objFilePath;
    objLoader(mesh);

    transform.position = glm::mat4(1.0f);

    // Uses Default Shader for now
    shader.vertexSourcePath = "../../src/shaders/DefaultShaderVert.spv";
    shader.fragmentSourcePath = "../../src/shaders/DefaultShaderFrag.spv";

    for (auto& texture : textureFilepaths)
    {
        TextureComponent t;
        t.texturePath = texture;
        m_scene->pushEntityTextureComponent(ent, t);
    };

    m_scene->setEntityMeshComponent(ent, mesh);
    m_scene->setEntityTransformComponent(ent, transform);
    m_scene->setEntityShaderComponent(ent, shader);

    return ent;
};