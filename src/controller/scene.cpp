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

glm::mat4 Scene::getCameraView()
{
    return glm::lookAt(Scene::m_ActiveCamera.position, Scene::m_ActiveCamera.front, Scene::m_ActiveCamera.up);
}

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