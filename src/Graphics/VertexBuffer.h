//
// Created by Leon on 02/02/2025.
//

#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H
#include "Bindable.h"
#include "GraphicsThrowMacros.h"


class VertexBuffer : public Bindable {
public:
    template<class V>
    VertexBuffer(const std::vector<V>& vertices) : stride{sizeof(V)} {
        ID3D11Device* gfx = Graphics::GetDevice();
        D3D11_BUFFER_DESC bd = {};
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(vertices) * vertices.size();
        bd.CPUAccessFlags = 0;
        bd.MiscFlags = 0;
        bd.StructureByteStride = sizeof(V);
        D3D11_SUBRESOURCE_DATA sd = {};
        sd.pSysMem = vertices.data();
        gfx->CreateBuffer(&bd, &sd, &vertexBuffer);
    }
    void Bind(Graphics& gfx) noexcept override {
        const UINT offset = 0u;
        GetContext(gfx)->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    }

private:
    UINT stride;
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
};



#endif //VERTEXBUFFER_H
