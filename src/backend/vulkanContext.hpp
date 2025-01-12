#ifndef VULKAN_CONTEXT_H
#define VULKAN_CONTEXT_H

#include "../config.hpp"

/*
vulkanContext.hpp
Struct that holds all of the vulkan related
objects for our applications to function. 
*/

struct vulkanContext
{
    /* Devices Related */
    VkDevice device;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkInstance instance;

    /* Debug */
    VkDebugUtilsMessengerEXT debugMessenger;

    /* Surface Related*/
    VkSurfaceKHR surface;

    /* Commands Related */
    VkQueue graphicQueue;
    VkQueue presentQueue;
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;

    /* Swap Chain Related */
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramesBuffers;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;

    /* Render Pass Related */
    VkRenderPass renderPass;

    /* Depth Testing Related */
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    /* Pipeline Related */
    VkPipelineLayout pipelineLayout; /* Seems like we only needs one piplineLayout for now ??*/
    uint32_t shaderCount;
    std::vector<VkPipeline> graphicsPiplines;

    /* Sync Object Related*/
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    /* Vertex Buffer Related */
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory IndexBufferMemory;

    /* Uniform Buffer Related*/
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBufferMapped;
    VkDescriptorPool descriptorPool;
    VkDescriptorSetLayout descriptorSetLayout;
    std::vector<VkDescriptorSet> descriptorSets;

    uint32_t currentFrame = 0;
    bool frameBufferResized = false;
};

#endif