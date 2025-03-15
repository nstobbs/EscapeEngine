#include "scene.hpp"

Scene::Scene()
{
    m_EntitiesCount = 0;
}

Entity Scene::makeEntity()
{
    m_EntitiesCount++;
    Entity ent = m_EntitiesCount;
    m_Entities.push_back(ent);
    return ent;
};

CameraComponent Scene::getActiveCamera()
{
    return m_ActiveCamera;
};

void Scene::setActiveCamera(CameraComponent camera)
{
    m_ActiveCamera = camera;
};

// TODO Shouldn't really be a part of the Scene Class really
glm::mat4 Scene::getCameraView()
{
    return glm::lookAt(m_ActiveCamera.position, m_ActiveCamera.position + m_ActiveCamera.direction, m_ActiveCamera.up);
}
//TODO could be better 
glm::mat4 Scene::getCameraProj(vulkanContext& context)
{
    auto matrix = glm::perspective(glm::radians(m_ActiveCamera.focalLength), 
                  static_cast<float>(context.swapChainExtent.width /  context.swapChainExtent.height), 
                  0.1f, 100.0f);
    matrix[1][1] *= -1;

    return matrix;
};

void Scene::setEntityTransformComponent(Entity ent, TransformComponent transform)
{
    m_TransformComponents[ent] = transform;
};

void Scene::setEntityMeshComponent(Entity ent, MeshComponent mesh)
{
    m_MeshComponents[ent] = mesh;
};

void Scene::setEntityShaderComponent(Entity ent, ShaderComponent shader)
{
    m_ShaderComponents[ent] = shader;
};

void Scene::setEntityBoidsComponent(Entity ent, BoidsComponent boids)
{
    m_BoidsComponents[ent] = boids;
};

void Scene::pushEntityTextureComponent(Entity ent, TextureComponent texture)
{
    m_TextureComponents[ent].push_back(texture);
};

TransformComponent Scene::getTransformComponent(Entity ent)
{
    return m_TransformComponents[ent];
};

MeshComponent Scene::getMeshComponent(Entity ent)
{
    return m_MeshComponents[ent];
};

ShaderComponent Scene::getShaderComponent(Entity ent)
{
    return m_ShaderComponents[ent];
};