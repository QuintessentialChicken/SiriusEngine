//
// Created by Leon on 02/02/2025.
//

#ifndef CONSTANTBUFFER_H
#define CONSTANTBUFFER_H
#include "Bindable.h"
#include "GfxDevice.h"

template<typename C>
class ConstantBuffer : public Bindable {
public:
    explicit ConstantBuffer() {
        D3D11_BUFFER_DESC cbd;
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbd.Usage = D3D11_USAGE_DYNAMIC;
        cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbd.MiscFlags = 0u;
        cbd.ByteWidth = sizeof(C);
        cbd.StructureByteStride = 0u;
        if (FAILED(GfxDevice::device->CreateBuffer( &cbd,nullptr,&constantBuffer ))) {
            throw std::runtime_error("Failed to create constant buffer");
        }
    }

    explicit ConstantBuffer(const C& consts) {
        D3D11_BUFFER_DESC cbd;
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbd.Usage = D3D11_USAGE_DYNAMIC;
        cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbd.MiscFlags = 0u;
        cbd.ByteWidth = sizeof(consts);
        cbd.StructureByteStride = 0u;

        D3D11_SUBRESOURCE_DATA csd = {};
        csd.pSysMem = &consts;
        if (FAILED(GfxDevice::device->CreateBuffer( &cbd,&csd,&constantBuffer ))) {
            throw std::runtime_error("Failed to create constant buffer");
        }
    }

    void Update(const C& consts) {
        D3D11_MAPPED_SUBRESOURCE msr;
        if (FAILED(GfxDevice::context->Map(constantBuffer.Get(),0u,D3D11_MAP_WRITE_DISCARD,0u, &msr))) {
            throw std::runtime_error("Failed to map constant buffer");
        }
        memcpy(msr.pData, &consts, sizeof(consts));
        GfxDevice::context->Unmap(constantBuffer.Get(), 0u);
    }

protected:
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C> {
    using ConstantBuffer<C>::constantBuffer;

public:
    using ConstantBuffer<C>::ConstantBuffer;

    void Bind() noexcept override {
        GfxDevice::context->VSSetConstantBuffers(0u, 1u, constantBuffer.GetAddressOf());
    }
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C> {
    using ConstantBuffer<C>::constantBuffer;

public:
    using ConstantBuffer<C>::ConstantBuffer;

    void Bind() noexcept override {
        GfxDevice::context->PSSetConstantBuffers(0u, 1u, constantBuffer.GetAddressOf());
    }
};

#endif //CONSTANTBUFFER_H
