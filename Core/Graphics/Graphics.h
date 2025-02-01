//
// Created by Lms on 24/01/2025.
//

#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <d3d11.h>
#include <vector>
#include <wrl.h>

#include "System/SiriusException.h"
#include "System/DxgiInfoManager.h"


class Graphics {
public:
    class Exception : public SiriusException {
        using SiriusException::SiriusException;
    };

    class HrException : public Exception {
    public:
        HrException(int line, const char *file, HRESULT hr, const std::vector<std::string>& infoMsgs = {}) noexcept;

        const char *what() const noexcept override;

        const char *GetType() const noexcept override;

        HRESULT GetErrorCode() const noexcept;

        std::string GetErrorString() const noexcept;

        std::string GetErrorDescription() const noexcept;

        std::string GetErrorInfo() const noexcept;

    private:
        HRESULT hr;
        std::string info;
    };
    class InfoException : public Exception
    {
    public:
        InfoException( int line,const char* file,const std::vector<std::string>& infoMsgs ) noexcept;
        const char* what() const noexcept override;
        const char* GetType() const noexcept override;
        std::string GetErrorInfo() const noexcept;
    private:
        std::string info;
    };
    class DeviceRemovedException : public HrException {
        using HrException::HrException;

    public:
        const char *GetType() const noexcept override;

    private:
        std::string reason;
    };

    explicit Graphics(HWND hWnd);

    Graphics(const Graphics &) = delete;

    Graphics &operator=(Graphics &) = delete;

    void EndFrame();

    void ClearBuffer(float r, float g, float b) noexcept;

    void DrawTestTriangle(float angle, float x, float y);

private:
#ifndef NDEBUG
    DxgiInfoManager infoManager;
#endif
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> target;
};


#endif //GRAPHICS_H
