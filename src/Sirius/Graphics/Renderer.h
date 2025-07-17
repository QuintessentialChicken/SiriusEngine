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

class RenderApi_Vulkan;
class Model;
class Shader_Vulkan;
class PipelineState_Vulkan;
class PipelineStateDesc;
class VertexBuffer_Vulkan;
class IndexBuffer_Vulkan;
class ConstantBuffer_Vulkan;
class ShaderType;

class Renderer {
public:
    static void Init();

    static void BeginFrame();

    static void EndFrame();

    static void Draw(const std::vector<Model>& model);

    static void DrawIndexed(UINT count);

    static void Shutdown();

    static void ResizeViewport(int width, int height);

    static Shader_Vulkan CreateShader(ShaderType type, const std::string& path);
    static PipelineState_Vulkan CreatePipelineState(const PipelineStateDesc& desc);
    static VertexBuffer_Vulkan CreateVertexBuffer(const void* data, size_t size, UINT stride);
    static IndexBuffer_Vulkan CreateIndexBuffer(const void* indices, size_t size);
    static ConstantBuffer_Vulkan CreateConstantBuffer(const void* data, size_t size);

    static DirectX::XMMATRIX GetProjection();


private:
    static std::unique_ptr<RenderApi_Vulkan> renderApi;
};


#endif //RENDERER_H
