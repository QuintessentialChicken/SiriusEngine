//
// Created by Leon on 04/02/2025.
//

#include "Cube.h"

#include <iostream>
#include <random>

#include "InputLayout.h"
#include "PixelShader.h"
#include "Topology.h"
#include "TransformCBuf.h"
#include "VertexBuffer.h"
#include "VertexShader.h"

Cube::Cube() {
    namespace dx = DirectX;
    std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution adist{0.0f, PI * 2.0f};
    std::uniform_real_distribution ddist{0.0f, PI * 0.5f};
    std::uniform_real_distribution odist{0.0f, PI * 0.08f};
    std::uniform_real_distribution rdist{6.0f, 20.0f};
    std::uniform_real_distribution bdist{0.4f, 3.0f};
    r = rdist(rng);
    droll = ddist(rng);
    dpitch = ddist(rng);
    dyaw = ddist(rng);
    dphi = odist(rng);
    dtheta = odist(rng);
    dchi = odist(rng);
    chi = adist(rng);
    theta = adist(rng);
    phi = adist(rng);

    if (!IsStaticInitialized()) {
        Cube::SetNormalsIndependentFlat();
        AddStaticBind(std::make_unique<VertexBuffer>(vertices));
        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(indices));

        auto vs = std::make_unique<VertexShader>(L"VertexShader.cso");
        auto vsbc = vs->GetBytecode();

        AddStaticBind(std::move(vs));

        AddStaticBind(std::make_unique<PixelShader>(L"PixelShader.cso"));


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

        AddStaticBind(std::make_unique<PixelConstantBuffer<PixelShaderConstants> >(colorBuffer));

        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
        {
            {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            // {"Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
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

DirectX::XMMATRIX Cube::GetTransformXMAlt() const noexcept {
    return DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
           DirectX::XMMatrixRotationRollPitchYaw(roll, pitch, yaw) *
           DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
           DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi);
}

void Cube::SetNormalsIndependentFlat() noexcept {
    {
        using namespace DirectX;
        assert( indices.size() % 3 == 0 && !indices.empty() );
        for( size_t i = 0; i < indices.size(); i += 3 )
        {
            auto& v0 = vertices[indices[i]];
            auto& v1 = vertices[indices[i + 1]];
            auto& v2 = vertices[indices[i + 2]];
            const auto p0 = XMLoadFloat3( &v0.pos );
            const auto p1 = XMLoadFloat3( &v1.pos );
            const auto p2 = XMLoadFloat3( &v2.pos );

            const auto n = XMVector3Normalize( XMVector3Cross( (p1 - p0),(p2 - p0) ) );

            XMStoreFloat3( &v0.n,n );
            XMStoreFloat3( &v1.n,n );
            XMStoreFloat3( &v2.n,n );
        }
    }
}

void Cube::Update(float dt) noexcept {
    roll += droll * dt;
    pitch += dpitch * dt;
    yaw += dyaw * dt;
    theta += dtheta * dt;
    phi += dphi * dt;
    chi += dchi * dt;
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
    Cube::rotation.z += rotation.z;
}

void Cube::SetScale(const DirectX::XMFLOAT3& scale) noexcept {
    Cube::scale.x = scale.x;
    Cube::scale.y = scale.y;
    Cube::scale.z = scale.z;
    width *= scale.x;
    height *= scale.y;
}
