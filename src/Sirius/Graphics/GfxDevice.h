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

private:
};

extern GfxDevice* pGfxDevice;


#endif //DEVICE_H
