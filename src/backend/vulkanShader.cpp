#include "vulkanShader.hpp"

std::vector<char> readShaderSourceFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("{ERROR} FAILED TO READ SHADER FILE");
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
};

VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice device)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
    {
        throw std::runtime_error("{ERROR} FAILED TO CREATE SHADER MODULE.");
    }

    return shaderModule;
};

void createBoidsDescriptorSetLayout(vulkanContext& context)
{
    std::vector<VkDescriptorSetLayoutBinding> bindings;

    // Raw Boids Data Binding
    VkDescriptorSetLayoutBinding boidsInBinding{};
    boidsInBinding.binding = 0;
    boidsInBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    boidsInBinding.descriptorCount = 1;
    boidsInBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT | 
                              VK_SHADER_STAGE_VERTEX_BIT |
                              VK_SHADER_STAGE_FRAGMENT_BIT;
    boidsInBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayout boidsInlayout;
    VkDescriptorSetLayoutCreateInfo boidsInlayoutInfo{};
    boidsInlayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    boidsInlayoutInfo.bindingCount = 1;
    boidsInlayoutInfo.pBindings = &boidsInBinding;

    auto result = vkCreateDescriptorSetLayout(context.device, &boidsInlayoutInfo, nullptr, &boidsInlayout);
    ASSERT_VK_RESULT(result, VK_SUCCESS, "Create Boids Descriptor boidsInlayout");
    context.boidsDescriptorsLayouts.push_back(boidsInlayout);

    VkDescriptorSetLayoutBinding boidsOutBinding{};
    boidsOutBinding.binding = 0;
    boidsOutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    boidsOutBinding.descriptorCount = 1;
    boidsOutBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT | 
                              VK_SHADER_STAGE_VERTEX_BIT |
                              VK_SHADER_STAGE_FRAGMENT_BIT;
    boidsOutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayout boidsOutlayout;
    VkDescriptorSetLayoutCreateInfo boidsOutlayoutInfo{};
    boidsOutlayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    boidsOutlayoutInfo.bindingCount = 1;
    boidsOutlayoutInfo.pBindings = &boidsOutBinding;

    result = vkCreateDescriptorSetLayout(context.device, &boidsOutlayoutInfo, nullptr, &boidsOutlayout);
    ASSERT_VK_RESULT(result, VK_SUCCESS, "Create Boids Descriptor boidsInlayout");
    context.boidsDescriptorsLayouts.push_back(boidsOutlayout);

    // Boids Sim Parameters Binding
    VkDescriptorSetLayoutBinding simBinding{};
    simBinding.binding = 0;
    simBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    simBinding.descriptorCount = 1;
    simBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT |
                            VK_SHADER_STAGE_VERTEX_BIT |
                            VK_SHADER_STAGE_FRAGMENT_BIT;
    simBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayout boidsSimLayout;
    VkDescriptorSetLayoutCreateInfo boidsSimLayoutInfo{};
    boidsSimLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    boidsSimLayoutInfo.bindingCount = 1;
    boidsSimLayoutInfo.pBindings = &simBinding;

    result = vkCreateDescriptorSetLayout(context.device, &boidsSimLayoutInfo, nullptr, &boidsSimLayout);
    ASSERT_VK_RESULT(result, VK_SUCCESS, "Create Boids Descriptor boidsInlayout");
    context.boidsDescriptorsLayouts.push_back(boidsSimLayout);
};

void createBoidsComputePipeline(vulkanContext& context)
{
    auto computeShaderCode = readShaderSourceFile("../../src/shaders/BoidsComp.spv");
    VkShaderModule computeShaderModule = createShaderModule(computeShaderCode, context.device);

    VkPipelineShaderStageCreateInfo computeShaderStageInfo{};
    computeShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    computeShaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    computeShaderStageInfo.module = computeShaderModule;
    computeShaderStageInfo.pName = "main";

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(context.boidsDescriptorsLayouts.size());
    pipelineLayoutInfo.pSetLayouts = context.boidsDescriptorsLayouts.data();

    VkPipelineLayout pipelineLayout;
    auto result = vkCreatePipelineLayout(context.device, &pipelineLayoutInfo, nullptr, &pipelineLayout);
    ASSERT_VK_RESULT(result, VK_SUCCESS, "Create Boids Pipeline Layout");
    context.boidsPipelineLayout = pipelineLayout;

    VkComputePipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.layout = context.boidsPipelineLayout;
    pipelineInfo.stage = computeShaderStageInfo;

    VkPipeline pipeline;
    result = vkCreateComputePipelines(context.device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline);
    ASSERT_VK_RESULT(result, VK_SUCCESS, "Create Compute Pipeline");
    context.boidsPipeline = pipeline;
};