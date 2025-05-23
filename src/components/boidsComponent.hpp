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

    float sim_separation = 0.025f;
    float sim_alignment = 0.5f;
    float sim_cohesion = 0.05f;

    float sim_protected_range = 0.4f;
    float sim_visible_range = 0.4f;
    float sim_turn_factor = 0.005f;
    float sim_max_speed = 0.3f;
    float sim_min_speed = 0.1f;
    float sim_update_rate = 0.025f;

    // Bounding Box 
    float height = 3.0f;
    float width = 3.0f;
    float depth = 3.0f;
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

    int sim_boidsCount;

    float deltaTime;

    float sim_protected_range;
    float sim_visible_range;
    float sim_turn_factor;
    float sim_max_speed;
    float sim_min_speed;
    float sim_update_rate;
};

#endif