//
// Created by Leon on 02/02/2025.
//

#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H
#include <vector>

#include "Bindable.h"
#include "GfxDevice.h"


class VertexBuffer : public Bindable {
public:
    template<class V>
    explicit VertexBuffer(const std::vector<V>& vertices) : stride{sizeof(V)} {
        D3D11_BUFFER_DESC bd = {};
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(vertices) * vertices.size();
        bd.CPUAccessFlags = 0;
        bd.MiscFlags = 0;
        bd.StructureByteStride = sizeof(V);
        D3D11_SUBRESOURCE_DATA sd = {};
        sd.pSysMem = vertices.data();
        if (FAILED(GfxDevice::device->CreateBuffer(&bd, &sd, &vertexBuffer))) {
            throw std::runtime_error("Failed to create vertex buffer");
        }
    }
    void Bind() noexcept override {
        constexpr UINT offset = 0u;
        GfxDevice::context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    }

private:
    UINT stride;
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
};



#endif //VERTEXBUFFER_H
