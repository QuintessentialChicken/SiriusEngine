//
// Created by Lms on 24/01/2025.
//

#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <d3d11.h>
#include <vector>
#include <wrl.h>
#include <DirectXMath.h>

#include "Core/SiriusException.h"
#include "Core/DxgiInfoManager.h"


class Graphics {
    friend class Bindable;

public:
    explicit Graphics(HWND hWnd);

    Graphics(const Graphics&) = delete;

    ~Graphics();

    Graphics& operator=(Graphics&) = delete;

    void BeginFrame(float r, float g, float b) noexcept;

    void EndFrame();

    void DrawIndexed(UINT count);

    void SetProjection(DirectX::FXMMATRIX proj) noexcept;

    [[nodiscard]] DirectX::XMMATRIX GetProjection() const noexcept;

    void SetCamera(DirectX::FXMMATRIX camera) noexcept;

    DirectX::XMMATRIX GetCamera() const noexcept;

    void EnableImgui() noexcept;

    void DisableImgui() noexcept;

    [[nodiscard]] bool isImguiEnabled() const noexcept;

private:
#ifndef NDEBUG
    DxgiInfoManager infoManager;
#endif
    DirectX::XMMATRIX projection;
    DirectX::XMMATRIX camera;
    bool imguiEnabled = true;
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> target;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DSV;

public:
    class Exception : public SiriusException {
        using SiriusException::SiriusException;
    };

    class HrException : public Exception {
    public:
        HrException(int line, const char* file, HRESULT hr, const std::vector<std::string>& infoMsgs = {}) noexcept;

        const char* what() const noexcept override;

        const char* GetType() const noexcept override;

        HRESULT GetErrorCode() const noexcept;

        std::string GetErrorString() const noexcept;

        std::string GetErrorDescription() const noexcept;

        std::string GetErrorInfo() const noexcept;

    private:
        HRESULT hr;
        std::string info;
    };

    class InfoException : public Exception {
    public:
        InfoException(int line, const char* file, const std::vector<std::string>& infoMsgs) noexcept;

        const char* what() const noexcept override;

        const char* GetType() const noexcept override;

        std::string GetErrorInfo() const noexcept;

    private:
        std::string info;
    };

    class DeviceRemovedException : public HrException {
        using HrException::HrException;

    public:
        const char* GetType() const noexcept override;

    private:
        std::string reason;
    };
};


#endif //GRAPHICS_H
