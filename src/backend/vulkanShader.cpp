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
    VkDescriptorSetLayoutBinding boidsBinding{};
    boidsBinding.binding = 0;
    boidsBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    boidsBinding.descriptorCount = 1;
    boidsBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT | 
                              VK_SHADER_STAGE_VERTEX_BIT |
                              VK_SHADER_STAGE_FRAGMENT_BIT;
    boidsBinding.pImmutableSamplers = nullptr;
    bindings.push_back(boidsBinding);

    // Boids Sim Parameters Binding
    VkDescriptorSetLayoutBinding simBinding{};
    simBinding.binding = 1;
    simBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    simBinding.descriptorCount = 1;
    simBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT |
                            VK_SHADER_STAGE_VERTEX_BIT |
                            VK_SHADER_STAGE_FRAGMENT_BIT;
    simBinding.pImmutableSamplers = nullptr;
    bindings.push_back(simBinding);

    VkDescriptorSetLayout layout;
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    auto result = vkCreateDescriptorSetLayout(context.device, &layoutInfo, nullptr, &layout);
    ASSERT_VK_RESULT(result, VK_SUCCESS, "Create Boids Descriptor Layout");
    context.boidsDescriptorsLayout = layout;
};

void createBoidsComputePipeline(vulkanContext& context)
{
    auto computeShaderCode = readShaderSourceFile("../../src/shaders/BoidsComp.spv"); // TODO add path
    VkShaderModule computeShaderModule = createShaderModule(computeShaderCode, context.device);

    VkPipelineShaderStageCreateInfo computeShaderStageInfo{};
    computeShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    computeShaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    computeShaderStageInfo.module = computeShaderModule;createBoidsDescriptorSetLayout;
    computeShaderStageInfo.pName = "main";

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &context.boidsDescriptorsLayout;

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