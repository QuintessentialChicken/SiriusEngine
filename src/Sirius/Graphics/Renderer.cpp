//
// Created by Leon on 10/05/2025.
//

#include "Renderer.h"


class IPipelineState;
std::unique_ptr<RenderApi_Vulkan> Renderer::renderApi = std::make_unique<RenderApi_Vulkan>();

void Renderer::Init() {
    renderApi->Init();
}

void Renderer::BeginFrame() {
    renderApi->BeginFrame();
}

void Renderer::EndFrame() {
    renderApi->EndFrame();
}

void Renderer::Draw(const std::vector<Model>& models) {
    renderApi->Draw(models);
}

Shader Renderer::CreateShader(ShaderType type, const std::string& path) {
    return renderApi->CreateShader(type, path);
}

PipelineState Renderer::CreatePipelineState(const PipelineStateDesc& desc) {
    return renderApi->CreatePipelineState(desc);
}

void Renderer::DrawIndexed(UINT count) {
    renderApi->DrawIndexed(count);
}

void Renderer::Shutdown() {
    renderApi->Shutdown();
}

void Renderer::ResizeViewport(int width, int height) {
    renderApi->ResizeViewport(width, height);
}

VertexBuffer Renderer::CreateVertexBuffer(const void* data, size_t size, UINT stride) {
    return renderApi->CreateVertexBuffer(data, size, stride);
}

IndexBuffer Renderer::CreateIndexBuffer(const void* indices, size_t size) {
    return renderApi->CreateIndexBuffer(indices, size);
}

ConstantBuffer Renderer::CreateConstantBuffer(const void* data, size_t size) {
    return renderApi->CreateConstantBuffer(data, size);
}

DirectX::XMMATRIX Renderer::GetProjection() {
    return renderApi->GetProjection();
}

