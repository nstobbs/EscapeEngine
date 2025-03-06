#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H

#include "../config.hpp"
#include "../controller/scene.hpp"

class CameraSystem
{
    public:
        CameraSystem(Scene* scene, GLFWwindow* window);
        void start();
        void update(float delta);
        
    private:
        Scene* m_scene;
        GLFWwindow* m_window;
        float cameraSpeed = 0.05f;
        float mouseSen = 0.1f;
        float lensSpeed = 0.01f; 
        double lastX, lastY;

        double yaw = -90.0f;
        double pitch = 0.0f;
};

#endif