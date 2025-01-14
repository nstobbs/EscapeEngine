#include "scene.hpp"

Scene::Scene()
{
    m_EntitiesCount = 0;
}

Entity Scene::makeEntity()
{
    m_EntitiesCount++;
    return m_EntitiesCount;
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

void Scene::setEntityTransformComponent(Entity ID, TransformComponent transform)
{
    m_TransformComponents[ID] = transform;
};

void Scene::setEntityMeshComponent(Entity ID, MeshComponent mesh)
{
    m_MeshComponents[ID] = mesh;
};

void Scene::setEntityShaderComponent(Entity ID, ShaderComponent shader)
{
    m_ShaderComponents[ID] = shader;
};

TransformComponent Scene::getTransformComponent(Entity ID)
{
    return m_TransformComponents[ID];
};

MeshComponent Scene::getMeshComponent(Entity ID)
{
    return m_MeshComponents[ID];
};

ShaderComponent Scene::getShaderComponent(Entity ID)
{
    return m_ShaderComponents[ID];
};