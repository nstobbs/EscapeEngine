#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H

#include "../config.hpp"
#include "../controller/scene.hpp"

class CameraSystem
{
    public:
        CameraSystem(Scene* scene, GLFWwindow* window);
        void update();
        
    private:
        Scene* m_scene;
        GLFWwindow* m_window;
        float cameraSpeed = 1.0f;
        float mouseSen = 0.05f; 
        double lastX, lastY = 600; // TODO Set this to the center of the Window size at creations instead of this.

        float yaw = -90.0f;
        float pitch = 0.0f;
};

#endif