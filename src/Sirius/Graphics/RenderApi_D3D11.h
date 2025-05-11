//
// Created by Leon on 10/05/2025.
//

#ifndef RENDERDEVICE_D3D11_H
#define RENDERDEVICE_D3D11_H
#include <array>
#include <dxgi.h>
#include <wrl/client.h>

#include "GfxDevice.h"
#include "InputLayout.h"
#include "PipelineState.h"
#include "RenderApi.h"
#include "Shader.h"


class RenderApi_D3D11 : public IRenderApi {
public:
    void Init() override;

    void BeginFrame() override;

    void EndFrame() override;

    void DrawIndexed(UINT count) override;

    void Shutdown() override;


    std::unique_ptr<IShader> CreateShader(ShaderType type, const std::wstring& path) override;

    std::unique_ptr<IInputLayout> CreateInputLayout(const std::vector<InputLayoutElement>& elements, const void* shaderBytecode, size_t bytecodeSize) override;

    std::unique_ptr<IPipelineState> CreatePipelineState(const PipelineStateDesc& desc) override;

    std::unique_ptr<IVertexBuffer> CreateVertexBuffer(const void* data, size_t size, UINT stride) override;

    std::unique_ptr<IIndexBuffer> CreateIndexBuffer(const void* indices, size_t size) override;

    std::unique_ptr<IConstantBuffer> CreateConstantBuffer(const void* data, size_t size) override;

    [[nodiscard]] DirectX::XMMATRIX GetProjection() const noexcept override;

private:
    std::array<float, 4> color = {0.3f, 0.0f, 0.3f, 1.0f};
    DirectX::XMMATRIX projection = {};
    DirectX::XMMATRIX camera = {};
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> target;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DSV;
};


#endif //RENDERDEVICE_D3D11_H
