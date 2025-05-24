//
// Created by Lms on 24/05/2025.
//

#ifndef BUFFER_VULKAN_H
#define BUFFER_VULKAN_H

#include "Graphics/Buffer.h"


class VertexBuffer_Vulkan : public IVertexBuffer {
public:
    VertexBuffer_Vulkan(uint32_t stride);

    void Bind() override;

    void Update(const void *data, size_t size) override;

private:

};



#endif //BUFFER_VULKAN_H
