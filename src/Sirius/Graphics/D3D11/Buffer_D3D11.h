//
// Created by Leon on 10/05/2025.
//

#ifndef BUFFER_D3D11_H
#define BUFFER_D3D11_H
#include <wrl/client.h>
#include <d3d11.h>

#include "Graphics/Buffer.h"


// D3D11 implementations
class VertexBuffer_D3D11 : public IVertexBuffer {
public:
    VertexBuffer_D3D11(const void* data, size_t size, UINT stride, ID3D11Device* device, ID3D11DeviceContext* context);

    void Bind() override;

    void Update(const void* data, size_t size) override;

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
    UINT stride;
    // Other D3D11-specific members
};

class IndexBuffer_D3D11 : public IIndexBuffer {
public:
    IndexBuffer_D3D11(const void* data, size_t size, ID3D11Device* device, ID3D11DeviceContext* context);

    void Bind() override;

    [[nodiscard]] UINT GetCount() const override;

private:
    UINT count;
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
};

class ConstantBuffer_D3D11 : public IConstantBuffer {
public:
    ConstantBuffer_D3D11(UINT size, ID3D11Device* device, ID3D11DeviceContext* context);

    ConstantBuffer_D3D11(const void* data, size_t size, ID3D11Device* device, ID3D11DeviceContext* context);

    void Bind(ShaderStage stages, UINT slot) override;

    void Update(const void* data, size_t size) override;

private:
    size_t bufferSize;
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
};


#endif //BUFFER_D3D11_H
