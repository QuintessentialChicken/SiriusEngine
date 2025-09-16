//
// Created by Lms on 19/07/2025.
//

#ifndef MATERIALSYSTEM_H
#define MATERIALSYSTEM_H
#include <array>
#include <cassert>
#include <DirectXMath.h>
#include <vec3.hpp>
#include <vec2.hpp>
#include <vector>

#include "Shader_Vulkan.h"
#include "Types_Vulkan.h"

struct ShaderEffect;
enum class TransparencyMode:uint8_t {
    Opaque,
    Transparent,
    Masked
};
struct VertexInputDescription {
    std::vector<VkVertexInputBindingDescription> bindings;
    std::vector<VkVertexInputAttributeDescription> attributes;

    VkPipelineVertexInputStateCreateFlags flags = 0;
};

struct Vertex {

    glm::vec3 position;
    DirectX::XMFLOAT3 color;
    glm::vec<2, uint8_t> oct_normal;//color;
    glm::vec2 uv;

    static VertexInputDescription get_vertex_description();
};

class PipelineBuilder {
public:
    std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;
	VertexInputDescription vertexDescription;
    VkPipelineVertexInputStateCreateInfo _vertexInputInfo;
    VkPipelineInputAssemblyStateCreateInfo _inputAssembly;
    VkViewport _viewport;
    VkRect2D _scissor;
    VkPipelineRasterizationStateCreateInfo _rasterizer;
    VkPipelineColorBlendAttachmentState _colorBlendAttachment;
    VkPipelineMultisampleStateCreateInfo _multisampling;
    VkPipelineLayout _pipelineLayout;
    VkPipelineDepthStencilStateCreateInfo _depthStencil;

    VkPipeline build_pipeline(VkDevice device, VkRenderPass pass);

    void clear_vertex_input();

    void setShaders(struct ShaderEffect *effect);

};

struct ShaderPass {
    ShaderEffect *effect{nullptr};
    VkPipeline pipeline{VK_NULL_HANDLE};
    VkPipelineLayout layout{VK_NULL_HANDLE};
};

struct ShaderParameters {
};

template<typename T>
struct PerPassData {
    T &operator[](MeshpassType pass) {
        switch (pass) {
            case MeshpassType::Forward:
                return data[0];
            case MeshpassType::Transparency:
                return data[1];
            case MeshpassType::DirectionalShadow:
                return data[2];
        }
        assert(false);
        return data[0];
    };

    void clear(T &&val) {
        for (int i = 0; i < 3; i++) {
            data[i] = val;
        }
    }

private:
    std::array<T, 3> data;
};

struct EffectTemplate {
    PerPassData<ShaderPass *> passShaders;

    ShaderParameters *defaultParameters;
    TransparencyMode transparency;
};

class MaterialSystem {
public:
    MaterialSystem(VkDevice device, VkRenderPass renderPass, ShaderCache& shaderCache);

    ShaderPass* build_shader(PipelineBuilder& builder, ShaderEffect* effect);


    void build_default_templates();

    void fill_builders();

private:
    PipelineBuilder forwardBuilder;
    PipelineBuilder shadowBuilder;

    VkDevice device;
    VkRenderPass renderPass{};
    ShaderCache& shaderCache;
};


#endif //MATERIALSYSTEM_H
