//
// Created by Leon on 10/05/2025.
//

#include "Renderer.h"

struct PipelineStateDesc;
class IPipelineState;
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

std::unique_ptr<IShader> Renderer::CreateShader(ShaderType type, const std::wstring& path) {
    return renderApi->CreateShader(type, path);
}

std::unique_ptr<IInputLayout> Renderer::CreateInputLayout(const std::vector<InputLayoutElement>& elements, const void* shaderBytecode, size_t bytecodeSize) {
    return renderApi->CreateInputLayout(elements, shaderBytecode, bytecodeSize);
}

std::unique_ptr<IPipelineState> Renderer::CreatePipelineState(const PipelineStateDesc& desc) {
    return renderApi->CreatePipelineState(desc);
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
    return renderApi->CreateIndexBuffer(indices, size);
}

std::unique_ptr<IConstantBuffer> Renderer::CreateConstantBuffer(const void* data, size_t size) {
    return renderApi->CreateConstantBuffer(data, size);
}

DirectX::XMMATRIX Renderer::GetProjection() {
    return renderApi->GetProjection();
}

