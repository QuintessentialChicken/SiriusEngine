//
// Created by Leon on 06/02/2025.
//

#include "Plane.h"

#include "ConstantBuffer.h"
#include "InputLayout.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "Surface.h"
#include "Texture.h"
#include "Topology.h"
#include "TransformCBuf.h"
#include "VertexBuffer.h"
#include "VertexShader.h"

Plane::Plane(Graphics& gfx) {
    if (!IsStaticInitialized()) {

        AddStaticBind( std::make_unique<Texture>( gfx,Surface::FromFile( R"(..\..\resources\superman.png)" ) ) );

        AddStaticBind( std::make_unique<VertexBuffer>( vertices ) );

        AddStaticBind( std::make_unique<Sampler>( gfx ) );

        auto pvs = std::make_unique<VertexShader>( gfx,L"TextureVS.cso" );
        auto pvsbc = pvs->GetBytecode();
        AddStaticBind( std::move( pvs ) );

        AddStaticBind( std::make_unique<PixelShader>( gfx,L"TexturePS.cso" ) );

        AddStaticIndexBuffer( std::make_unique<IndexBuffer>( gfx, indices ) );

        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
        {
            { "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
            { "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
        };
        AddStaticBind( std::make_unique<InputLayout>( gfx,ied,pvsbc ) );

        AddStaticBind( std::make_unique<Topology>( gfx,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
    } else {
        SetIndexFromStatic();
    }

    AddBind(std::make_unique<TransformCBuf>(gfx, *this));

    // model deformation transform (per instance, not stored as bind)
    Plane::SetScale({2.0f, 2.0f, 2.0f});
}

DirectX::XMMATRIX Plane::GetTransformXM() const noexcept {
    return DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
           DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) *
           DirectX::XMMatrixTranslation(position.x, position.y, position.z);
}

void Plane::SetTransform(const DirectX::XMFLOAT3& position) noexcept {
    Plane::position = position;
}

void Plane::AddTransform(const DirectX::XMFLOAT3& distance) noexcept {
    position.x += distance.x;
    position.y += distance.y;
    position.z += distance.z;
}


/**
 * 
 * @param rotation Angle in Radians
 */
void Plane::SetRotation(const DirectX::XMFLOAT3& rotation) noexcept {
    Plane::rotation.x = rotation.x;
    Plane::rotation.y = rotation.y;
    Plane::rotation.z = rotation.z;
}

void Plane::SetScale(const DirectX::XMFLOAT3& scale) noexcept {
    Plane::scale.x = scale.x;
    Plane::scale.y = scale.y;
    Plane::scale.z = scale.z;
}
