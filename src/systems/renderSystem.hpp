#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../config.hpp"
#include "../backend/vulkanContext.hpp"
#include "../backend/vulkanSwapChain.hpp"
#include "../controller/scene.hpp"

class RenderSystem
{
    public:
        RenderSystem(vulkanContext& context, Scene& scene, GLFWwindow& window);
        /* Update Will be the Function to start Drawing the Frame*/
        void start();
        void update();

    private:

        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        vulkanContext& m_context;
        GLFWwindow* m_window;
        
        Scene* m_scene;
        std::vector<Entity*> m_renderableEntities;
};

#endif