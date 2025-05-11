//
// Created by Leon on 11/05/2025.
//

#include "Buffer.h"

#include "Renderer.h"

TransformBuffer::TransformBuffer() {
    // Create the constant buffer when the TransformBuffer is constructed
    constBuffer = Renderer::CreateConstantBuffer(nullptr, sizeof(Transforms));
}

void TransformBuffer::Update(const DirectX::XMMATRIX& model, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection) const {
    const Transforms transforms = {
        DirectX::XMMatrixTranspose(model),
        DirectX::XMMatrixTranspose(model * view * projection)
    };

    constBuffer->Update(&transforms, sizeof(transforms));
}

void TransformBuffer::Bind() const {
    constBuffer->Bind(ShaderStage::Vertex, 0);
}
