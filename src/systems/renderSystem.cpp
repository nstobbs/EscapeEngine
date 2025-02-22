#include "renderSystem.hpp"

RenderSystem::RenderSystem(vulkanContext& context, Scene* scene, GLFWwindow* window) : m_context(context)
{
    m_scene = scene;
    m_window = window;
};

void RenderSystem::start()
{
    /* Check Every Entity Created
    And if they have a Mesh, Shader, Transform
    Components And Render that Image*/
    std::vector<Entity> renderableEntities;
    for (Entity ent : m_scene->m_Entities)
    {
        size_t resultMesh = m_scene->m_MeshComponents.count(ent);
        size_t resultShader = m_scene->m_ShaderComponents.count(ent);
        size_t resultTransform = m_scene->m_TransformComponents.count(ent);

        if (resultMesh != 0 && resultShader != 0 && resultTransform != 0)
        {
            /* IF a Entity has the required components then draw it*/
            renderableEntities.push_back(ent);
        };  
    };

    if (renderableEntities.size() != 0)
    {
        m_renderableEntities = renderableEntities;
    };
};

void RenderSystem::update()
{
    vkWaitForFences(m_context.device, 1, &m_context.inFlightFences[m_context.currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult resultResize = vkAcquireNextImageKHR(m_context.device, m_context.swapChain, UINT64_MAX,
                                            m_context.imageAvailableSemaphores[m_context.currentFrame],
                                            VK_NULL_HANDLE, &imageIndex);
    if (resultResize == VK_ERROR_OUT_OF_DATE_KHR)
    {
        recreateSwapChain(m_context, m_window);
        return;
    } else if (resultResize != VK_SUCCESS && resultResize != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("{ERROR} FAILED TO RE-CREATE SWAP CHAIN IMAGES");
    }

    vkResetFences(m_context.device, 1, &m_context.inFlightFences[m_context.currentFrame]);
    vkResetCommandBuffer(m_context.commandBuffers[m_context.currentFrame], 0);

    /*Record Command Buffer Start*/
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;

    auto result = vkBeginCommandBuffer(m_context.commandBuffers[m_context.currentFrame], &beginInfo); 
    ASSERT_VK_RESULT(result, VK_SUCCESS, "Begin Command Buffer");

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_context.renderPass;
    renderPassInfo.framebuffer = m_context.swapChainFramesBuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = m_context.swapChainExtent;

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{0.005f, 0.005f, 0.005f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(m_context.commandBuffers[m_context.currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkBuffer vertexBuffers[] = {m_context.vertexBuffer};
    VkDeviceSize offsets[] = {0};

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(m_context.swapChainExtent.width);
    viewport.height = static_cast<float>(m_context.swapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = m_context.swapChainExtent;

    /* For Each Entity We Bind the Graphics Pipeline Set from it shaderComponent */
    for (Entity ent : m_renderableEntities)
    {    
        ShaderComponent shader = m_scene->m_ShaderComponents.at(ent);
        /* Update the Uniform Buffer For the Object We are Currently Rendering*/
        TransformComponent modelTransforms = m_scene->m_TransformComponents.at(ent);
        updateUniformBuffer(m_context, m_scene, modelTransforms); 
        /* Get the ShaderID for this Entity */
        uint32_t shaderID = (m_scene->m_ShaderComponents.at(ent).ID);
        vkCmdBindPipeline(m_context.commandBuffers[m_context.currentFrame],
                         VK_PIPELINE_BIND_POINT_GRAPHICS, m_context.graphicsPiplines.at(ent));

        vkCmdBindVertexBuffers(m_context.commandBuffers[m_context.currentFrame],
                                 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(m_context.commandBuffers[m_context.currentFrame], 
                            m_context.indexBuffer, 0, VK_INDEX_TYPE_UINT32); 

        vkCmdSetViewport(m_context.commandBuffers[m_context.currentFrame], 
                         0, 1, &viewport);
        vkCmdSetScissor(m_context.commandBuffers[m_context.currentFrame], 0, 1, &scissor);

        vkCmdBindDescriptorSets(m_context.commandBuffers[m_context.currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_context.pipelineLayouts.at(ent), 0, 1,
                            &m_context.descriptorSets.at(ent).at(sceneType)[m_context.currentFrame], 0, nullptr);

        vkCmdBindDescriptorSets(m_context.commandBuffers[m_context.currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_context.pipelineLayouts.at(ent), 1, 1,
                            &m_context.descriptorSets.at(ent).at(objectType)[m_context.currentFrame], 0, nullptr);

        vkCmdBindDescriptorSets(m_context.commandBuffers[m_context.currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_context.pipelineLayouts.at(ent), 2, 1,
                            &m_context.textureDescriptorSet, 0, nullptr);

        uint32_t meshID = m_scene->m_MeshComponents.at(ent).ID; // Doesn't seem needed 
        uint32_t firstIndex = m_scene->m_MeshComponents.at(ent).details.firstIndex;
        uint32_t indicesCount = m_scene->m_MeshComponents.at(ent).indicesCount;

        /* TEMP ONLY SUPPORT ONE TEXTURE MATERIALS */
        /* Push Constant FoR Sending TextureIndex*/
        TextureIndexPush index;
        for (auto& texture : m_scene->m_TextureComponents.at(ent))
        {
            index.textureIndex = texture.ID - 1;
        };

        vkCmdPushConstants(m_context.commandBuffers[m_context.currentFrame], 
                           m_context.pipelineLayouts.at(ent),
                           VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_VERTEX_BIT,
                           0,
                           sizeof(TextureIndexPush),
                           &index);

        vkCmdDrawIndexed(m_context.commandBuffers[m_context.currentFrame], indicesCount, 1, firstIndex, 0, 0);
    };
    
    //TODO MOVE THIS TO A DIFFERENT FILE PLEASE!!!
    /* TEMP IMGUI RENDER CODE, TO MOVE INTO IT'S OWN SYSTEM LATER */
    bool showDemo = true;
    bool enableDebugMenu = true;

    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow(&showDemo);

    /* TEMP WINDOW FOR CAMERA DEBUGGING */
    ImGui::Begin("Debug Menu", &enableDebugMenu);
    ImGui::SeparatorText("CAMERA INFORMATION:");


    float x = m_scene->m_ActiveCamera.position.x;
    float y = m_scene->m_ActiveCamera.position.y;
    float z = m_scene->m_ActiveCamera.position.z;
    ImGui::Text("Position: %04f, %04f, %04f", x, y, z);

    x = m_scene->m_ActiveCamera.front.x;
    y = m_scene->m_ActiveCamera.front.y;
    z = m_scene->m_ActiveCamera.front.z;
    ImGui::Text("Front: %04f, %04f, %04f", x, y, z);

    x = m_scene->m_ActiveCamera.up.x;
    y = m_scene->m_ActiveCamera.up.y;
    z = m_scene->m_ActiveCamera.up.z;
    ImGui::Text("Up: %04f, %04f, %04f", x, y, z);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_context.commandBuffers[m_context.currentFrame], 0);

    vkCmdEndRenderPass(m_context.commandBuffers[m_context.currentFrame]);

    result =  vkEndCommandBuffer(m_context.commandBuffers[m_context.currentFrame]);
    ASSERT_VK_RESULT(result, VK_SUCCESS, "End Command Buffer Recording");
    /*Record Command Buffer End*/

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {m_context.imageAvailableSemaphores[m_context.currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_context.commandBuffers[m_context.currentFrame];

    VkSemaphore signalSemaphores[] = {m_context.renderFinishedSemaphores[m_context.currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;
    
    result = vkQueueSubmit(m_context.graphicQueue, 1, &submitInfo, m_context.inFlightFences[m_context.currentFrame]);
    ASSERT_VK_RESULT(result, VK_SUCCESS, "Submit Draw Command Buffer");

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {m_context.swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains= swapChains;
    presentInfo.pImageIndices = &imageIndex;

    presentInfo.pResults = nullptr;
    
    VkResult resultPresent = vkQueuePresentKHR(m_context.presentQueue, &presentInfo);
    if (resultPresent == VK_ERROR_OUT_OF_DATE_KHR || resultPresent == VK_SUBOPTIMAL_KHR || m_context.frameBufferResized)
    {
        m_context.frameBufferResized = false;
        recreateSwapChain(m_context, m_window);
    } else if (resultPresent != VK_SUCCESS)
    {
        throw std::runtime_error("{ERROR} FAILED TO CREATE SWAPCHAIN.");
    }

    m_context.currentFrame = (m_context.currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
};