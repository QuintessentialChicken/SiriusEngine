//
// Created by Leon on 04/02/2025.
//

#include "Cube.h"

#include <random>

#include "ConstantBuffer.h"
#include "InputLayout.h"
#include "PixelShader.h"
#include "Topology.h"
#include "TransformCBuf.h"
#include "VertexBuffer.h"
#include "VertexShader.h"

Cube::Cube(Graphics& gfx) {

    namespace dx = DirectX;
    std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<float> bdist{1.0f, 3.0f};
    r = bdist(rng);
    droll = bdist(rng);
    dpitch = bdist(rng);
    dyaw = bdist(rng);
    dtheta = bdist(rng);
    dphi = bdist(rng);
    dchi = bdist(rng);

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
            } face_colors[6];
        };
        const PixelShaderConstants colorBuffer =
        {
            {
                {1.0f, 0.0f, 1.0f},
                {1.0f, 0.0f, 0.0f},
                {0.0f, 1.0f, 0.0f},
                {0.0f, 0.0f, 1.0f},
                {1.0f, 1.0f, 0.0f},
                {0.0f, 1.0f, 1.0f},
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
    XMStoreFloat3x3(&mt, dx::XMMatrixScaling(4.0f, 2.0f, 3.0f));
}

void Cube::Update(const float dt) noexcept {
    roll += droll * dt;
    pitch += dpitch * dt;
    yaw += dyaw * dt;
    theta += dtheta * dt;
    phi += dphi * dt;
    chi += dchi * dt;
}

DirectX::XMMATRIX Cube::GetTransformXM() const noexcept {
    namespace dx = DirectX;
    return dx::XMLoadFloat3x3(&mt) * // Individual transformation per box
           dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
           dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
           dx::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
           dx::XMMatrixTranslation(0.0f, 0.0f, 5.0f);
}
