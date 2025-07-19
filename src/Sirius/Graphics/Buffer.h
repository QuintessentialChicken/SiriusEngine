//
// Created by Leon on 10/05/2025.
//

#ifndef BUFFER_H
#define BUFFER_H
#include <DirectXMath.h>
#include <intsafe.h>
#include <vulkan/vulkan.h>

class Buffer {
public:
    static void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                             VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkDevice device, VkPhysicalDevice physicalDevice);

    static void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDevice device, VkQueue graphicsQueue, VkCommandPool commandPool);
};

class VertexBuffer {
public:
    VertexBuffer(const void* data, VkDeviceSize size, VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool);

    ~VertexBuffer();

    void Bind();

    void Update(const void *data, size_t size);

    VkBuffer buffer;
private:
    VkDevice device;
    VkDeviceMemory memory;
};

class IndexBuffer {
public:
    IndexBuffer(const void* data, VkDeviceSize size, VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool);

    ~IndexBuffer();

    void Bind();

    [[nodiscard]] UINT GetCount() const;

    VkBuffer buffer;
private:
    VkDevice device;
    VkDeviceMemory memory;
    UINT count;
};

class ConstantBuffer {
public:
    ConstantBuffer(VkDeviceSize size, VkDevice device, VkPhysicalDevice physicalDevice);

    ~ConstantBuffer();

    void Update(const void* data, size_t size);
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory uniformBufferMemory = VK_NULL_HANDLE;
    void* uniformBufferMapped = nullptr;
private:
    VkDevice device;
};

class TransformBuffer {
public:
    struct Transforms {
        DirectX::XMMATRIX model;
        DirectX::XMMATRIX modelViewProj;
    };

    TransformBuffer();

    void Update(const DirectX::XMMATRIX& model, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection);

    void Bind() const;

private:
    ConstantBuffer constBuffer;
};

#endif //BUFFER_H
