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

    std::random_device rd;
    std::mt19937 gen(rd());

    // Starting Prepping Vulkan for the Boids Sims
    for (auto sim : m_boidsSims)
    {
        BoidsComponent world = m_scene->m_BoidsComponents.at(sim);
        std::vector<Boid> boids;
        // 1. Generate Random Boid Data within Bounding Box
            // Position vec3
            // Direction vec3
        std::uniform_real_distribution<float> widthDistr(-world.width, world.width);
        std::uniform_real_distribution<float> heightDistr(-world.height, world.height);
        std::uniform_real_distribution<float> depthDistr(-world.depth, world.depth);
        for (int i = 0; i < world.boidsCount; i++)
        {
            Boid b;
            b.pos = glm::vec3(widthDistr(gen), heightDistr(gen), depthDistr(gen));
            b.dir = glm::vec3(widthDistr(gen), heightDistr(gen), depthDistr(gen));
            boids.push_back(b);
        };

        
        // 2. Create the Graphic Pipeline AND Compute Pipeline Layouts
        createBoidsDescriptorSetLayout(m_context);
        createBoidsComputePipeline(m_context);

        // 3. Upload This Data to the GPU
        createBoidsStorageBuffer(m_context, boids);
        createUniformBuffer(m_context, boidType, sizeof(BoidsSim));

        // TODO the graphic pipeline for rendering should of been created at this point
        
        // 4. Create the DescriptorSets for just the Boids Sims

    };
};