//
// Created by Lms on 24/05/2025.
//

#ifndef BUFFER_VULKAN_H
#define BUFFER_VULKAN_H

#include <vector>
#include <vulkan/vulkan_core.h>

#include "Graphics/Buffer.h"

class Buffer_Vulkan {
public:
    static void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                             VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkDevice device, VkPhysicalDevice physicalDevice);

    static void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDevice device, VkQueue graphicsQueue, VkCommandPool commandPool);
};
class VertexBuffer_Vulkan : public IVertexBuffer {
public:
    VertexBuffer_Vulkan(const void* data, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool);

    ~VertexBuffer_Vulkan() override;

    void Bind() override;

    void Update(const void *data, size_t size) override;

private:
    VkDevice device;
    VkBuffer buffer;
    VkDeviceMemory memory;
};

class IndexBuffer_Vulkan : public IIndexBuffer {
public:
    IndexBuffer_Vulkan(const void* data, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool);

    ~IndexBuffer_Vulkan() override;

    void Bind() override;

    [[nodiscard]] UINT GetCount() const override;

private:
    VkDevice device;
    VkBuffer buffer;
    VkDeviceMemory memory;
    UINT count;
};

class ConstantBuffer_Vulkan : public IConstantBuffer {
public:
    ConstantBuffer_Vulkan(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, int framesInFlight, VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool);

    ~ConstantBuffer_Vulkan() override;

    void Update(const void* data, size_t size) override;
private:
    VkDevice device;
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;
};





#endif //BUFFER_VULKAN_H
