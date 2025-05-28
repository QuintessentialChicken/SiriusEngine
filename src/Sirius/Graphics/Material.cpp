//
// Created by Leon on 11/05/2025.
//

#include "Material.h"

#include <vector>

#include "Renderer.h"

Material::Material(const std::wstring& vertexShaderPath, const std::wstring& pixelShaderPath) {
    // Create shaders
    vertexShader = Renderer::CreateShader(ShaderType::Vertex, vertexShaderPath);
    pixelShader = Renderer::CreateShader(ShaderType::Pixel, pixelShaderPath);

    // Create input layout
    const std::vector<InputLayoutElement> elements = {
        {"Position", 0, DataFormat::Float3, 0, 0},
        {"Normal", 0, DataFormat::Float3, 0, 12}
    };
    // inputLayout = Renderer::CreateInputLayout(elements, vertexShader->GetBytecode(), vertexShader->GetBytecodeSize());

    // Create pipeline state
    PipelineStateDesc pipelineDesc;
    pipelineDesc.topology = PrimitiveTopology::TriangleList;
    // pipelineDesc.inputLayout = inputLayout.get();
    pipelineDesc.vertexShader = vertexShader.get();
    pipelineDesc.pixelShader = pixelShader.get();
    pipelineState = Renderer::CreatePipelineState(pipelineDesc);
}

void Material::Bind() {
    pipelineState->Bind();
}

std::unique_ptr<Material> Material::CreatePhongMaterial() {
    return std::make_unique<Material>(L"PhongVS.cso", L"PhongPS.cso");
}

ColoredCubeMaterial::ColoredCubeMaterial() : Material(L"VertexShader.cso", L"PixelShader.cso") {
    colorData = {
        DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
        DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
        DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
        DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f),
        DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f),
        DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)
    };

    colorBuffer = Renderer::CreateConstantBuffer(&colorData, sizeof(colorData));
    colorBufferDirty = false;
}

void ColoredCubeMaterial::SetFaceColor(int faceIndex, const DirectX::XMFLOAT4& color) {
    if (faceIndex >= 0 && faceIndex < 6) {
        colorData.faceColors[faceIndex] = color;
        colorBufferDirty = true;
    }
}

void ColoredCubeMaterial::Bind() {
    // First call the base class Bind to set up the pipeline state
    Material::Bind();

    // Update the color buffer if needed
    if (colorBufferDirty) {
        colorBuffer->Update(&colorData, sizeof(ColorBufferData));
        colorBufferDirty = false;
    }

    // Bind the color buffer to pixel shader slot 0
    colorBuffer->Bind(ShaderStage::Pixel, 0);
}


