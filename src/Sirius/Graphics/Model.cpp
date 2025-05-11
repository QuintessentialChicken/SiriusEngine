//
// Created by Leon on 11/05/2025.
//

#include "Model.h"

#include <utility>

#include "Renderer.h"

Model::Model(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
: mesh(std::move(mesh)), material(std::move(material)), transformBuffer(std::make_unique<TransformBuffer>())
{}

void Model::Draw(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection) {
    // Update transform buffer
    transformBuffer->Update(transform.GetMatrix(), view, projection);

    // Bind resources
    material->Bind();
    mesh->Bind();
    transformBuffer->Bind();

    // Draw
    Renderer::DrawIndexed(mesh->GetIndexCount());
}

Transform& Model::GetTransform() { return transform; }
