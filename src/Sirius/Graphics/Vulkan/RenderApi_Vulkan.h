//
// Created by Leon on 17/05/2025.
//

#ifndef RENDERAPI_VULKAN_H
#define RENDERAPI_VULKAN_H
#include <optional>

#include "Graphics/RenderApi.h"
#include <vulkan/vulkan.h>


class RenderApi_Vulkan : public IRenderApi {
public:
    void Init() override;

    void BeginFrame() override;

    void ResizeViewport(int width, int height) override;

    std::unique_ptr<IShader> CreateShader(ShaderType type, const std::wstring& path) override;

    std::unique_ptr<IInputLayout> CreateInputLayout(const std::vector<InputLayoutElement>& elements, const void* shaderBytecode, size_t bytecodeSize) override;

    std::unique_ptr<IPipelineState> CreatePipelineState(const PipelineStateDesc& desc) override;

    std::unique_ptr<IVertexBuffer> CreateVertexBuffer(const void* data, size_t size, UINT stride) override;

    std::unique_ptr<IIndexBuffer> CreateIndexBuffer(const void* indices, size_t size) override;

    std::unique_ptr<IConstantBuffer> CreateConstantBuffer(const void* data, size_t size) override;

    [[nodiscard]] DirectX::XMMATRIX GetProjection() const noexcept override;

    void EndFrame() override;

    void DrawIndexed(UINT count) override;

    void Shutdown() override;

private:
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;

        bool isComplete() {
            return graphicsFamily.has_value();
        }
    };

    void CreateInstance();

    bool CheckValidationLayerSupport();

    void PickPhysicalDevice();

    bool IsDeviceSuitable(VkPhysicalDevice device);

    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

    void CreateLogicalDevice();

    VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VkQueue graphicsQueue = VK_NULL_HANDLE;
};


#endif //RENDERAPI_VULKAN_H
