#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#include "../config.cpp"

struct CameraComponent
{
    glm::vec3 direction;
    glm::vec3 position;
    glm::vec3 up = glm::vec3(0.0f, 0.0f, -1.0f); // -1 is upwards in vulkan
    float focalLength;

    //todo temp
    double pitch;
    double yaw;
};

#endif