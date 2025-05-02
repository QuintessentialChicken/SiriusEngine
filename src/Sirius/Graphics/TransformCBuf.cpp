//
// Created by Leon on 02/02/2025.
//

#include "TransformCBuf.h"

#include "ConstantBuffer.h"
#include "Drawable.h"
#include <DirectXMath.h>

TransformCBuf::TransformCBuf(const Drawable& parent) : parent{parent}
{
    if( !vcbuf )
    {
        vcbuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>();
    }
}

void TransformCBuf::Bind() noexcept
{
    vcbuf->Update(DirectX::XMMatrixTranspose(parent.GetTransformXM() * GfxDevice::camera * GfxDevice::projection));
    vcbuf->Bind();
}

std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformCBuf::vcbuf;