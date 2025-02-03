//
// Created by Leon on 02/02/2025.
//

#include "Box.h"

#include "ConstantBuffer.h"
#include "Cube.h"
#include "InputLayout.h"
#include "PixelShader.h"
#include "Topology.h"
#include "TransformCBuf.h"
#include "VertexBuffer.h"
#include "VertexShader.h"

Box::Box(
    Graphics& gfx, std::mt19937& rng,
    std::uniform_real_distribution<float>& adist,
    std::uniform_real_distribution<float>& ddist,
    std::uniform_real_distribution<float>& odist,
    std::uniform_real_distribution<float>& rdist)
    : r(rdist(rng)),
      theta(adist(rng)),
      phi(adist(rng)),
      chi(adist(rng)),
      droll(ddist(rng)),
      dpitch(ddist(rng)),
      dyaw(ddist(rng)),
      dtheta(odist(rng)),
      dphi(odist(rng)),
      dchi(odist(rng)) {

    namespace dx = DirectX;

    if (!IsStaticInitialized()) {
        struct Vertex {
            struct
            {
                float x;
                float y;
                float z;
            } pos;
        };
        const std::vector<Vertex> vertices =
        {
            { -1.0f,-1.0f,-1.0f },
            { 1.0f,-1.0f,-1.0f },
            { -1.0f,1.0f,-1.0f },
            { 1.0f,1.0f,-1.0f },
            { -1.0f,-1.0f,1.0f },
            { 1.0f,-1.0f,1.0f },
            { -1.0f,1.0f,1.0f },
            { 1.0f,1.0f,1.0f },
        };

        AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));

        auto vs = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
        auto vsbc = vs->GetBytecode();

        AddStaticBind(std::move(vs));

        AddStaticBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));

        const std::vector<unsigned short> indices =
        {
            0,2,1, 2,3,1,
            1,3,5, 3,7,5,
            2,6,3, 3,6,7,
            4,5,7, 4,7,6,
            0,4,2, 2,4,6,
            0,1,4, 1,5,4
        };
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
                { 1.0f,0.0f,1.0f },
                { 1.0f,0.0f,0.0f },
                { 0.0f,1.0f,0.0f },
                { 0.0f,0.0f,1.0f },
                { 1.0f,1.0f,0.0f },
                { 0.0f,1.0f,1.0f },
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
    // XMStoreFloat3x3(&mt, dx::XMMatrixScaling(1.0f, 1.0f, bdist(rng)));
}

void Box::Update(const float dt) noexcept {
    roll += droll * dt;
    pitch += dpitch * dt;
    yaw += dyaw * dt;
    theta += dtheta * dt;
    phi += dphi * dt;
    chi += dchi * dt;
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept {
    namespace dx = DirectX;
    return dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
           dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
           dx::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
           dx::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}
