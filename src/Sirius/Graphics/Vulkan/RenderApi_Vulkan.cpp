//
// Created by Leon on 17/05/2025.
//

#include "RenderApi_Vulkan.h"

#include <iostream>
#include <optional>
#include <stdexcept>
#include <vulkan/vulkan_win32.h>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

void RenderApi_Vulkan::Init() {
    CreateInstance();
    PickPhysicalDevice();
    CreateLogicalDevice();
}


void RenderApi_Vulkan::BeginFrame() {
}

void RenderApi_Vulkan::ResizeViewport(int width, int height) {
}

std::unique_ptr<IShader> RenderApi_Vulkan::CreateShader(ShaderType type, const std::wstring& path) {
    return nullptr;
}

std::unique_ptr<IInputLayout> RenderApi_Vulkan::CreateInputLayout(const std::vector<InputLayoutElement>& elements, const void* shaderBytecode, size_t bytecodeSize) {
    return nullptr;
}

std::unique_ptr<IPipelineState> RenderApi_Vulkan::CreatePipelineState(const PipelineStateDesc& desc) {
    return nullptr;
}

std::unique_ptr<IVertexBuffer> RenderApi_Vulkan::CreateVertexBuffer(const void* data, size_t size, UINT stride) {
    return nullptr;
}

std::unique_ptr<IIndexBuffer> RenderApi_Vulkan::CreateIndexBuffer(const void* indices, size_t size) {
    return nullptr;
}

std::unique_ptr<IConstantBuffer> RenderApi_Vulkan::CreateConstantBuffer(const void* data, size_t size) {
    return nullptr;
}

DirectX::XMMATRIX RenderApi_Vulkan::GetProjection() const noexcept {
    return DirectX::XMMatrixIdentity();
}

void RenderApi_Vulkan::EndFrame() {
}

void RenderApi_Vulkan::DrawIndexed(UINT count) {
}

void RenderApi_Vulkan::Shutdown() {
    vkDestroyInstance(instance, nullptr);
    vkDestroyDevice(device, nullptr);
}

void RenderApi_Vulkan::CreateInstance() {
    if (enableValidationLayers && !CheckValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    std::vector requiredExtensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    };
    createInfo.enabledExtensionCount = requiredExtensions.size();
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan instance!");
    }
    std::cout << "Vulkan instance created\n" << std::endl;
}

bool RenderApi_Vulkan::CheckValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

void RenderApi_Vulkan::PickPhysicalDevice() {
    uint32_t deviceCount = 0;

    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    // Check each device and pick the first that fits our criteria
    for (const auto& device : devices) {
        if (IsDeviceSuitable(device)) {
            physicalDevice = device;
            break;
        }
    }
    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("Failed to find a suitable GPU!");
    }
}

bool RenderApi_Vulkan::IsDeviceSuitable(VkPhysicalDevice device) {
    // Example criteria: Device must be a discrete GPU and support Geometry Shader
    /*
     * VkPhysicalDeviceProperties deviceProperties;
     * VkPhysicalDeviceFeatures deviceFeatures;
     * vkGetPhysicalDeviceProperties(device, &deviceProperties);
     * vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
     * return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;
    */

    QueueFamilyIndices indices = FindQueueFamilies(device);

    return indices.isComplete();
}

RenderApi_Vulkan::QueueFamilyIndices RenderApi_Vulkan::FindQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }
        if (indices.isComplete()) break;
        i++;
    }
    return indices;
}

void RenderApi_Vulkan::CreateLogicalDevice() {
    QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;

    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = 0;

    // Set device-level validation layers for backwards compatibility. Current implementations ignore this
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device!");
    }
    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
}
