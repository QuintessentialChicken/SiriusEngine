//
// Created by Lms on 19/07/2025.
//

#ifndef MATERIALSYSTEM_H
#define MATERIALSYSTEM_H
#include <array>
#include <cassert>
#include <DirectXMath.h>
#include <vector>

#include "Types_Vulkan.h"

struct ShaderEffect;
enum class TransparencyMode:uint8_t {
    Opaque,
    Transparent,
    Masked
};

class PipelineBuilder {
public:
    std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;
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

private:
    struct Vertex {
        DirectX::XMFLOAT2 position;
        DirectX::XMFLOAT3 color;

        static VkVertexInputBindingDescription getBindingDescription() {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

            // Fill in the description for the position
            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, position);

            // Fill in the description for the color
            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, color);
            return attributeDescriptions;
        }
    };
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
    MaterialSystem(VkDevice device, VkRenderPass renderPass);

    ShaderPass* build_shader(PipelineBuilder& builder, ShaderEffect* effect);


    void build_default_templates();

    void fill_builders();

private:
    PipelineBuilder forwardBuilder;
    PipelineBuilder shadowBuilder;

    VkDevice device;
    VkRenderPass renderPass{};
};


#endif //MATERIALSYSTEM_H
