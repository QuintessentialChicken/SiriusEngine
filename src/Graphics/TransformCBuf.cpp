//
// Created by Leon on 02/02/2025.
//

#include "TransformCBuf.h"

#include "ConstantBuffer.h"
#include "Drawable.h"
#include <DirectXMath.h>

TransformCBuf::TransformCBuf( Graphics& gfx, const Drawable& parent) : parent{parent}
{
    if( !vcbuf )
    {
        vcbuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>( gfx );
    }
}

void TransformCBuf::Bind( Graphics& gfx ) noexcept
{
    vcbuf->Update(gfx, XMMatrixTranspose(parent.GetTransformXM() * gfx.GetCamera() * gfx.GetProjection()));
    vcbuf->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformCBuf::vcbuf;