//
// Created by Leon on 10/05/2025.
//

#include "RenderApi_D3D11.h"

#include <array>
#include <dxgi1_2.h>
#include <External/imgui_impl_dx11.h>
#include <External/imgui_impl_win32.h>

#include "Buffer_D3D11.h"
#include "InputLayout_D3D11.h"
#include "PipelineState_D3D11.h"
#include "Shader_D3D11.h"
#include "Core/dxerr.h"
#include "Graphics/GfxDevice.h"
#include "Graphics/WndProc.h"

#define GFX_EXCEPT_NOINFO(hr) RenderApi_D3D11::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw RenderApi_D3D11::HrException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define GFX_EXCEPT(hr) RenderApi_D3D11::HrException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) RenderApi_D3D11::DeviceRemovedException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#else
#define GFX_EXCEPT(hr) RenderApi_D3D11::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) RenderApi_D3D11::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#endif



void RenderApi_D3D11::Init() {
    if (!hwndMain) {
        hwndMain = GfxDevice::CreateDeviceWindow();
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

    // for checking results of d3d functions,
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
    swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);
    device->CreateRenderTargetView(backBuffer.Get(), nullptr, &target);

    // create depth stencil state
    D3D11_DEPTH_STENCIL_DESC dsDesc = {};
    dsDesc.DepthEnable = TRUE;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DSState;
    device->CreateDepthStencilState(&dsDesc, &DSState);

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
    device->CreateTexture2D(&depthDesc, nullptr, &depthStencil);

    // create view of depth stensil texture
    D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc = {};
    DSVDesc.Format = DXGI_FORMAT_D32_FLOAT;
    DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    DSVDesc.Texture2D.MipSlice = 0u;
    device->CreateDepthStencilView(depthStencil.Get(), &DSVDesc, &DSV);

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

    float aspectRatio = static_cast<float>(screenHeight) / static_cast<float>(screenWidth);
    projection = DirectX::XMMatrixPerspectiveLH(1.0f, aspectRatio, 0.5f, 40.0f);
}


void RenderApi_D3D11::BeginFrame() {
    if (true) {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    context->ClearRenderTargetView(target.Get(), color.data());
    context->ClearDepthStencilView(DSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
    Sleep(10);
}

void RenderApi_D3D11::EndFrame() {
    if (true) {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
    if (const HRESULT hr = swapChain->Present(0u, 0u); FAILED(hr)) {
        throw std::runtime_error("Failed to present swap chain");
    }
}

void RenderApi_D3D11::DrawIndexed(const UINT count) {
    context->DrawIndexed(count, 0, 0);
}

void RenderApi_D3D11::Shutdown() {
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

void RenderApi_D3D11::ResizeViewport(int width, int height) {
    if (!swapChain)
        return;
        
    // Unbind resources before resizing
    context->OMSetRenderTargets(0, nullptr, nullptr);
    
    // Release render target and depth stencil resources
    target.Reset();
    DSV.Reset();
    
    HRESULT hr;
    // Resize swap chain buffers
    hr = swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
    if (FAILED(hr)) {
        throw HrException(__LINE__, __FILE__, hr);
    }
    
    // Recreate render target view
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
    if (FAILED(hr)) {
        throw HrException(__LINE__, __FILE__, hr);
    }
    
    hr = device->CreateRenderTargetView(backBuffer.Get(), nullptr, target.GetAddressOf());
    if (FAILED(hr)) {
        throw HrException(__LINE__, __FILE__, hr);
    }
    
    // Recreate depth stencil texture and view
    Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencil;
    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = width;
    depthDesc.Height = height;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.SampleDesc.Quality = 0;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    
    hr = device->CreateTexture2D(&depthDesc, nullptr, depthStencil.GetAddressOf());
    if (FAILED(hr)) {
        throw HrException(__LINE__, __FILE__, hr);
    }
    
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;
    
    hr = device->CreateDepthStencilView(depthStencil.Get(), &dsvDesc, DSV.GetAddressOf());
    if (FAILED(hr)) {
        throw RenderApi_D3D11::HrException(__LINE__, __FILE__, hr);
    }
    
    // Rebind render targets
    context->OMSetRenderTargets(1, target.GetAddressOf(), DSV.Get());
    
    // Set up the viewport
    D3D11_VIEWPORT vp;
    vp.Width = static_cast<float>(width);
    vp.Height = static_cast<float>(height);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    context->RSSetViewports(1, &vp);
    
    // Update projection matrix for new aspect ratio
    float aspectRatio = static_cast<float>(height) / static_cast<float>(width);
    projection = DirectX::XMMatrixPerspectiveLH(1.0f, aspectRatio, 0.5f, 40.0f);
}

std::unique_ptr<IShader> RenderApi_D3D11::CreateShader(ShaderType type, const std::wstring& path) {
    return std::make_unique<Shader_D3D11>(type, path, device.Get(), context.Get());
}

std::unique_ptr<IInputLayout> RenderApi_D3D11::CreateInputLayout(const std::vector<InputLayoutElement>& elements, const void* shaderBytecode, size_t bytecodeSize) {
    return std::make_unique<InputLayout_D3D11>(elements, shaderBytecode, bytecodeSize, device.Get(), context.Get());
}

std::unique_ptr<IPipelineState> RenderApi_D3D11::CreatePipelineState(const PipelineStateDesc& desc) {
    return std::make_unique<PipelineState_D3D11>(desc, context.Get());
}

std::unique_ptr<IVertexBuffer> RenderApi_D3D11::CreateVertexBuffer(const void* data, size_t size, UINT stride) {
    return std::make_unique<VertexBuffer_D3D11>(data, size, stride, device.Get(), context.Get());
}

std::unique_ptr<IIndexBuffer> RenderApi_D3D11::CreateIndexBuffer(const void* indices, size_t size) {
    return std::make_unique<IndexBuffer_D3D11>(indices, size, device.Get(), context.Get());
}

std::unique_ptr<IConstantBuffer> RenderApi_D3D11::CreateConstantBuffer(const void* data, size_t size) {
    if (data == nullptr) {
        return std::make_unique<ConstantBuffer_D3D11>(static_cast<UINT>(size), device.Get(), context.Get());
    }
    return std::make_unique<ConstantBuffer_D3D11>(data, size, device.Get(), context.Get());
}

DirectX::XMMATRIX RenderApi_D3D11::GetProjection() const noexcept {
    return projection;
}


RenderApi_D3D11::HrException::HrException(int line, const char* file, HRESULT hr,
                                   const std::vector<std::string>& infoMsgs) noexcept : Exception{line, file}, hr{hr} {
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

const char* RenderApi_D3D11::HrException::what() const noexcept {
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

const char* RenderApi_D3D11::HrException::GetType() const noexcept {
    return "Sirius Graphics Exception";
}

HRESULT RenderApi_D3D11::HrException::GetErrorCode() const noexcept {
    return hr;
}

std::string RenderApi_D3D11::HrException::GetErrorString() const noexcept {
    return DXGetErrorString(hr);
}

std::string RenderApi_D3D11::HrException::GetErrorDescription() const noexcept {
    char buf[512];
    DXGetErrorDescription(hr, buf, sizeof(buf));
    return buf;
}

std::string RenderApi_D3D11::HrException::GetErrorInfo() const noexcept {
    return info;
}


const char* RenderApi_D3D11::DeviceRemovedException::GetType() const noexcept {
    return "Sirius Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}