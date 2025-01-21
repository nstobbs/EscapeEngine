#include "cameraSystem.hpp"

CameraSystem::CameraSystem(Scene* scene, GLFWwindow* window)
{
    m_scene = scene;
    m_window = window;
};

void CameraSystem::update()
{   
    CameraComponent cam = m_scene->m_ActiveCamera;

    // Keyboard Input 
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_TRUE)
    {
        // Forward
        m_scene->m_ActiveCamera.position += cameraSpeed *  cam.front;
    };

    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_TRUE)
    {
        // Left
    m_scene->m_ActiveCamera.position -= glm::normalize(glm::cross(cam.front, cam.up)) * cameraSpeed;
    };

    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_TRUE)
    {
        // Backward
        m_scene->m_ActiveCamera.position -= cameraSpeed *  cam.front;
    };

    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_TRUE)
    {
        // Right
        m_scene->m_ActiveCamera.position += glm::normalize(glm::cross(cam.front, cam.up)) * cameraSpeed;
    };

    // Mouse Input
    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Hide Cursor
        double x,y;
        glfwGetCursorPos(m_window, &x, &y);

        float offsetX = (x - lastX) * mouseSen;
        float offsetY = (y - lastY) * mouseSen;
        lastX = x;
        lastY = y;

        yaw += offsetX;
        pitch += offsetY;

        if (pitch > 89.9f)
        {
            pitch = 89.0f;
        }

        if (pitch < -89.0f)
        {
            pitch = -89.0f;
        }

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        m_scene->m_ActiveCamera.front = glm::normalize(direction); 

    }

    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
    {
        /* Return the Cursor*/
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
};  