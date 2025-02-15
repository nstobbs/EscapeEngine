#ifndef VULKAN_CONTEXT_H
#define VULKAN_CONTEXT_H

#include "../config.hpp"
#include "../entity/entity.hpp"
/*
vulkanContext.hpp
Struct that holds all of the vulkan related
objects for our applications to function. 
*/

// TODO: I don't like this name, it's not really data type but different uniform  
// TODO Shouldn't be here really
enum uniformLayout 
{
    sceneType,
    objectType
};

struct vulkanContext
{
    /* ############ General Vulkan ############ */
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

    /* Sync Object Related*/
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    uint32_t currentFrame = 0;
    bool frameBufferResized = false;

    /* ############ Mesh Component ############*/
    /* Vertex Buffer Related */
    uint32_t meshCount;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    /* ############ Shader Component ############ */
     /* Pipeline Related */
    uint32_t shaderCount;
    std::unordered_map<Entity, VkPipelineLayout> pipelineLayouts;
    std::unordered_map<Entity, VkPipeline> graphicsPiplines;
        
    /* Uniform Buffer Related*/
    uint32_t uniformBuffersCount = 0;
    std::unordered_map<uniformLayout, std::vector<VkBuffer>> uniformBuffers;
    std::unordered_map<uniformLayout, std::vector<VkDeviceMemory>> uniformBuffersMemory;
    std::unordered_map<uniformLayout, std::vector<void*>> uniformBufferMapped;

    /* Descriptor Sets Related */
    VkDescriptorPool descriptorPool;
    std::unordered_map<Entity, std::vector<VkDescriptorSetLayout>> descriptorSetLayoutsLists;
    
    //TODO this just doesn't seem like the best approach at all but ...
    std::unordered_map<Entity, std::unordered_map<uniformLayout, std::vector<VkDescriptorSet>>> descriptorSets; 
    // TextureArray
    VkDescriptorSetLayout textureDescriptorSetLayout;
    VkDescriptorSet textureDescriptorSet;

    /* ############ Texture Component ############*/
    /* Texture Related*/
    uint32_t textureCount;
    std::vector<VkImage> textureImages;
    std::vector<VkDeviceMemory> textureImageMemorys;
    std::vector<VkImageView> textureImageViews;
    VkSampler textureSampler; // We need more if we use different sampler settings. TODO ADD SETTINGS TO USE MORE THAN ONE

};

#endif