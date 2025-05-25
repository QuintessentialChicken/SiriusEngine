//
// Created by Lms on 24/05/2025.
//

#ifndef BUFFER_VULKAN_H
#define BUFFER_VULKAN_H

#include <vulkan/vulkan_core.h>

#include "Graphics/Buffer.h"


class VertexBuffer_Vulkan : public IVertexBuffer {
public:
    VertexBuffer_Vulkan(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

    void Bind() override;

    void Update(const void *data, size_t size) override;

private:
    VkBuffer buffer;
    VkDeviceMemory memory;
};



#endif //BUFFER_VULKAN_H
