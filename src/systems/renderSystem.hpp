#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../config.hpp"
#include "../backend/vulkanContext.hpp"
#include "../backend/vulkanSwapChain.hpp"
#include "../controller/scene.hpp"

class RenderSystem
{
    public:
        RenderSystem(vulkanContext& context, Scene* scene, GLFWwindow* window);
        /* Update Will be the Function to start Drawing the Frame*/
        void start();
        void update();

    private:
        Scene* m_scene;
        GLFWwindow* m_window;
        vulkanContext& m_context;

        std::vector<Entity> m_renderableEntities;
};

#endif