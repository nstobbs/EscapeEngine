#include "imguiInitialize.hpp"

void initializeImgui(vulkanContext &context, GLFWwindow *pWindow)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForVulkan(pWindow, true);
    ImGui_ImplVulkan_InitInfo imguiInitInfo{};
    imguiInitInfo.Instance = context.instance;
    imguiInitInfo.PhysicalDevice = context.physicalDevice;
    imguiInitInfo.Device = context.device;
    imguiInitInfo.QueueFamily = context.familyIndices.graphicsFamily.value();
    imguiInitInfo.Queue = context.graphicQueue;
    imguiInitInfo.DescriptorPool = context.descriptorPool;
    imguiInitInfo.RenderPass = context.renderPass;
    imguiInitInfo.Subpass = 0;
    imguiInitInfo.MinImageCount = MAX_FRAMES_IN_FLIGHT;
    imguiInitInfo.ImageCount = MAX_FRAMES_IN_FLIGHT;
    imguiInitInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    ImGui_ImplVulkan_Init(&imguiInitInfo);

    
    VkCommandBuffer commandBuffer = beginSingleTimeCommands(context);
    ImGui_ImplVulkan_CreateFontsTexture();
    endSingleTimeCommands(context, commandBuffer);
};