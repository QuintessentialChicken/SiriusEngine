//
// Created by Leon on 26/04/2025.
//
#include <array>

#include "GfxDevice.h"
#include "External/imgui_impl_dx11.h"
#include "Core/WndProc.h"
#include "External/imgui_impl_win32.h"


GfxDevice* pGfxDevice;
std::array<float, 4> GfxDevice::color = {0.0f, 0.3f, 0.0f, 1.0f};
DirectX::XMMATRIX GfxDevice::projection;
DirectX::XMMATRIX GfxDevice::camera;
Microsoft::WRL::ComPtr<IDXGISwapChain> GfxDevice::swapChain;
Microsoft::WRL::ComPtr<ID3D11Device> GfxDevice::device;
Microsoft::WRL::ComPtr<ID3D11DeviceContext> GfxDevice::context;
Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GfxDevice::target;
Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GfxDevice::DSV;

void GfxDevice::InitClass() {
    if (!hwndMain) {
        hwndMain = CreateDeviceWindow();
    }


    DXGI_SWAP_CHAIN_DESC sd = {};
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
    sd.OutputWindow = hwndMain;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;

    UINT swapCreateFlags = 0u;
#ifndef NDEBUG
    swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // for checking results of d3d functions
    HRESULT hr;

    // create device and front/back buffers, and swap chain and rendering context
    D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        swapCreateFlags,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &swapChain,
        &device,
        nullptr,
        &context
    );

    // gain access to texture subresource in swap chain (back buffer)
    Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;
    swapChain->GetBuffer( 0,__uuidof(ID3D11Resource),&backBuffer );
    device->CreateRenderTargetView( backBuffer.Get(),nullptr,&target );

    // create depth stensil state
    D3D11_DEPTH_STENCIL_DESC dsDesc = {};
    dsDesc.DepthEnable = TRUE;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DSState;
    device->CreateDepthStencilState( &dsDesc,&DSState );

    // bind depth state
    context->OMSetDepthStencilState(DSState.Get(), 1u);

    UINT screenWidth = 800;
    UINT screenHeight = 600;
    // create depth stensil texture
    Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencil;
    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = screenWidth;
    depthDesc.Height = screenHeight;
    depthDesc.MipLevels = 1u;
    depthDesc.ArraySize = 1u;
    depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthDesc.SampleDesc.Count = 1u;
    depthDesc.SampleDesc.Quality = 0u;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    device->CreateTexture2D( &depthDesc,nullptr,&depthStencil );

    // create view of depth stensil texture
    D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc = {};
    DSVDesc.Format = DXGI_FORMAT_D32_FLOAT;
    DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    DSVDesc.Texture2D.MipSlice = 0u;
    device->CreateDepthStencilView(depthStencil.Get(),&DSVDesc,&DSV);

    // bind depth stensil view to OM
    context->OMSetRenderTargets(1u, target.GetAddressOf(), DSV.Get());

    // GFX_THROW_INFO(swapChain->SetFullscreenState( TRUE, nullptr ));
    D3D11_VIEWPORT vp;
    vp.Width = 800;
    vp.Height = 600;
    vp.MinDepth = 0;
    vp.MaxDepth = 1;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    context->RSSetViewports(1, &vp);

    ImGui::CreateContext();
    ImGui_ImplDX11_Init(device.Get(), context.Get());

    // float aspectRatio = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
    // float viewHeight = 5.0f;
    // float viewWidth = viewHeight * aspectRatio;
    projection = DirectX::XMMatrixPerspectiveLH(1, 3.0f/4.0f, 0.5f, 40.0f);

}

int GfxDevice::InitSingleton() {
    static GfxDevice deviceD3d11;
    pGfxDevice = &deviceD3d11;
    return 1100;
}

void GfxDevice::ShutdownClass() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    if (hwndMain) {
        DestroyWindow(hwndMain);
        context.Reset();
        swapChain.Reset();
        target.Reset();
        DSV.Reset();
        device.Reset();
        hwndMain = nullptr;
        winClass = 0;
    }
}

void GfxDevice::BeginFrame() {
    if( true )
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    context->ClearRenderTargetView(GfxDevice::target.Get(), color.data());
    context->ClearDepthStencilView(GfxDevice::DSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void GfxDevice::EndFrame() {
    if (true) {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    if (HRESULT hr; FAILED(hr = GfxDevice::swapChain->Present( 1u,0u ))) {
        throw std::runtime_error("Failed to present swap chain");
    }
}

void GfxDevice::DrawIndexed(const UINT count) {
    context->DrawIndexed(count, 0, 0);
}

