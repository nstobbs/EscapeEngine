#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#include "../config.cpp"

struct CameraComponent
{
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 position;
    float focalLength;
};

#endif