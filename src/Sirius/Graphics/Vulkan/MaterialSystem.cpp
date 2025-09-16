//
// Created by Lms on 19/07/2025.
//

#include "MaterialSystem.h"

#include <stdexcept>

#include "Shader_Vulkan.h"

VkPipeline PipelineBuilder::build_pipeline(VkDevice device, VkRenderPass pass) {
    _vertexInputInfo = VkPipelineVertexInputStateCreateInfo{};
    _vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    _vertexInputInfo.pNext = nullptr;

    //no vertex bindings or attributes
    _vertexInputInfo.vertexBindingDescriptionCount = 0;
    _vertexInputInfo.vertexAttributeDescriptionCount = 0;
    //connect the pipeline builder vertex input info to the one we get from Vertex
    _vertexInputInfo.pVertexAttributeDescriptions = vertexDescription.attributes.data();
    _vertexInputInfo.vertexAttributeDescriptionCount = (uint32_t)vertexDescription.attributes.size();

    _vertexInputInfo.pVertexBindingDescriptions = vertexDescription.bindings.data();
    _vertexInputInfo.vertexBindingDescriptionCount = (uint32_t)vertexDescription.bindings.size();
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

VertexInputDescription Vertex::get_vertex_description() {
    VertexInputDescription description;

    //we will have just 1 vertex buffer binding, with a per-vertex rate
    VkVertexInputBindingDescription mainBinding = {};
    mainBinding.binding = 0;
    mainBinding.stride = sizeof(Vertex);
    mainBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    description.bindings.push_back(mainBinding);

    //Position will be stored at Location 0
    VkVertexInputAttributeDescription positionAttribute = {};
    positionAttribute.binding = 0;
    positionAttribute.location = 0;
    positionAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    positionAttribute.offset = offsetof(Vertex, position);

    //Normal will be stored at Location 1
    VkVertexInputAttributeDescription normalAttribute = {};
    normalAttribute.binding = 0;
    normalAttribute.location = 1;
    normalAttribute.format = VK_FORMAT_R8G8_UNORM;//VK_FORMAT_R32G32B32_SFLOAT;
    normalAttribute.offset = offsetof(Vertex, oct_normal);

    //Position will be stored at Location 2
    VkVertexInputAttributeDescription colorAttribute = {};
    colorAttribute.binding = 0;
    colorAttribute.location = 2;
    colorAttribute.format = VK_FORMAT_R8G8B8_UNORM;//VK_FORMAT_R32G32B32_SFLOAT;
    colorAttribute.offset = offsetof(Vertex, color);

    //UV will be stored at Location 2
    VkVertexInputAttributeDescription uvAttribute = {};
    uvAttribute.binding = 0;
    uvAttribute.location = 3;
    uvAttribute.format = VK_FORMAT_R32G32_SFLOAT;
    uvAttribute.offset = offsetof(Vertex, uv);


    description.attributes.push_back(positionAttribute);
    description.attributes.push_back(normalAttribute);
    description.attributes.push_back(colorAttribute);
    description.attributes.push_back(uvAttribute);
    return description;
}

ShaderEffect* build_effect(std::string_view vertexShader, std::string_view fragmentShader, VkDevice device, ShaderCache& shaderCache) {
    ShaderEffect::ReflectionOverrides overrides[] = {
        {"sceneData", VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC},
        {"cameraData", VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC}
    };
    //textured defaultlit shader
    auto* effect = new ShaderEffect();

    effect->add_stage(shaderCache.get_shader(vertexShader.data()), VK_SHADER_STAGE_VERTEX_BIT);

    if (fragmentShader.size() > 2) {
        effect->add_stage(shaderCache.get_shader(fragmentShader.data()), VK_SHADER_STAGE_FRAGMENT_BIT);
    }

    effect->reflect_layout(device, overrides, 2);

    return effect;
}


MaterialSystem::MaterialSystem(VkDevice device, VkRenderPass renderPass, ShaderCache& shaderCache) : device{device}, renderPass{renderPass}, shaderCache{shaderCache} {
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

    ShaderEffect* defaultLit = build_effect("../../Sirius/Shaders/tri_mesh_ssbo_instanced.vert.spv", "../../Sirius/Shaders/default_lit.frag.spv", device, shaderCache);
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
    forwardBuilder.vertexDescription = Vertex::get_vertex_description();
    forwardBuilder._inputAssembly = inputAssemblyStateInfo;

    forwardBuilder._rasterizer = rasterizer;
    forwardBuilder._rasterizer.cullMode = VK_CULL_MODE_NONE; //BACK_BIT;

    forwardBuilder._multisampling = multisampling;

    forwardBuilder._colorBlendAttachment = colorBlendAttachment;

    //default depthtesting
    forwardBuilder._depthStencil = stencilInfo;
    forwardBuilder._depthStencil.depthCompareOp = VK_COMPARE_OP_GREATER_OR_EQUAL;
}
