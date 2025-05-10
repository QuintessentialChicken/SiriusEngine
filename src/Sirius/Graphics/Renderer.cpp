//
// Created by Leon on 10/05/2025.
//

#include "Renderer.h"

std::unique_ptr<IRenderApi> Renderer::renderApi = IRenderApi::Create();

void Renderer::Init() {
    renderApi->Init();
}

void Renderer::BeginFrame() {
    renderApi->BeginFrame();
}

void Renderer::EndFrame() {
    renderApi->EndFrame();
}

void Renderer::DrawIndexed(UINT count) {
    renderApi->DrawIndexed(count);
}

void Renderer::Shutdown() {
    renderApi->Shutdown();
}

std::unique_ptr<IVertexBuffer> Renderer::CreateVertexBuffer(const void* data, size_t size, UINT stride) {
    return renderApi->CreateVertexBuffer(data, size, stride);
}

std::unique_ptr<IIndexBuffer> Renderer::CreateIndexBuffer(const void* indices, size_t size) {
}

std::unique_ptr<IConstantBuffer> Renderer::CreateConstantBuffer(const void* data, size_t size) {
}
