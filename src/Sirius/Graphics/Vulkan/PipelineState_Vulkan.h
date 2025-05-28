//
// Created by Leon on 28/05/2025.
//

#ifndef PIPELINESTATE_VULKAN_H
#define PIPELINESTATE_VULKAN_H
#include <array>
#include <DirectXMath.h>
#include <vulkan/vulkan_core.h>

#include "Graphics/PipelineState.h"


class PipelineState_Vulkan : public IPipelineState {
public:
    PipelineState_Vulkan() = default;
    void Bind() override;

private:
    VkPipeline graphicsPipeline;
    VkPipelineLayout pipelineLayout;
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

inline void PipelineState_Vulkan::Bind() {
}


#endif //PIPELINESTATE_VULKAN_H
