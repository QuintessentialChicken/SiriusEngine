//
// Created by Leon on 10/05/2025.
//

#ifndef RENDERER_H
#define RENDERER_H
#include <intsafe.h>
#include <memory>
#include "RenderApi.h"

class Renderer {
public:
    static void Init();

    static void BeginFrame();

    static void EndFrame();

    static void DrawIndexed(UINT count);

    static void Shutdown();

    static std::unique_ptr<IVertexBuffer> CreateVertexBuffer(const void* data, size_t size, UINT stride);
    static std::unique_ptr<IIndexBuffer> CreateIndexBuffer(const void* indices, size_t size);
    static std::unique_ptr<IConstantBuffer> CreateConstantBuffer(const void* data, size_t size);


private:
    static std::unique_ptr<IRenderApi> renderApi;
};


#endif //RENDERER_H
