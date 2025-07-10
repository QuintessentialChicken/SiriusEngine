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

    static void Draw();

    static void DrawIndexed(UINT count);

    static void Shutdown();

    static void ResizeViewport(int width, int height);


    static std::unique_ptr<IShader> CreateShader(ShaderType type, const std::string& path);
    static std::unique_ptr<IInputLayout> CreateInputLayout(const std::vector<InputLayoutElement>& elements, const void* shaderBytecode, size_t bytecodeSize);
    static std::unique_ptr<IPipelineState> CreatePipelineState(const PipelineStateDesc& desc);
    static std::unique_ptr<IVertexBuffer> CreateVertexBuffer(const void* data, size_t size, UINT stride);
    static std::unique_ptr<IIndexBuffer> CreateIndexBuffer(const void* indices, size_t size);
    static std::unique_ptr<IConstantBuffer> CreateConstantBuffer(const void* data, size_t size);

    static DirectX::XMMATRIX GetProjection();


private:
    static std::unique_ptr<IRenderApi> renderApi;
};


#endif //RENDERER_H
