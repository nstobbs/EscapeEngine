#ifndef BOIDS_COMPONENT_H
#define BOIDS_COMPONENT_H

#include "../config.cpp"

/*
BoidsComponent.hpp

Component for storing of all of
the world parameters for the boidSystem
to use later.

*/

struct BoidsComponent
{
    // Sim Parameter
    uint16_t boidsCount;

    float sim_separation = 0.5f;
    float sim_alignment = 0.5f;
    float sim_cohesion = 0.5f;

    // Bounding Box 
    float height = 1.0f;
    float width = 1.5f;
    float depth = 1.0f;
};

struct Boid
{
    glm::vec3 pos;
    glm::vec3 dir;
};

//TODO Could Maybe Do this a better but fuck it

struct BoidsSim
{
    float sim_separation;
    float sim_alignment;
    float sim_cohesion;
    float height;
    float width;
    float depth;
};

#endif