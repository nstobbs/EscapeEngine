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
        m_boidsSims = boidsSims;
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
        
        // 4. Create the DescriptorSets for just the Boids Sims
        createBoidsDescriptorSets(m_context, m_scene);

        // 5. Create Command Buffer and Sync Objects
        createBoidsSyncObjects(m_context);
        createBoidsCommandBuffers(m_context);
    };
};

void BoidsSystem::update()
{
    vkWaitForFences(m_context.device, 1, &m_context.boidsInFlightFences[m_context.currentFrame],
            VK_TRUE, UINT64_MAX);
            
    //TODO Just fix this lol, I know im about to write some hot trash here!!!
    BoidsComponent currentSim;
    for (auto ent : m_boidsSims)
    {
        currentSim = m_scene->m_BoidsComponents.at(ent);
    };

    updateBoidsUniformBuffer(m_context, currentSim);

    vkResetFences(m_context.device, 1, &m_context.boidsInFlightFences[m_context.currentFrame]);
    vkResetCommandBuffer(m_context.boidsCommandBuffer[m_context.currentFrame], 0);

    /* Start Command Buffer Recording */
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    auto result = vkBeginCommandBuffer(m_context.boidsCommandBuffer[m_context.currentFrame], &beginInfo);
    ASSERT_VK_RESULT(result, VK_SUCCESS, "Begin Boids Compute Command Buffer");

    vkCmdBindPipeline(m_context.boidsCommandBuffer[m_context.currentFrame], VK_PIPELINE_BIND_POINT_COMPUTE,
            m_context.boidsPipeline);
    
    // Storage Buffer In
    vkCmdBindDescriptorSets(m_context.boidsCommandBuffer[m_context.currentFrame], VK_PIPELINE_BIND_POINT_COMPUTE,
            m_context.boidsPipelineLayout, 0, 1, &m_context.boidsDescriptors[m_context.currentFrame], 0, nullptr);

    // Storage Buffer Out
    vkCmdBindDescriptorSets(m_context.boidsCommandBuffer[m_context.currentFrame], VK_PIPELINE_BIND_POINT_COMPUTE,
            m_context.boidsPipelineLayout, 1, 1, &m_context.boidsDescriptors[m_context.currentFrame + 2], 0, nullptr);

    // Uniform Buffer
    vkCmdBindDescriptorSets(m_context.boidsCommandBuffer[m_context.currentFrame], VK_PIPELINE_BIND_POINT_COMPUTE,
            m_context.boidsPipelineLayout, 2, 1, &m_context.boidsDescriptors[m_context.currentFrame + 4], 0, nullptr);


    auto workgroup_size = currentSim.boidsCount / 256;
    if (workgroup_size == 0)
    {
        workgroup_size = 1;
    }

    vkCmdDispatch(m_context.boidsCommandBuffer[m_context.currentFrame], workgroup_size, 1, 1);

    result = vkEndCommandBuffer(m_context.boidsCommandBuffer[m_context.currentFrame]);
    ASSERT_VK_RESULT(result, VK_SUCCESS, "Ending Boids Compute Command Buffer");
    /* End Command Buffer Recording */
    
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_context.boidsCommandBuffer[m_context.currentFrame];

    /* 
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = 
    */

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &m_context.boidsFinishedSemaphores[m_context.currentFrame]; 
    
    result = vkQueueSubmit(m_context.computeQueue, 1, &submitInfo, m_context.boidsInFlightFences[m_context.currentFrame]);
    ASSERT_VK_RESULT(result, VK_SUCCESS, "Submit Boids Compute Command Buffer");
};