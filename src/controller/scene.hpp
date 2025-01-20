#ifndef SCENE_COMPONENT_H
#define SCENE_COMPONENT_H

#include "../config.hpp"

#include "../entity/entity.hpp"
#include "../components/cameraComponent.hpp"
#include "../components/meshComponent.hpp"
#include "../components/transformComponent.hpp"
#include "../components/shaderComponent.hpp"
#include "../components/textureComponent.hpp"

class Scene
{
    public:
        Scene();
        Entity makeEntity();

        CameraComponent getActiveCamera();
        void setActiveCamera(CameraComponent camera);
        glm::mat4 getCameraView();

        void setEntityTransformComponent(Entity ID, TransformComponent transform);
        void setEntityMeshComponent(Entity ID, MeshComponent mesh);
        void setEntityShaderComponent(Entity ID, ShaderComponent shader);

        MeshComponent getMeshComponent(Entity ID);
        TransformComponent getTransformComponent(Entity ID);
        ShaderComponent getShaderComponent(Entity ID);

        /* TODO Rethink how to handle this.
        Was in Private but needs to be public
        only for the Application Class */
        std::unordered_map<Entity, MeshComponent> m_MeshComponents;
        std::unordered_map<Entity, TransformComponent> m_TransformComponents;
        std::unordered_map<Entity, ShaderComponent> m_ShaderComponents;
        std::unordered_map<Entity, std::vector<TextureComponent>> m_TextureComponents;
        std::vector<Entity> m_Entities;
        CameraComponent m_ActiveCamera;

    private:
        uint32_t m_EntitiesCount;
};

#endif