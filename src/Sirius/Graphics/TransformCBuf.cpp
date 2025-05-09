//
// Created by Leon on 02/02/2025.
//

#include "TransformCBuf.h"

#include "ConstantBuffer.h"
#include "Drawable.h"
#include <DirectXMath.h>

#include "Camera.h"

TransformCBuf::TransformCBuf(const Drawable& parent) : parent{parent}
{
    if( !vcbuf )
    {
        vcbuf = std::make_unique<VertexConstantBuffer<Transforms>>();
    }
}

void TransformCBuf::Bind() noexcept
{
    const auto model = parent.GetTransformXMAlt();

    const Transforms tf =
{
        DirectX::XMMatrixTranspose( model ),
        DirectX::XMMatrixTranspose(
            model *
            GfxDevice::camera *
            GfxDevice::projection
        )
    };
    vcbuf->Update(tf);
    vcbuf->Bind();
}

std::unique_ptr<VertexConstantBuffer<TransformCBuf::Transforms>> TransformCBuf::vcbuf;