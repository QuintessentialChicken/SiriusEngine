// PipelineState.h
#ifndef PIPELINE_STATE_H
#define PIPELINE_STATE_H

#include <array>
#include <DirectXMath.h>

#include "InputLayout.h"

class Shader;

// Primitive topology
enum class PrimitiveTopology {
    PointList,
    LineList,
    LineStrip,
    TriangleList,
    TriangleStrip
};

// Pipeline state description
struct PipelineStateDesc {
    PrimitiveTopology topology = PrimitiveTopology::TriangleList;
    IInputLayout *inputLayout = nullptr;
    Shader *vertexShader = nullptr;
    Shader *pixelShader = nullptr;
    Shader *geometryShader = nullptr;
    Shader *hullShader = nullptr;
    Shader *domainShader = nullptr;
};

class PipelineState {
    PipelineState(PipelineStateDesc desc, VkDescriptorSetLayout descriptorSetLayout, VkRenderPass renderPass, VkDevice device);

    VkPipeline GetPipeline();

    // Temporary to give RenderApi_Vulkan access and not break the other parts of rendering
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;

private:
    VkPipeline graphicsPipeline = VK_NULL_HANDLE;
    VkDevice device;

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


#endif // PIPELINE_STATE_H
