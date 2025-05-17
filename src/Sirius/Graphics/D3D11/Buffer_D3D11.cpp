//
// Created by Leon on 10/05/2025.
//

#include "Buffer_D3D11.h"

#include <stdexcept>

#include "RenderApi_D3D11.h"


/***************************** Vertex Buffer *****************************/

VertexBuffer_D3D11::VertexBuffer_D3D11(const void* data, size_t size, UINT stride, ID3D11Device* device, ID3D11DeviceContext* context)
    : device{device}, context{context}, stride{stride} {
    D3D11_BUFFER_DESC bd = {};
    bd.ByteWidth = size;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    bd.StructureByteStride = stride;
    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = data;
    if (FAILED(device->CreateBuffer(&bd, &sd, &vertexBuffer))) {
        throw std::runtime_error("Failed to create vertex buffer");
    }
}

void VertexBuffer_D3D11::Bind() {
    constexpr UINT offset = 0u;
    context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
}

void VertexBuffer_D3D11::Update(const void* data, size_t size) {
    constexpr UINT offset = 0u;
    context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
}


/***************************** Index Buffer *****************************/

IndexBuffer_D3D11::IndexBuffer_D3D11(const void* data, size_t size, ID3D11Device* device, ID3D11DeviceContext* context)
    : count{static_cast<UINT>(size / sizeof(unsigned short))}, device{device}, context{context} {
    D3D11_BUFFER_DESC ibd = {};
    ibd.ByteWidth = size;
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    ibd.StructureByteStride = sizeof(unsigned short);
    D3D11_SUBRESOURCE_DATA isd = {};
    isd.pSysMem = data;
    if (FAILED(device->CreateBuffer(&ibd, &isd, &indexBuffer))) {
        throw std::runtime_error("Failed to create index buffer");
    }
}

void IndexBuffer_D3D11::Bind() {
    context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}

UINT IndexBuffer_D3D11::GetCount() const {
    return count;
}


/***************************** Constant Buffer *****************************/

ConstantBuffer_D3D11::ConstantBuffer_D3D11(UINT size, ID3D11Device* device, ID3D11DeviceContext* context)
    : bufferSize(size), device{device}, context{context} {
    D3D11_BUFFER_DESC cbd;
    cbd.ByteWidth = size;
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbd.MiscFlags = 0u;
    cbd.StructureByteStride = 0u;
    if (FAILED(device->CreateBuffer(&cbd, nullptr, &constantBuffer))) {
        throw std::runtime_error("Failed to create constant buffer");
    }
}

ConstantBuffer_D3D11::ConstantBuffer_D3D11(const void* data, size_t size, ID3D11Device* device, ID3D11DeviceContext* context)
    : bufferSize(size), device{device}, context{context} {
    D3D11_BUFFER_DESC cbd;
    cbd.ByteWidth = size;
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbd.MiscFlags = 0u;
    cbd.StructureByteStride = 0u;
    D3D11_SUBRESOURCE_DATA csd = {};
    csd.pSysMem = data;
    if (FAILED(device->CreateBuffer(&cbd, &csd, &constantBuffer))) {
        throw std::runtime_error("Failed to create constant buffer");
    }
}

void ConstantBuffer_D3D11::Bind(ShaderStage stages, UINT slot) {
    if ((stages & ShaderStage::Vertex) == ShaderStage::Vertex) {
        context->VSSetConstantBuffers(slot, 1, constantBuffer.GetAddressOf());
    }

    if ((stages & ShaderStage::Pixel) == ShaderStage::Pixel) {
        context->PSSetConstantBuffers(slot, 1, constantBuffer.GetAddressOf());
    }

    if ((stages & ShaderStage::Geometry) == ShaderStage::Geometry) {
        context->GSSetConstantBuffers(slot, 1, constantBuffer.GetAddressOf());
    }

    if ((stages & ShaderStage::Hull) == ShaderStage::Hull) {
        context->HSSetConstantBuffers(slot, 1, constantBuffer.GetAddressOf());
    }

    if ((stages & ShaderStage::Domain) == ShaderStage::Domain) {
        context->DSSetConstantBuffers(slot, 1, constantBuffer.GetAddressOf());
    }

    if ((stages & ShaderStage::Compute) == ShaderStage::Compute) {
        context->CSSetConstantBuffers(slot, 1, constantBuffer.GetAddressOf());
    }
}

void ConstantBuffer_D3D11::Update(const void* data, size_t size) {
    if (size > bufferSize) {
        throw std::runtime_error("Data size exceeds constant buffer size");
    }
    D3D11_MAPPED_SUBRESOURCE msr;
    if (FAILED(context->Map(constantBuffer.Get(),0u,D3D11_MAP_WRITE_DISCARD,0u, &msr))) {
        throw std::runtime_error("Failed to map constant buffer");
    }
    memcpy(msr.pData, data, size);
    context->Unmap(constantBuffer.Get(), 0u);
}