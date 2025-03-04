#ifndef BOIDS_SYSTEM_H
#define BOIDS_SYSTEM_H

#include "../config.hpp"
#include "../controller/scene.hpp"

class BoidsSystem
{
    public:
        BoidsSystem(vulkanContext& context, Scene* scene);
        void start();
        void update();
    private:
        vulkanContext& m_context;
        Scene* m_scene;
        
        std::vector<Entity> m_boidsSims;
};

#endif