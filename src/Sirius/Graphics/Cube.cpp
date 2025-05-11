//
// Created by Leon on 04/02/2025.
//

#include "Cube.h"

#include <random>

#include <DirectXMath.h>

constexpr float PI = 3.14159265f;

Cube::Cube() {
    std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution adist{0.0f, PI * 2.0f};
    std::uniform_real_distribution ddist{0.0f, PI * 0.5f};
    std::uniform_real_distribution odist{0.0f, PI * 0.08f};
    std::uniform_real_distribution rdist{6.0f, 20.0f};

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

    // Create mesh and material
    auto mesh = Mesh::CreateCube();
    auto material = std::make_shared<ColoredCubeMaterial>();

    // Create model
    model = std::make_unique<Model>(mesh, material);
}

void Cube::Update(float dt) {
    // Update rotation angles
    roll += droll * dt;
    pitch += dpitch * dt;
    yaw += dyaw * dt;
    theta += dtheta * dt;
    phi += dphi * dt;
    chi += dchi * dt;

    // Update transform based on animation parameters
    UpdateTransform();
}

void Cube::Draw(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection) {
    model->Draw(view, projection);
}

void Cube::UpdateTransform() {
    // Create a transform matrix that mimics the old behavior
    DirectX::XMMATRIX rotMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
    DirectX::XMMATRIX transMatrix = DirectX::XMMatrixTranslation(r, 0.0f, 0.0f);
    DirectX::XMMATRIX orbitMatrix = DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi);

    // Combine transformations
    DirectX::XMMATRIX worldMatrix = rotMatrix * transMatrix * orbitMatrix;

    // Extract position, rotation, and scale from the matrix
    DirectX::XMVECTOR scale;
    DirectX::XMVECTOR rotation;
    DirectX::XMVECTOR position;

    DirectX::XMMatrixDecompose(&scale, &rotation, &position, worldMatrix);

    // Convert to XMFLOAT3
    DirectX::XMFLOAT3 pos{}, rot{}, scl{};
    DirectX::XMStoreFloat3(&pos, position);
    DirectX::XMStoreFloat3(&rot, rotation); // Note: this is a quaternion, not Euler angles
    DirectX::XMStoreFloat3(&scl, scale);

    // Update the model's transform
    model->GetTransform().SetPosition(pos);
    // For quaternion to Euler conversion, you might need more complex math
    // This is a simplified approach:
    model->GetTransform().SetRotation(DirectX::XMFLOAT3(pitch, yaw, roll));
    model->GetTransform().SetScale(scl);
}

void Cube::SetPosition(const DirectX::XMFLOAT3& pos) {
    model->GetTransform().SetPosition(pos);
}

void Cube::Translate(const DirectX::XMFLOAT3& offset) {
    model->GetTransform().Translate(offset);
}

void Cube::SetRotation(const DirectX::XMFLOAT3& rot) {
    model->GetTransform().SetRotation(rot);
}

void Cube::SetScale(const DirectX::XMFLOAT3& scl) {
    model->GetTransform().SetScale(scl);
}

