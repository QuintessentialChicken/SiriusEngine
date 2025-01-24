//
// Created by Lms on 24/01/2025.
//

#include "Graphics.h"

#include <array>

Graphics::Graphics(HWND hWnd) {
    DXGI_SWAP_CHAIN_DESC sd;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.RefreshRate.Denominator = 0;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    sd.OutputWindow = hWnd;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;

    D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &swapChain,
        &device,
        nullptr,
        &context);

    ID3D11Resource* backBuffer = nullptr;
    swapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void **>(&backBuffer));
    device->CreateRenderTargetView(backBuffer, nullptr, &target);
    backBuffer->Release();
}

Graphics::~Graphics() {
    if (target != nullptr) {
        target->Release();
    }
    if (context != nullptr) {
        context->Release();
    }
    if (swapChain != nullptr) {
        swapChain->Release();
    }
    if (device != nullptr) {
        device->Release();
    }
}

void Graphics::EndFrame() {
    swapChain->Present(1, 0);
}

void Graphics::ClearBuffer(float r, float g, float b) noexcept {
    std::array<const float, 4> color = {r, g, b, 1.0f };
    context->ClearRenderTargetView(target, color.data());
}
