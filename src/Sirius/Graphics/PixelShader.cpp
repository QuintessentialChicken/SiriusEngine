//
// Created by Leon on 02/02/2025.
//

#include <d3dcompiler.h>

#include "PixelShader.h"

PixelShader::PixelShader(const std::wstring& path) {
    Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
    if (FAILED(D3DReadFileToBlob( path.c_str(),&pBlob))) {
        throw std::runtime_error("Failed to read pixel shader");
    }
    if FAILED(GfxDevice::device->CreatePixelShader( pBlob->GetBufferPointer(),pBlob->GetBufferSize(),nullptr,&pixelShader)) {
        throw std::runtime_error("Failed to create pixel shader");
    }
}

void PixelShader::Bind() noexcept {
    GfxDevice::context->PSSetShader( pixelShader.Get(),nullptr,0u );
}