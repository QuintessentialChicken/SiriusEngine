//
// Created by Lms on 24/05/2025.
//

#ifndef BUFFER_VULKAN_H
#define BUFFER_VULKAN_H
#include <intsafe.h>
#include <vulkan/vulkan_core.h>

enum class ShaderStage;

class Buffer_Vulkan {
public:
    static void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                             VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkDevice device, VkPhysicalDevice physicalDevice);

    static void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDevice device, VkQueue graphicsQueue, VkCommandPool commandPool);
};
class VertexBuffer_Vulkan {
public:
    VertexBuffer_Vulkan(const void* data, VkDeviceSize size, VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool);

    ~VertexBuffer_Vulkan();

    void Bind();

    void Update(const void *data, size_t size);

    VkBuffer buffer;
private:
    VkDevice device;
    VkDeviceMemory memory;
};

class IndexBuffer_Vulkan {
public:
    IndexBuffer_Vulkan(const void* data, VkDeviceSize size, VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool);

    ~IndexBuffer_Vulkan();

    void Bind();

    [[nodiscard]] UINT GetCount() const;

    VkBuffer buffer;
private:
    VkDevice device;
    VkDeviceMemory memory;
    UINT count;
};

class ConstantBuffer_Vulkan {
public:
    ConstantBuffer_Vulkan(VkDeviceSize size, VkDevice device, VkPhysicalDevice physicalDevice);

    ~ConstantBuffer_Vulkan();

    void Bind(ShaderStage stages, UINT slot);

    void Update(const void* data, size_t size);
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory uniformBufferMemory = VK_NULL_HANDLE;
    void* uniformBufferMapped = nullptr;
private:
    VkDevice device;
};





#endif //BUFFER_VULKAN_H
