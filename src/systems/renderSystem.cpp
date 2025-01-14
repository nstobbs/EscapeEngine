#include "renderSystem.hpp"

RenderSystem::RenderSystem(vulkanContext& context, Scene& scene, GLFWwindow& window)
{
    m_context = context;
    m_scene = &scene;
    m_window = &window;
};

void RenderSystem::start()
{
    /* Check Every Entity Created
    And if they have a Mesh, Shader, Transform
    Components And Render that Image*/
    std::vector<Entity*> renderableEntities;
    for (Entity ent : m_scene->m_Entities)
    {
        uint32_t resultMesh = m_scene->m_MeshComponents.count(ent);
        uint32_t resultShader = m_scene->m_ShaderComponents.count(ent);
        uint32_t resultTransform = m_scene->m_TransformComponents.count(ent);

        if (resultMesh != 0 && resultShader != 0 && resultTransform != 0)
        {
            /* IF a Entity has the required components then draw it*/
            renderableEntities.push_back(&ent);
        };
        if (renderableEntities.size() != 0)
        {
            m_renderableEntities = renderableEntities;
        };  
    };
}

void RenderSystem::update()
{
    vkWaitForFences(m_context.device, 1, &m_context.inFlightFences[m_context.currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(m_context.device, m_context.swapChain, UINT64_MAX,
                                            m_context.imageAvailableSemaphores[m_context.currentFrame],
                                            VK_NULL_HANDLE, &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        recreateSwapChain(m_context, m_window);
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("{ERROR} FAILED TO RE-CREATE SWAP CHAIN IMAGES");
    }

    vkResetFences(m_context.device, 1, &m_context.inFlightFences[m_context.currentFrame]);
    vkResetCommandBuffer(m_context.commandBuffers[m_context.currentFrame], 0);

    recordCommandBuffer(m_context.commandBuffers[m_context.currentFrame], imageIndex);

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

    if (vkQueueSubmit(m_context.graphicQueue, 1, &submitInfo,
                      m_context.inFlightFences[m_context.currentFrame]) != VK_SUCCESS)
    {
        std::runtime_error("{ERROR} FAILED TO SUBMIT DRAW COMMAND BUFFER.");
    };




};

void RenderSystem::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("{ERROR} FAILED TO BEGIN COMMAND BUFFER.");
    };

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_context.renderPass;
    renderPassInfo.framebuffer = m_context.swapChainFramesBuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = m_context.swapChainExtent;

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{0.1, 0.1f, 0.1f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

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
    for (auto& ent : m_renderableEntities)
    {

        /* Get the ShaderID for this Entity */
        uint32_t shaderID = m_scene->m_ShaderComponents.at((Entity)ent).ID;
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_context.graphicsPiplines[shaderID]);

        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, m_context.indexBuffer, 0, VK_INDEX_TYPE_UINT32); 

        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_context.pipelineLayout, 0, 0,
                            &m_context.descriptorSets[m_context.currentFrame], 0, nullptr);
        
        uint32_t meshID = m_scene->m_MeshComponents.at((Entity)ent).ID;
        uint32_t firstIndex = m_scene->m_MeshComponents.at((Entity)ent).details.firstIndex;
        uint32_t indicesCount = m_scene->m_MeshComponents.at((Entity)ent).indicesCount;
        vkCmdDrawIndexed(commandBuffer, indicesCount, 1, firstIndex, 0, 0);
    };

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("{ERROR} FAILED TO RECORD COMMAND BUFFER.");
    };
    VkSemaphore signalSemaphores[] = {m_context.renderFinishedSemaphores[m_context.currentFrame]};

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {m_context.swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains= swapChains;
    presentInfo.pImageIndices = &imageIndex;

    presentInfo.pResults = nullptr;
    
    VkResult result = vkQueuePresentKHR(m_context.presentQueue, &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_context.frameBufferResized)
    {
        m_context.frameBufferResized = false;
        recreateSwapChain(m_context, m_window);
    } else if (result != VK_SUCCESS)
    {
        throw std::runtime_error("{ERROR} FAILED TO CREATE SWAPCHAIN.");
    }

    m_context.currentFrame = (m_context.currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
};