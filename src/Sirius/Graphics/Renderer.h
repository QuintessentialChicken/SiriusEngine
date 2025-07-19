//
// Created by Leon on 10/05/2025.
//

#ifndef RENDERER_H
#define RENDERER_H
#include <DirectXMath.h>
#include <intsafe.h>
#include <memory>
#include <string>
#include <vector>

#include "Buffer.h"
#include "PipelineState.h"
#include "RenderApi_Vulkan.h"


enum class ShaderType;
class Model;

class Renderer {
public:
    static void Init();

    static void BeginFrame();

    static void EndFrame();

    static void Draw(const std::vector<Model>& model);

    static void DrawIndexed(UINT count);

    static void Shutdown();

    static void ResizeViewport(int width, int height);

    static Shader CreateShader(ShaderType type, const std::string& path);
    static PipelineState CreatePipelineState(const PipelineStateDesc& desc);
    static VertexBuffer CreateVertexBuffer(const void* data, size_t size, UINT stride);
    static IndexBuffer CreateIndexBuffer(const void* indices, size_t size);
    static ConstantBuffer CreateConstantBuffer(const void* data, size_t size);

    static DirectX::XMMATRIX GetProjection();


private:
    static std::unique_ptr<RenderApi_Vulkan> renderApi;
};


#endif //RENDERER_H
