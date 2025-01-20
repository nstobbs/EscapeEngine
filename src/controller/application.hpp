#ifndef APPLICATION_H
#define APPLICATION_H

#include "../config.hpp"

#include "../backend/vulkanContext.hpp"
#include "../backend/glfwInitialize.hpp"
#include "../backend/vulkanInitialize.hpp"
#include "../backend/vulkanDebug.hpp"
#include "../backend/vulkanTexture.hpp"

#include "../systems/renderSystem.hpp"
#include "../systems/cameraSystem.hpp"

#include "scene.hpp"

class Application
{
    public:
        Application(Scene& currentScene);
        void run();
        
    private:
        void startUp();
        void loop();
        void tearDown();

        Scene* m_Scene;
        GLFWwindow* m_window;
        vulkanContext m_vulkanContext;
};

#endif