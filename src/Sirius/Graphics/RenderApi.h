//
// Created by Leon on 10/05/2025.
//

#ifndef RENDERAPI_H
#define RENDERAPI_H
#include <intsafe.h>
#include <memory>

#include "Buffer.h"

class ID3D11Device;
class ID3D11DeviceContext;

class IRenderApi {
public:
    virtual ~IRenderApi() = default;

    static std::unique_ptr<IRenderApi> Create();
    virtual void Init() = 0;
    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;
    virtual void DrawIndexed(UINT count) = 0;
    virtual void Shutdown() = 0;

    virtual std::unique_ptr<IVertexBuffer> CreateVertexBuffer(const void* data, size_t size, UINT stride) = 0;
    virtual std::unique_ptr<IIndexBuffer> CreateIndexBuffer(const void* indices, size_t size) = 0;
    virtual std::unique_ptr<IConstantBuffer> CreateConstantBuffer(const void* data, size_t size) = 0;


    enum class Api {
        D3D11,
        Vulkan
    };
private:
    static Api api;
};



#endif //RENDERAPI_H
