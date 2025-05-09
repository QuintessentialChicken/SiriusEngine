//
// Created by Leon on 09/05/2025.
//

#include "Sphere.h"

#include <cassert>
#include <DirectXMath.h>
#include <random>

#include "TransformCBuf.h"

#include "ConstantBuffer.h"
#include "InputLayout.h"
#include "PixelShader.h"
#include "Topology.h"
#include "VertexBuffer.h"
#include "VertexShader.h"

Sphere::Sphere() : Sphere(12, 24) {}

Sphere::Sphere(int latDiv, int longDiv) {
    assert(latDiv >= 3);
    assert(longDiv >= 3);
    namespace dx = DirectX;

    // std::mt19937 rng{std::random_device{}()};
    // std::uniform_real_distribution adist{0.0f, PI * 2.0f};
    // std::uniform_real_distribution ddist{0.0f, PI * 0.5f};
    // std::uniform_real_distribution odist{0.0f, PI * 0.08f};
    // std::uniform_real_distribution rdist{6.0f, 20.0f};
    // std::uniform_real_distribution bdist{0.4f, 3.0f};
    // r = rdist(rng);
    // droll = ddist(rng);
    // dpitch = ddist(rng);
    // dyaw = ddist(rng);
    // dphi = odist(rng);
    // dtheta = odist(rng);
    // dchi = odist(rng);
    // chi = adist(rng);
    // theta = adist(rng);
    // phi = adist(rng);

    constexpr float radius = 1.0f;
    const auto base = dx::XMVectorSet(0.0f, 0.0f, radius, 0.0f);
    const float lattitudeAngle = PI / latDiv;
    const float longitudeAngle = 2.0f * PI / longDiv;

    for (int iLat = 1; iLat < latDiv; iLat++) {
        const auto latBase = dx::XMVector3Transform(
            base,
            dx::XMMatrixRotationX(lattitudeAngle * iLat)
        );
        for (int iLong = 0; iLong < longDiv; iLong++) {
            vertices.emplace_back();
            auto v = dx::XMVector3Transform(
                latBase,
                dx::XMMatrixRotationZ(longitudeAngle * iLong)
            );
            dx::XMStoreFloat3(&vertices.back().pos, v);
        }
    }

    // add the cap vertices
    const auto iNorthPole = static_cast<unsigned short>(vertices.size());
    vertices.emplace_back();
    dx::XMStoreFloat3(&vertices.back().pos, base);
    const auto iSouthPole = static_cast<unsigned short>(vertices.size());
    vertices.emplace_back();
    dx::XMStoreFloat3(&vertices.back().pos, dx::XMVectorNegate(base));

    const auto calcIdx = [latDiv,longDiv](unsigned short iLat, unsigned short iLong) { return iLat * longDiv + iLong; };
    std::vector<unsigned short> indices;
    for (unsigned short iLat = 0; iLat < latDiv - 2; iLat++) {
        for (unsigned short iLong = 0; iLong < longDiv - 1; iLong++) {
            indices.push_back(calcIdx(iLat, iLong));
            indices.push_back(calcIdx(iLat + 1, iLong));
            indices.push_back(calcIdx(iLat, iLong + 1));
            indices.push_back(calcIdx(iLat, iLong + 1));
            indices.push_back(calcIdx(iLat + 1, iLong));
            indices.push_back(calcIdx(iLat + 1, iLong + 1));
        }
        // wrap band
        indices.push_back(calcIdx(iLat, longDiv - 1));
        indices.push_back(calcIdx(iLat + 1, longDiv - 1));
        indices.push_back(calcIdx(iLat, 0));
        indices.push_back(calcIdx(iLat, 0));
        indices.push_back(calcIdx(iLat + 1, longDiv - 1));
        indices.push_back(calcIdx(iLat + 1, 0));
    }

    // cap fans
    for (unsigned short iLong = 0; iLong < longDiv - 1; iLong++) {
        // north
        indices.push_back(iNorthPole);
        indices.push_back(calcIdx(0, iLong));
        indices.push_back(calcIdx(0, iLong + 1));
        // south
        indices.push_back(calcIdx(latDiv - 2, iLong + 1));
        indices.push_back(calcIdx(latDiv - 2, iLong));
        indices.push_back(iSouthPole);
    }
    // wrap triangles
    // north
    indices.push_back(iNorthPole);
    indices.push_back(calcIdx(0, longDiv - 1));
    indices.push_back(calcIdx(0, 0));
    // south
    indices.push_back(calcIdx(latDiv - 2, 0));
    indices.push_back(calcIdx(latDiv - 2, longDiv - 1));
    indices.push_back(iSouthPole);

    if (!IsStaticInitialized()) {
        // model.Transform( dx::XMMatrixScaling( radius,radius,radius ) );
        AddStaticBind(std::make_unique<VertexBuffer>(vertices));
        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(indices));

        auto vs = std::make_unique<VertexShader>(L"SolidVS.cso");
        auto vsbc = vs->GetBytecode();

        AddStaticBind(std::move(vs));

        AddStaticBind(std::make_unique<PixelShader>(L"SolidPS.cso"));

        struct PSColorConstant {
            dx::XMFLOAT3 color = {1.0f, 1.0f, 1.0f};
            float padding;
        } colorConst;

        AddStaticBind(std::make_unique<PixelConstantBuffer<PSColorConstant>>(colorConst));

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

void Sphere::Update(float dt) noexcept {
    roll += droll * dt;
    pitch += dpitch * dt;
    yaw += dyaw * dt;
    theta += dtheta * dt;
    phi += dphi * dt;
    chi += dchi * dt;
}

DirectX::XMMATRIX Sphere::GetTransformXM() const noexcept {
    return DirectX::XMMatrixTranslation(position.x, position.y, position.z);
}

DirectX::XMMATRIX Sphere::GetTransformXMAlt() const noexcept {
    return DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
       DirectX::XMMatrixRotationRollPitchYaw(roll, pitch, yaw) *
       DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
       DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi);
}

void Sphere::SetTransform(const DirectX::XMFLOAT3& pos) noexcept {
    position = pos;
}

void Sphere::AddTransform(const DirectX::XMFLOAT3& distance) noexcept {
    position.x += distance.x;
    position.y += distance.y;
    position.z += distance.z;
}

void Sphere::SetRotation(const DirectX::XMFLOAT3& rotation) noexcept {
    Sphere::rotation.x += rotation.x;
    Sphere::rotation.y += rotation.y;
    Sphere::rotation.z += rotation.z;
}

void Sphere::SetScale(const DirectX::XMFLOAT3& scale) noexcept {
    Sphere::scale.x = scale.x;
    Sphere::scale.y = scale.y;
    Sphere::scale.z = scale.z;
    width *= scale.x;
    height *= scale.y;
}
