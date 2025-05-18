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

    void Draw() override;

    void DrawIndexed(UINT count) override;

    void Shutdown() override;

private:
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;


        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    void CreateInstance();

    void CreateSurface();

    bool CheckValidationLayerSupport();

    void PickPhysicalDevice();

    bool IsDeviceSuitable(VkPhysicalDevice device);

    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

    void CreateLogicalDevice();

    bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    void CreateSwapChain();

    void CreateImageViews();

    void CreateGraphicsPipeline();

    VkShaderModule CreateShaderModule(const std::vector<char>& code);

    void CreateRenderPass();

    void CreateFramebuffers();

    void CreateCommandPool();

    void CreateCommandBuffer();

    void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    void CreateSyncObjects();


    static std::vector<char> ReadFile(const std::string& filename);

    VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VkQueue graphicsQueue = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkQueue presentQueue = VK_NULL_HANDLE;
    VkSwapchainKHR swapChain = VK_NULL_HANDLE;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat = {};
    VkExtent2D swapChainExtent = {};
    std::vector<VkImageView> swapChainImageViews;
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    VkRenderPass renderPass = VK_NULL_HANDLE;
    VkPipeline graphicsPipeline = VK_NULL_HANDLE;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    VkCommandPool commandPool = VK_NULL_HANDLE;
    VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
    VkSemaphore imageAvailableSemaphore = VK_NULL_HANDLE;
    VkSemaphore renderFinishedSemaphore = VK_NULL_HANDLE;
    VkFence inFlightFence = VK_NULL_HANDLE;


};


#endif //RENDERAPI_VULKAN_H
