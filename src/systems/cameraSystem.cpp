#include "cameraSystem.hpp"

CameraSystem::CameraSystem(Scene* scene, GLFWwindow* window)
{
    m_scene = scene;
    m_window = window;
};

void CameraSystem::update(float delta)
{   
    /* KeyBoard Input */
    /* Forward */
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_TRUE)
    {
        m_scene->m_ActiveCamera.position += cameraSpeed * m_scene->m_ActiveCamera.position;
    };

    /* Left */
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_TRUE)
    {
        m_scene->m_ActiveCamera.position -= glm::normalize(glm::cross(m_scene->m_ActiveCamera.front, m_scene->m_ActiveCamera.up)) * cameraSpeed;
    };

    /* Backward */
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_TRUE)
    {
        m_scene->m_ActiveCamera.position -= cameraSpeed *  m_scene->m_ActiveCamera.position;
    };

    /* Right */
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_TRUE)
    {
        m_scene->m_ActiveCamera.position += glm::normalize(glm::cross(m_scene->m_ActiveCamera.front, m_scene->m_ActiveCamera.up)) * cameraSpeed;
    };

    /* Mouse Input */
    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Hide Cursor
        double x,y;
        glfwGetCursorPos(m_window, &x, &y);

        double offsetX = (x - lastX) * mouseSen;
        double offsetY = (y - lastY) * mouseSen;;
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

    };

    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
    {
        /* Return the Cursor*/
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    };
};  