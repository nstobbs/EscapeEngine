#include "boidsSystem.hpp"

BoidsSystem::BoidsSystem(vulkanContext& context, Scene* scene) : m_context(context)
{
    m_context = context;
    m_scene =  scene;
};

void BoidsSystem::start()
{   
    std::vector<Entity> boidsSims;
    for (Entity ent : m_scene->m_Entities)
    {
        size_t resultBoids = m_scene->m_BoidsComponents.count(ent);
        size_t resultMesh = m_scene->m_MeshComponents.count(ent);
        size_t resultShader = m_scene->m_ShaderComponents.count(ent);
        size_t resultTransform = m_scene->m_TransformComponents.count(ent);
        

        if (resultMesh != 0 && resultBoids != 0 && resultTransform != 0 && resultShader != 0)
        {
            if (m_scene->m_BoidsComponents.at(ent).boidsCount != 0)
            {
                boidsSims.push_back(ent);
            }
        };  
    };

    if (boidsSims.size() != 0)
    {
        boidsSims = m_boidsSims;
    };

    // Starting Prepping Vulkan for the Boids Sims
    for (auto sim : m_boidsSims)
    {
        // 1. Generate Random Boid Data within Bounding Box
            // Position vec3
            // Direction vec3
        // 2. Upload This Data to the GPU 
        // 3. Create the Graphic Pipeline AND Compute Pipeline Layouts
        // 4. Create the Pipelines

    };
};