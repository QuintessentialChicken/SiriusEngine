//
// Created by Leon on 02/02/2025.
//

#include "InputLayout.h"

#include "GfxDevice.h"


InputLayout::InputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode) {
    if (FAILED(GfxDevice::device->CreateInputLayout(
        layout.data(),
        static_cast<UINT>(layout.size()),
        pVertexShaderBytecode->GetBufferPointer(),
        pVertexShaderBytecode->GetBufferSize(),
        &inputLayout
    ))) {
        throw std::runtime_error("Failed to create input layout");
    }
}

void InputLayout::Bind() noexcept {
    GfxDevice::context->IASetInputLayout(inputLayout.Get());
}
