//
// Created by Leon on 11/05/2025.
//

#include "Model.h"

#include <utility>

#include "../Graphics/Renderer.h"
#include "../../Game/App.h"

Model::Model(std::unique_ptr<Mesh> mesh, std::unique_ptr<Material> material)
: material(std::move(material)), transformBuffer(std::make_unique<TransformBuffer>()) {
    AddComponent<Mesh>(std::move(mesh));
}

std::unique_ptr<Model> Model::CreatePrimitive(Primitives primitive) {
    switch (primitive) {
        case Primitives::CUBE:
            return std::make_unique<Model>(Mesh::CreateCube(), std::make_unique<ColoredCubeMaterial>());
        case Primitives::SPHERE:
            return std::make_unique<Model>(Mesh::CreateSphere(), std::make_unique<ColoredCubeMaterial>());
        case Primitives::CUBE_PHONG:
            return std::make_unique<Model>(Mesh::CreateCube(), Material::CreatePhongMaterial());
        default:
            return nullptr;
    }
}

void Model::Draw(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection) {
    auto mesh = GetComponent<Mesh>();
    if (GetComponent<Mesh>() == nullptr) return;
    // Update transform buffer
    transformBuffer->Update(transform.GetMatrix(), view, projection);

    // Bind resources
    material->Bind();
    mesh->Bind();
    transformBuffer->Bind();

    // Draw
    Renderer::DrawIndexed(mesh->GetIndexCount());
}

void Model::SetPosition(const DirectX::XMFLOAT3& pos) {
    GetTransform().SetPosition(pos);
}

void Model::Translate(const DirectX::XMFLOAT3& offset) {
    GetTransform().Translate(offset);
}

void Model::SetRotation(const DirectX::XMFLOAT3& rot) {
    GetTransform().SetRotation(rot);
}

void Model::SetScale(const DirectX::XMFLOAT3& scl) {
    GetTransform().SetScale(scl);
}

void Model::Update(float dt) {
    roll += droll * dt;
    pitch += dpitch * dt;
    yaw += dyaw * dt;
    theta += dtheta * dt;
    phi += dphi * dt;
    chi += dchi * dt;

    UpdateTransform();
}

Transform& Model::GetTransform() { return transform; }

void Model::UpdateTransform() {
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
    GetTransform().SetPosition(pos);
    // For quaternion to Euler conversion, you might need more complex math
    // This is a simplified approach:
    GetTransform().SetRotation(DirectX::XMFLOAT3(pitch, yaw, roll));
    GetTransform().SetScale(scl);
}
