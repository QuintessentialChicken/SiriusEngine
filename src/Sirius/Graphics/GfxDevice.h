//
// Created by Leon on 26/04/2025.
//

#ifndef DEVICE_H
#define DEVICE_H
#include <complex>
#include <d3d11.h>
#include <DirectXMath.h>
#include <dxgi.h>
#include <wrl.h>
#include "Core/SiriusException.h"


class GfxDevice {

public:
    static HWND CreateDeviceWindow();
    static void SetWindowTitle(const std::string& title);
    static void SetBackgroundColor(std::array<float, 4> color);

    static std::array<float, 4> color;
    static DirectX::XMMATRIX projection;
    static DirectX::XMMATRIX camera;
    static Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
    static Microsoft::WRL::ComPtr<ID3D11Device> device;
    static Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
    static Microsoft::WRL::ComPtr<ID3D11RenderTargetView> target;
    static Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DSV;




    class Exception : public SiriusException {
        using SiriusException::SiriusException;

    public:
        static std::string TranslateErrorCode(HRESULT hr) noexcept;
    };

    class HrException : public Exception {
    public:
        HrException(int line, const char *file, HRESULT hr) noexcept;

        const char *what() const noexcept override;

        const char *GetType() const noexcept override;

        HRESULT GetErrorCode() const noexcept;

        std::string GetErrorDescription() const noexcept;

    private:
        HRESULT hr;
    };

    class NoGfxException : public Exception {
    public:
        using Exception::Exception;

        const char *GetType() const noexcept override;
    };
};

extern GfxDevice* pGfxDevice;


#endif //DEVICE_H
