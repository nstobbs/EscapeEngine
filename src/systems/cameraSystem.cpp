#include "cameraSystem.hpp"

CameraSystem::CameraSystem(Scene* scene, GLFWwindow* window)
{
    m_scene = scene;
    m_window = window;
};

void CameraSystem::start()
{
    yaw = -90.0f;
    pitch = 0.0f;

    // Center Camera with Mouse ????
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    lastX = static_cast<double>(width) / 2.0;
    lastY = static_cast<double>(height) / 2.0;

    m_scene->m_ActiveCamera.pitch = pitch;
    m_scene->m_ActiveCamera.yaw = yaw;
};

void CameraSystem::update(float delta)
{   
    /* KeyBoard Input */
    /* Forward */
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_TRUE)
    {
        m_scene->m_ActiveCamera.position += (m_scene->m_ActiveCamera.direction * cameraSpeed);
    };

    /* Left */
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_TRUE)
    {
        m_scene->m_ActiveCamera.position -= glm::normalize(glm::cross(m_scene->m_ActiveCamera.direction, m_scene->m_ActiveCamera.up)) * cameraSpeed;
    };

    /* Backward */
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_TRUE)
    {
        m_scene->m_ActiveCamera.position -= (m_scene->m_ActiveCamera.direction * cameraSpeed);
    };

    /* Right */
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_TRUE)
    {
        m_scene->m_ActiveCamera.position += glm::normalize(glm::cross(m_scene->m_ActiveCamera.direction, m_scene->m_ActiveCamera.up)) * cameraSpeed;
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

        if (pitch > 89.0f)
        {
            pitch = 89.0f;
        }
        if (pitch < -89.0f)
        {
            pitch = -89.0f;
        }

        if (yaw > 359.0f)
        {
            yaw = 1.0f;
        }
        if (yaw < 0.0f)
        {
            yaw = 359.0f;
        }

        glm::vec3 direction;
        
        direction.x = cos(glm::radians(static_cast<float>(pitch))) * cos(glm::radians(static_cast<float>(yaw)));
        direction.y = sin(glm::radians(static_cast<float>(yaw)));
        direction.z = sin(glm::radians(static_cast<float>(pitch))) * cos(glm::radians(static_cast<float>(yaw)));
        m_scene->m_ActiveCamera.direction = glm::normalize(direction);
        m_scene->m_ActiveCamera.pitch = pitch;
        m_scene->m_ActiveCamera.yaw = yaw;
    };

    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
    {
        /* Return the Cursor*/
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    };

    /* Focal Lens Controls */
    if(glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_TRUE)
    {
        m_scene->m_ActiveCamera.focalLength += lensSpeed;
    };

    if(glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_TRUE)
    {
        m_scene->m_ActiveCamera.focalLength -= lensSpeed;
    };
};  