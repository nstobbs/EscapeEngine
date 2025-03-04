#ifndef SCENE_COMPONENT_H
#define SCENE_COMPONENT_H

#include "../config.hpp"

#include "../entity/entity.hpp"
#include "../components/cameraComponent.hpp"
#include "../components/meshComponent.hpp"
#include "../components/transformComponent.hpp"
#include "../components/shaderComponent.hpp"
#include "../components/textureComponent.hpp"
#include "../components/boidsComponent.hpp"
#include "../backend/vulkanContext.hpp" //TODO Feel like this is over stepping a bit with the backend. Vulkan shouldnt be used here

class Scene
{
    public:
        Scene();
        Entity makeEntity();

        CameraComponent getActiveCamera();
        void setActiveCamera(CameraComponent camera);
        glm::mat4 getCameraView();
        glm::mat4 getCameraProj(vulkanContext& context);

        void setEntityTransformComponent(Entity ent, TransformComponent transform);
        void setEntityMeshComponent(Entity ent, MeshComponent mesh);
        void setEntityShaderComponent(Entity ent, ShaderComponent shader);
        void setEntityBoidsComponent(Entity ent, BoidsComponent boids);
        void pushEntityTextureComponent(Entity ent, TextureComponent texture);


        // TODO dont really use these. Should Really switch over to it at somepoint
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
        std::unordered_map<Entity, BoidsComponent> m_BoidsComponents;
        std::vector<Entity> m_Entities;
        CameraComponent m_ActiveCamera;

    private:
        uint32_t m_EntitiesCount;
};

#endif