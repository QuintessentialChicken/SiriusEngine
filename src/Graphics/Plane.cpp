//
// Created by Leon on 06/02/2025.
//

#include "Plane.h"

#include "ConstantBuffer.h"
#include "InputLayout.h"
#include "PixelShader.h"
#include "Topology.h"
#include "TransformCBuf.h"
#include "VertexBuffer.h"
#include "VertexShader.h"

Plane::Plane(Graphics& gfx) {
    if (!IsStaticInitialized()) {
        AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));

        auto vs = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
        auto vsbc = vs->GetBytecode();

        AddStaticBind(std::move(vs));

        AddStaticBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));

        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

        struct PixelShaderConstants {
            struct {
                float r;
                float g;
                float b;
                float a;
            } face_colors[1];
        };
        constexpr PixelShaderConstants colorBuffer =
        {
            {
                {1.0f, 1.0f, 1.0f}
            }
        };
        AddStaticBind(std::make_unique<PixelConstantBuffer<PixelShaderConstants> >(gfx, colorBuffer));

        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
        {
            {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };
        AddStaticBind(std::make_unique<InputLayout>(gfx, ied, vsbc));

        AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    } else {
        SetIndexFromStatic();
    }

    AddBind(std::make_unique<TransformCBuf>(gfx, *this));

    // model deformation transform (per instance, not stored as bind)
    Plane::SetTransform({0.0f, 0.0f, 10.0f});
    Plane::SetRotation({1.5708f, 0.0f, 0.0f});
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
