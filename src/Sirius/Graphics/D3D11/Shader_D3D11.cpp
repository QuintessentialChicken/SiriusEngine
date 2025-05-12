//
// Created by Leon on 11/05/2025.
//

#include "Shader_D3D11.h"

#include <d3dcompiler.h>
#include <stdexcept>

Shader_D3D11::Shader_D3D11(ShaderType type, const std::wstring& path, ID3D11Device* device, ID3D11DeviceContext* context)
: type{type}, device{device}, context{context} {
    HRESULT hr = D3DReadFileToBlob(path.c_str(), &byteCode);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to read shader file");
    }

    switch (type) {
        case ShaderType::Vertex:
            hr = device->CreateVertexShader(
                byteCode->GetBufferPointer(),
                byteCode->GetBufferSize(),
                nullptr,
                &vertexShader
            );
            break;

        case ShaderType::Pixel:
            hr = device->CreatePixelShader(
                byteCode->GetBufferPointer(),
                byteCode->GetBufferSize(),
                nullptr,
                &pixelShader
            );
            break;

        case ShaderType::Geometry:
            hr = device->CreateGeometryShader(
                byteCode->GetBufferPointer(),
                byteCode->GetBufferSize(),
                nullptr,
                &geometryShader
            );
            break;

        case ShaderType::Compute:
            hr = device->CreateComputeShader(
                byteCode->GetBufferPointer(),
                byteCode->GetBufferSize(),
                nullptr,
                &computeShader
            );
            break;

        case ShaderType::Hull:
            hr = device->CreateHullShader(
                byteCode->GetBufferPointer(),
                byteCode->GetBufferSize(),
                nullptr,
                &hullShader
            );
            break;

        case ShaderType::Domain:
            hr = device->CreateDomainShader(
                byteCode->GetBufferPointer(),
                byteCode->GetBufferSize(),
                nullptr,
                &domainShader
            );
            break;
    }

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create shader");
    }
}

void Shader_D3D11::Bind() {
    switch (type) {
        case ShaderType::Vertex:
            context->VSSetShader(vertexShader.Get(), nullptr, 0);
            break;

        case ShaderType::Pixel:
            context->PSSetShader(pixelShader.Get(), nullptr, 0);
            break;

        case ShaderType::Geometry:
            context->GSSetShader(geometryShader.Get(), nullptr, 0);
            break;

        case ShaderType::Compute:
            context->CSSetShader(computeShader.Get(), nullptr, 0);
            break;

        case ShaderType::Hull:
            context->HSSetShader(hullShader.Get(), nullptr, 0);
            break;

        case ShaderType::Domain:
            context->DSSetShader(domainShader.Get(), nullptr, 0);
            break;
    }
}

const void* Shader_D3D11::GetBytecode() const {
    return byteCode->GetBufferPointer();
}

size_t Shader_D3D11::GetBytecodeSize() const {
    return byteCode->GetBufferSize();
}
