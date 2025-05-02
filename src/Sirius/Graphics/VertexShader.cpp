//
// Created by Leon on 02/02/2025.
//

#include <d3dcompiler.h>


#include "VertexShader.h"

#include <string>

#include "GfxDevice.h"

VertexShader::VertexShader(const std::wstring& path) {
    if (FAILED(D3DReadFileToBlob(path.c_str(),&bytecodeBlob))) {
        throw std::runtime_error("Failed to read vertex shader");
    }
    if (FAILED(GfxDevice::device->CreateVertexShader(
        bytecodeBlob->GetBufferPointer(),
        bytecodeBlob->GetBufferSize(),
        nullptr,
        &vertexShader
    ))) {
        throw std::runtime_error("Failed to create vertex shader");
    }
}

void VertexShader::Bind() noexcept {
    GfxDevice::context->VSSetShader(vertexShader.Get(), nullptr, 0);
}

ID3DBlob* VertexShader::GetBytecode() const noexcept {
    return bytecodeBlob.Get();
}
