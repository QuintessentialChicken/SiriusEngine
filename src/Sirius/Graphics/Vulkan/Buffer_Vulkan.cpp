//
// Created by Lms on 24/05/2025.
//

#include "Buffer_Vulkan.h"

#include <stdexcept>

#include "RenderApi_Vulkan.h"

void Buffer_Vulkan::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer,
                                 VkDeviceMemory& bufferMemory, VkDevice device, VkPhysicalDevice physicalDevice) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;

    // Iterate all available memory types and check if it has the right type and at least the required properties
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((memRequirements.memoryTypeBits & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            allocInfo.memoryTypeIndex = i;
            break;
        }
    }

    // TODO Switch from allocating everything individually to using a custom allocator. Should be fine for now
    if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate buffer memory!");
    }

    vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

void Buffer_Vulkan::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDevice device, VkQueue graphicsQueue, VkCommandPool commandPool) {
    // Memory transfer operations are executed using command buffers.
    // It could be beneficial to create a new pool for these short-lived buffers to allow for potential optimizations by the implementation
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

    // Begin recording. The ONE_TIME_SUBMIT_BIT tells the drivers our intent of using the buffer only once
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0; // Optional
    copyRegion.dstOffset = 0; // Optional
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);
    vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

/***************************** Vertex Buffer *****************************/
VertexBuffer_Vulkan::VertexBuffer_Vulkan(const void* data, VkDeviceSize size, VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool): device{device} {
    // Create a staging buffer to transfer data from the CPU to the actual vertex buffer, which is device local (i.e., not accessible by the CPU)
    VkDeviceSize bufferSize = size;
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    // TRANSFER_SRC_BIT specifies that this buffer can be used as the source in a memory transfer operation
    Buffer_Vulkan::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, device, physicalDevice);

    void* transferData;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &transferData);
    memcpy(transferData, data, bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    // TRANSFER_DST_BIT specifies that this buffer can be used as the destination in a memory transfer operation
    Buffer_Vulkan::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer, memory, device, physicalDevice);
    Buffer_Vulkan::CopyBuffer(stagingBuffer, buffer, bufferSize, device, graphicsQueue, commandPool);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

VertexBuffer_Vulkan::~VertexBuffer_Vulkan() {
    vkDestroyBuffer(device, buffer, nullptr);
    vkFreeMemory(device, memory, nullptr);
}

void VertexBuffer_Vulkan::Bind() {
}

void VertexBuffer_Vulkan::Update(const void* data, size_t size) {
}

/***************************** Index Buffer *****************************/
IndexBuffer_Vulkan::IndexBuffer_Vulkan(const void* data, VkDeviceSize size, VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool) : device{device}, count{static_cast<UINT>(size / sizeof(unsigned short))} {
    VkDeviceSize bufferSize = size;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    Buffer_Vulkan::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, device, physicalDevice);

    void* transferData;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &transferData);
    memcpy(transferData, data, bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    Buffer_Vulkan::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer, memory, device, physicalDevice);

    Buffer_Vulkan::CopyBuffer(stagingBuffer, buffer, bufferSize, device, graphicsQueue, commandPool);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

IndexBuffer_Vulkan::~IndexBuffer_Vulkan() {
    vkDestroyBuffer(device, buffer, nullptr);
    vkFreeMemory(device, memory, nullptr);
}

void IndexBuffer_Vulkan::Bind() {
}

UINT IndexBuffer_Vulkan::GetCount() const {
    return count;
}


/***************************** Constant Buffer *****************************/
ConstantBuffer_Vulkan::ConstantBuffer_Vulkan(VkDeviceSize size, VkDevice device, VkPhysicalDevice physicalDevice) : device{device} {
    VkDeviceSize bufferSize = size;

    Buffer_Vulkan::CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, buffer, uniformBufferMemory, device, physicalDevice);

    vkMapMemory(device, uniformBufferMemory, 0, bufferSize, 0, &uniformBufferMapped);
}

ConstantBuffer_Vulkan::~ConstantBuffer_Vulkan() {
    vkDestroyBuffer(device, buffer, nullptr);
    vkFreeMemory(device, uniformBufferMemory, nullptr);
}

void ConstantBuffer_Vulkan::Update(const void* data, size_t size) {
    memcpy(uniformBufferMapped, &data, size);
}

void ConstantBuffer_Vulkan::Bind(ShaderStage stages, UINT slot) {
}