//
// Created by Lms on 19/07/2025.
//

#include "MaterialSystem.h"

#include <stdexcept>

#include "Shader_Vulkan.h"

VkPipeline PipelineBuilder::build_pipeline(VkDevice device, VkRenderPass pass) {
    auto attributeDescription = Vertex::getAttributeDescriptions();
    auto bindingDescription = Vertex::getBindingDescription();
    _vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    _vertexInputInfo.pNext = nullptr;

    //no vertex bindings or attributes
    _vertexInputInfo.vertexBindingDescriptionCount = 0;
    _vertexInputInfo.vertexAttributeDescriptionCount = 0;
    //connect the pipeline builder vertex input info to the one we get from Vertex
    _vertexInputInfo.vertexAttributeDescriptionCount = 1;
    _vertexInputInfo.pVertexAttributeDescriptions = attributeDescription.data();

    _vertexInputInfo.vertexBindingDescriptionCount = 1;
    _vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    _vertexInputInfo.flags = 0;


    //make viewport state from our stored viewport and scissor.
    //at the moment we wont support multiple viewports or scissors
    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.pNext = nullptr;

    viewportState.viewportCount = 1;
    viewportState.pViewports = &_viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &_scissor;

    //setup dummy color blending. We arent using transparent objects yet
    //the blending is just "no blend", but we do write to the color attachment
    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.pNext = nullptr;

    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &_colorBlendAttachment;

    //build the actual pipeline
    //we now use all of the info structs we have been writing into into this one to create the pipeline
    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = nullptr;

    pipelineInfo.stageCount = (uint32_t) _shaderStages.size();
    pipelineInfo.pStages = _shaderStages.data();
    pipelineInfo.pVertexInputState = &_vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &_inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &_rasterizer;
    pipelineInfo.pMultisampleState = &_multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDepthStencilState = &_depthStencil;
    pipelineInfo.layout = _pipelineLayout;
    pipelineInfo.renderPass = pass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;


    std::vector<VkDynamicState> dynamicStates;
    dynamicStates.push_back(VK_DYNAMIC_STATE_VIEWPORT);
    dynamicStates.push_back(VK_DYNAMIC_STATE_SCISSOR);
    dynamicStates.push_back(VK_DYNAMIC_STATE_DEPTH_BIAS);
    dynamicState.pDynamicStates = dynamicStates.data();
    dynamicState.dynamicStateCount = (uint32_t) dynamicStates.size();

    pipelineInfo.pDynamicState = &dynamicState;

    //its easy to error out on create graphics pipeline, so we handle it a bit better than the common VK_CHECK case
    VkPipeline newPipeline;
    if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &newPipeline) != VK_SUCCESS) {
        throw std::runtime_error("Failed to build graphics pipeline");
    } else {
        return newPipeline;
    }
}

void PipelineBuilder::clear_vertex_input() {
    _vertexInputInfo.pVertexAttributeDescriptions = nullptr;
    _vertexInputInfo.vertexAttributeDescriptionCount = 0;

    _vertexInputInfo.pVertexBindingDescriptions = nullptr;
    _vertexInputInfo.vertexBindingDescriptionCount = 0;
}

void PipelineBuilder::setShaders(ShaderEffect* effect) {
    _shaderStages.clear();
    effect->fill_stages(_shaderStages);

    _pipelineLayout = effect->builtLayout;
}

ShaderEffect* build_effect(std::string_view vertexShader, std::string_view fragmentShader, VkDevice device) {
    ShaderEffect::ReflectionOverrides overrides[] = {
        {"sceneData", VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC},
        {"cameraData", VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC}
    };
    //textured defaultlit shader
    auto* effect = new ShaderEffect();
    ShaderModule vertexModule{Shader_Vulkan::CreateShaderModule(Shader_Vulkan::ReadFile(vertexShader.data()), device)};

    effect->add_stage(&vertexModule, VK_SHADER_STAGE_VERTEX_BIT);

    if (fragmentShader.size() > 2) {
        ShaderModule fragModule{Shader_Vulkan::CreateShaderModule(Shader_Vulkan::ReadFile(fragmentShader.data()), device)};
        effect->add_stage(&fragModule, VK_SHADER_STAGE_FRAGMENT_BIT);
    }

    // TODO Implement (or replace) shader reflection
    effect->reflect_layout(device, overrides, 2);

    return effect;
}


MaterialSystem::MaterialSystem(VkDevice device, VkRenderPass renderPass) : device{device}, renderPass{renderPass} {
    build_default_templates();
}

ShaderPass* MaterialSystem::build_shader(PipelineBuilder& builder, ShaderEffect* effect) {
    auto* pass = new ShaderPass();

    pass->effect = effect;
    pass->layout = effect->builtLayout;

    PipelineBuilder pipbuilder = builder;

    pipbuilder.setShaders(effect);

    pass->pipeline = pipbuilder.build_pipeline(device, renderPass);

    return pass;
}

void MaterialSystem::build_default_templates() {
    fill_builders();

    ShaderEffect* defaultLit = build_effect("tri_mesh_ssbo_instanced.vert.spv", "default_lit.frag.spv", device);
    ShaderPass* defaultLitPass = build_shader(forwardBuilder, defaultLit);


    EffectTemplate defaultColored;

    defaultColored.passShaders[MeshpassType::Transparency] = nullptr;
    defaultColored.passShaders[MeshpassType::DirectionalShadow] = nullptr;
    defaultColored.passShaders[MeshpassType::Forward] = defaultLitPass;
    defaultColored.defaultParameters = nullptr;
    defaultColored.transparency = TransparencyMode::Opaque;
}

void MaterialSystem::fill_builders() {
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateInfo = {};
    inputAssemblyStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyStateInfo.pNext = nullptr;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    //default depthtesting
    VkPipelineDepthStencilStateCreateInfo stencilInfo = {};
    stencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    stencilInfo.pNext = nullptr;
    stencilInfo.depthTestEnable = VK_TRUE;
    stencilInfo.depthWriteEnable = VK_TRUE;
    stencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    stencilInfo.depthBoundsTestEnable = VK_FALSE;
    stencilInfo.stencilTestEnable = VK_FALSE;

    inputAssemblyStateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyStateInfo.primitiveRestartEnable = VK_FALSE;
    shadowBuilder._inputAssembly = inputAssemblyStateInfo;

    shadowBuilder._rasterizer.cullMode = VK_CULL_MODE_FRONT_BIT;
    shadowBuilder._rasterizer.depthBiasEnable = VK_TRUE;
    shadowBuilder._rasterizer = rasterizer;

    shadowBuilder._multisampling = multisampling;
    shadowBuilder._colorBlendAttachment = colorBlendAttachment;


    shadowBuilder._depthStencil = stencilInfo;

    forwardBuilder._inputAssembly = inputAssemblyStateInfo;


    forwardBuilder._rasterizer = rasterizer;
    forwardBuilder._rasterizer.cullMode = VK_CULL_MODE_NONE; //BACK_BIT;

    forwardBuilder._multisampling = multisampling;

    forwardBuilder._colorBlendAttachment = colorBlendAttachment;

    //default depthtesting
    forwardBuilder._depthStencil = stencilInfo;
    forwardBuilder._depthStencil.depthCompareOp = VK_COMPARE_OP_GREATER_OR_EQUAL;
}
