//
// Created by Leon on 04/02/2025.
//

#include "Cube.h"

#include <iostream>
#include <random>

#include "ConstantBuffer.h"
#include "InputLayout.h"
#include "PixelShader.h"
#include "Topology.h"
#include "TransformCBuf.h"
#include "VertexBuffer.h"
#include "VertexShader.h"

Cube::Cube() {
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
        AddStaticBind(std::make_unique<VertexBuffer>(vertices));

        auto vs = std::make_unique<VertexShader>(L"VertexShader.cso");
        auto vsbc = vs->GetBytecode();

        AddStaticBind(std::move(vs));

        AddStaticBind(std::make_unique<PixelShader>(L"PixelShader.cso"));

        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(indices));

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
        AddStaticBind(std::make_unique<PixelConstantBuffer<PixelShaderConstants>>(colorBuffer));

        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
        {
            {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };
        AddStaticBind(std::make_unique<InputLayout>(ied, vsbc));

        AddStaticBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    } else {
        SetIndexFromStatic();
    }

    AddBind(std::make_unique<TransformCBuf>(*this));
}

// TODO Move Matrix Calculation to separate method which is called when transformations (pos, rot, scale) are updated
DirectX::XMMATRIX Cube::GetTransformXM() const noexcept {
    return DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
           DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) *
           DirectX::XMMatrixTranslation(position.x, position.y, position.z);
}

void Cube::SetTransform(const DirectX::XMFLOAT3& pos) noexcept {
    position = pos;
}

void Cube::AddTransform(const DirectX::XMFLOAT3& distance) noexcept {
    position.x += distance.x;
    position.y += distance.y;
    position.z += distance.z;
}

void Cube::SetRotation(const DirectX::XMFLOAT3& rotation) noexcept {
    Cube::rotation.x += rotation.x;
    Cube::rotation.y += rotation.y;
    Cube::rotation.z += rotation.z;}

void Cube::SetScale(const DirectX::XMFLOAT3& scale) noexcept {
}
