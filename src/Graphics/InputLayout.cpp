//
// Created by Leon on 02/02/2025.
//

#include "InputLayout.h"

#include "GraphicsThrowMacros.h"

InputLayout::InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode) {
    INFOMAN(gfx);

    GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout(
        layout.data(),static_cast<UINT>(layout.size()),
        pVertexShaderBytecode->GetBufferPointer(),
        pVertexShaderBytecode->GetBufferSize(),
        &inputLayout
    ));
}

void InputLayout::Bind(Graphics& gfx) noexcept {
    GetContext(gfx)->IASetInputLayout(inputLayout.Get());
}
