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

        void setEntityTransformComponent(Entity ent, TransformComponent transform);
        void setEntityMeshComponent(Entity ent, MeshComponent mesh);
        void setEntityShaderComponent(Entity ent, ShaderComponent shader);
        void pushEntityTextureComponent(Entity ent, TextureComponent texture);

        MeshComponent getMeshComponent(Entity ent);
        TransformComponent getTransformComponent(Entity ent);
        ShaderComponent getShaderComponent(Entity ent);

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