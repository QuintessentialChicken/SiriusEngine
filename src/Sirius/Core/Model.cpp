//
// Created by Leon on 11/05/2025.
//

#include "Model.h"

#include <utility>
#include <External/imgui.h>

#include "Graphics/Renderer.h"
#include "Graphics/PointLight.h"
#include "Input/Mouse.h"

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
        case Primitives::POINTLIGHT: {
            auto light = std::make_unique<Model>(Mesh::CreateSphere(), std::make_unique<ColoredCubeMaterial>());
            light->AddComponent<PointLight>(std::make_unique<PointLight>());
            return light;
        }
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
    transform.SetPosition(pos);
}

void Model::Translate(const DirectX::XMFLOAT3& offset) {
    transform.Translate(offset);
}

void Model::SetRotation(const DirectX::XMFLOAT3& rot) {
    transform.SetRotation(rot);
}

void Model::SetScale(const DirectX::XMFLOAT3& scl) {
    transform.SetScale(scl);
}

void Model::Update(float dt) {
    for (const auto& component : components) {
        component->Update();
    }
}

void Model::SpawnControlWindow() {
    if (ImGui::Begin("Transform")) {
        ImGui::SliderFloat("X", &transform.position.x, -20.0f, 20.0f);
        ImGui::SliderFloat("Y", &transform.position.y, -20.0f, 20.0f);
        ImGui::SliderFloat("Z", &transform.position.z, -20.0f, 20.0f);
        ImGui::Checkbox("Take control", &userControlled);
        transform.UpdateMatrix();
    }
    ImGui::End();
}

// Transform& Model::GetTransform() { return transform; }
