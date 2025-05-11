//
// Created by Leon on 11/05/2025.
//

#include "InputLayout_D3D11.h"

#include <stdexcept>

InputLayout_D3D11::InputLayout_D3D11(const std::vector<InputLayoutElement>& elements, const void* shaderBytecode, size_t bytecodeSize, ID3D11Device* device, ID3D11DeviceContext* context) : context{context} {
    std::vector<D3D11_INPUT_ELEMENT_DESC> d3d11Elements;
    for (const auto& element : elements) {
        D3D11_INPUT_ELEMENT_DESC desc = {};
        desc.SemanticName = element.semanticName.c_str();
        desc.SemanticIndex = element.semanticIndex;
        desc.Format = GetDXGIFormat(element.format);
        desc.InputSlot = element.inputSlot;
        desc.AlignedByteOffset = element.alignedByteOffset;
        desc.InputSlotClass = element.perInstance
            ? D3D11_INPUT_PER_INSTANCE_DATA
            : D3D11_INPUT_PER_VERTEX_DATA;
        desc.InstanceDataStepRate = element.instanceDataStepRate;

        d3d11Elements.push_back(desc);
    }

    HRESULT hr = device->CreateInputLayout(
        d3d11Elements.data(),
        static_cast<UINT>(d3d11Elements.size()),
        shaderBytecode,
        bytecodeSize,
        &inputLayout
    );
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create input layout");
    }

}

void InputLayout_D3D11::Bind() {
    context->IASetInputLayout(inputLayout.Get());
}

DXGI_FORMAT InputLayout_D3D11::GetDXGIFormat(const DataFormat format) {
    switch (format) {
        case DataFormat::Float1: return DXGI_FORMAT_R32_FLOAT;
        case DataFormat::Float2: return DXGI_FORMAT_R32G32_FLOAT;
        case DataFormat::Float3: return DXGI_FORMAT_R32G32B32_FLOAT;
        case DataFormat::Float4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case DataFormat::Int1:   return DXGI_FORMAT_R32_SINT;
        case DataFormat::Int2:   return DXGI_FORMAT_R32G32_SINT;
        case DataFormat::Int3:   return DXGI_FORMAT_R32G32B32_SINT;
        case DataFormat::Int4:   return DXGI_FORMAT_R32G32B32A32_SINT;
        case DataFormat::UInt1:  return DXGI_FORMAT_R32_UINT;
        case DataFormat::UInt2:  return DXGI_FORMAT_R32G32_UINT;
        case DataFormat::UInt3:  return DXGI_FORMAT_R32G32B32_UINT;
        case DataFormat::UInt4:  return DXGI_FORMAT_R32G32B32A32_UINT;
        default:
            throw std::runtime_error("Unsupported data format");
    }
}
