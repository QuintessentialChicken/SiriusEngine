//
// Created by Lms on 24/01/2025.
//


#include <array>
#include <sstream>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <ranges>

#include "Graphics.h"
#include "Core/dxerr.h"
#include "External/imgui_impl_dx11.h"
#include "External/imgui_impl_win32.h"
#include "Graphics/GraphicsThrowMacros.h"
namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

Graphics::Graphics(HWND hWnd) {
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
    sd.OutputWindow = hWnd;
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
    GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
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
    ));

    // gain access to texture subresource in swap chain (back buffer)
    wrl::ComPtr<ID3D11Resource> backBuffer;
    GFX_THROW_INFO(swapChain->GetBuffer( 0,__uuidof(ID3D11Resource),&backBuffer ));
    GFX_THROW_INFO(device->CreateRenderTargetView( backBuffer.Get(),nullptr,&target ));

    // create depth stensil state
    D3D11_DEPTH_STENCIL_DESC dsDesc = {};
    dsDesc.DepthEnable = TRUE;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
    wrl::ComPtr<ID3D11DepthStencilState> DSState;
    GFX_THROW_INFO(device->CreateDepthStencilState( &dsDesc,&DSState ));

    // bind depth state
    context->OMSetDepthStencilState(DSState.Get(), 1u);

    // create depth stensil texture
    wrl::ComPtr<ID3D11Texture2D> depthStencil;
    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = 800u;
    depthDesc.Height = 600u;
    depthDesc.MipLevels = 1u;
    depthDesc.ArraySize = 1u;
    depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthDesc.SampleDesc.Count = 1u;
    depthDesc.SampleDesc.Quality = 0u;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    GFX_THROW_INFO(device->CreateTexture2D( &depthDesc,nullptr,&depthStencil ));

    // create view of depth stensil texture
    D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc = {};
    DSVDesc.Format = DXGI_FORMAT_D32_FLOAT;
    DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    DSVDesc.Texture2D.MipSlice = 0u;
    GFX_THROW_INFO(device->CreateDepthStencilView(depthStencil.Get(),&DSVDesc,&DSV));

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

    ImGui_ImplDX11_Init(device.Get(), context.Get());
}

Graphics::~Graphics() {
    ImGui_ImplDX11_Shutdown();
}

void Graphics::BeginFrame(float r, float g, float b) noexcept {
    if( imguiEnabled )
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    std::array<const float, 4> color = {r, g, b, 1.0f};
    context->ClearRenderTargetView(target.Get(), color.data());
    context->ClearDepthStencilView(DSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Graphics::EndFrame() {
    if (imguiEnabled) {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
#ifndef NDEBUG
    infoManager.Set();
#endif
    if (HRESULT hr; FAILED(hr = swapChain->Present( 1u,0u ))) {
        if (hr == DXGI_ERROR_DEVICE_REMOVED) {
            throw GFX_DEVICE_REMOVED_EXCEPT(device->GetDeviceRemovedReason());
        } else {
            throw GFX_EXCEPT(hr);
        }
    }
}

void Graphics::DrawIndexed(const UINT count) {
    GFX_THROW_INFO_ONLY(context->DrawIndexed(count, 0, 0));
}

void Graphics::SetProjection(DirectX::FXMMATRIX proj) noexcept {
    projection = proj;
}

DirectX::XMMATRIX Graphics::GetProjection() const noexcept {
    return projection;
}

void Graphics::SetCamera(DirectX::FXMMATRIX camera) noexcept {
    Graphics::camera = camera;
}

DirectX::XMMATRIX Graphics::GetCamera() const noexcept {
    return camera;
}

void Graphics::EnableImgui() noexcept {
    imguiEnabled = true;
}

void Graphics::DisableImgui() noexcept {
    imguiEnabled = false;
}

bool Graphics::isImguiEnabled() const noexcept {
    return imguiEnabled;
}

Graphics::HrException::HrException(int line, const char* file, HRESULT hr, const std::vector<std::string>& infoMsgs) noexcept : Exception{line, file}, hr{hr} {
    // join all info messages with newlines into single string
    for (const auto& m: infoMsgs) {
        info += m;
        info.push_back('\n');
    }
    // remove final newline if exists
    if (!info.empty()) {
        info.pop_back();
    }
}

const char* Graphics::HrException::what() const noexcept {
    std::ostringstream oss;
    oss << GetType() << std::endl
            << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
            << std::dec << " (" << static_cast<unsigned long>(GetErrorCode()) << ")" << std::endl
            << "[Error String] " << GetErrorString() << std::endl
            << "[Description] " << GetErrorDescription() << std::endl;
    if (!info.empty()) {
        oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
    }
    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const noexcept {
    return "Sirius Graphics Exception";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept {
    return hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept {
    return DXGetErrorString(hr);
}

std::string Graphics::HrException::GetErrorDescription() const noexcept {
    char buf[512];
    DXGetErrorDescription(hr, buf, sizeof(buf));
    return buf;
}

std::string Graphics::HrException::GetErrorInfo() const noexcept {
    return info;
}

Graphics::InfoException::InfoException(int line, const char* file, const std::vector<std::string>& infoMsgs) noexcept : Exception{line, file} {
    // join all info messages with newlines into single string
    for (const auto& m: infoMsgs) {
        info += m;
        info.push_back('\n');
    }
    // remove final newline if exists
    if (!info.empty()) {
        info.pop_back();
    }
}

const char* Graphics::InfoException::what() const noexcept {
    std::ostringstream oss;
    oss << GetType() << std::endl
            << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Graphics::InfoException::GetType() const noexcept {
    return "Sirius Graphics Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept {
    return info;
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept {
    return "Sirius Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}
