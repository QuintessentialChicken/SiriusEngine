//
// Created by Leon on 10/05/2025.
//

#ifndef RENDERAPI_H
#define RENDERAPI_H
#include <DirectXMath.h>
#include <intsafe.h>
#include <memory>
#include <vector>

#include "Buffer.h"
#include "InputLayout.h"
#include "PipelineState.h"
#include "Shader.h"

class IRenderApi {
public:
    virtual ~IRenderApi() = default;

    static std::unique_ptr<IRenderApi> Create();
    virtual void Init() = 0;
    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;
    virtual void Draw() = 0;
    virtual void DrawIndexed(UINT count) = 0;
    virtual void Shutdown() = 0;
    virtual void ResizeViewport(int width, int height) = 0;

    virtual std::unique_ptr<IShader> CreateShader(ShaderType type, const std::string& path) = 0;
    virtual std::unique_ptr<IInputLayout> CreateInputLayout(const std::vector<InputLayoutElement>& elements, const void* shaderBytecode, size_t bytecodeSize) = 0;
    virtual std::unique_ptr<IPipelineState> CreatePipelineState(const PipelineStateDesc& desc) = 0;
    virtual std::unique_ptr<IVertexBuffer> CreateVertexBuffer(const void* data, size_t size, UINT stride) = 0;
    virtual std::unique_ptr<IIndexBuffer> CreateIndexBuffer(const void* indices, size_t size) = 0;
    virtual std::unique_ptr<IConstantBuffer> CreateConstantBuffer(const void* data, size_t size) = 0;

    [[nodiscard]] virtual DirectX::XMMATRIX GetProjection() const noexcept = 0;


    enum class Api {
        D3D11,
        Vulkan
    };
private:
    static Api api;
};



#endif //RENDERAPI_H
