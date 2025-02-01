//
// Created by Lms on 24/01/2025.
//


#include <array>
#include <sstream>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "Graphics.h"

#include <algorithm>
#include <ranges>

#include "System/dxerr.h"
namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

// graphics exception checking/throwing macros (some with dxgi infos)
#define GFX_EXCEPT_NOINFO(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HrException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw Graphics::InfoException( __LINE__,__FILE__,v);}}
#else
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO_ONLY(call) (call)
#endif

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
    // if (!info.empty()) {
    //     oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
    // }
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

    UINT swapCreateFlags = 0;
#ifndef NDEBUG
    swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    HRESULT hr;
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

    wrl::ComPtr<ID3D11Resource> backBuffer = nullptr;
    GFX_THROW_INFO(swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
    GFX_THROW_INFO(device->CreateRenderTargetView(backBuffer.Get(), nullptr, &target)); // Doesn't catch Access violation reading location when given a nullptr as resource
}

void Graphics::EndFrame() {
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

void Graphics::ClearBuffer(float r, float g, float b) noexcept {
    std::array<const float, 4> color = {r, g, b, 1.0f};
    context->ClearRenderTargetView(target.Get(), color.data());
}

void Graphics::DrawTestTriangle(float angle, float x, float y) {
    HRESULT hr;

    struct Vertex {
        struct {
            float x, y, z;
        } pos;

        struct {
            unsigned char r, g, b, a;
        } color;
    };

    constexpr Vertex vertices[] = {
        {1.0f, 1.0f, -1.0f, 255, 0, 0, 0},
        {-1.0f, 1.0f, -1.0f, 0, 255, 0, 0},
        {1.0f, -1.0f, -1.0f, 0, 0, 255, 0},
        {-1.0f, -1.0f, -1.0f, 255, 255, 0, 0},
        {1.0f, 1.0f, 1.0f, 255, 0, 255, 0},
        {-1.0f, 1.0f, 1.0f, 0, 255, 255, 0},
        {1.0f, -1.0f, 1.0f, 0, 0, 0, 0},
        {-1.0f, -1.0f, 1.0f, 255, 255, 255, 0}
    };

    wrl::ComPtr<ID3D11Buffer> vertexBuffer = nullptr;
    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(vertices);
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    bd.StructureByteStride = sizeof(Vertex);
    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices;
    GFX_THROW_INFO(device->CreateBuffer(&bd, &sd, &vertexBuffer));
    constexpr UINT stride = sizeof(Vertex);
    constexpr UINT offset = 0;
    context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

    // Create Index Buffer
    const unsigned short indices[] = {
        0, 2, 1,  2, 3, 1,
        1, 3, 5,  3, 7, 5,
        2, 6, 3,  3, 6, 7,
        4, 5, 7,  4, 7, 6,
        0, 4, 2,  2, 4, 6,
        0, 1, 4,  1, 5, 4
    };
    wrl::ComPtr<ID3D11Buffer> indexBuffer = nullptr;
    D3D11_BUFFER_DESC ibd = {};
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.ByteWidth = sizeof(indices);
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    ibd.StructureByteStride = sizeof(unsigned short);
    D3D11_SUBRESOURCE_DATA isd = {};
    isd.pSysMem = indices;
    GFX_THROW_INFO(device->CreateBuffer(&ibd, &isd, &indexBuffer));
    // Bind Index Buffer
    context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

    // Create Constant Buffer for Transformation Matrix
    struct ConstantBuffer {
        dx::XMMATRIX transform;
    };
    const ConstantBuffer cb = {
        dx::XMMatrixTranspose(
            dx::XMMatrixRotationZ(angle) *
            dx::XMMatrixRotationX(angle) *
            dx::XMMatrixTranslation(x, y, 4.0f) *
            dx::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f)
        )

    };

    wrl::ComPtr<ID3D11Buffer> constantBuffer = nullptr;
    D3D11_BUFFER_DESC cbd;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.ByteWidth = sizeof(cb);
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbd.MiscFlags = 0;
    cbd.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA csd = {};
    csd.pSysMem = &cb;
    GFX_THROW_INFO(device->CreateBuffer(&cbd, &csd, &constantBuffer));

    //Bind Constant Buffer to Vertex Shader
    context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

    // Create PixelShader
    wrl::ComPtr<ID3D11PixelShader> pixelShader;
    wrl::ComPtr<ID3DBlob> blob;
    GFX_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &blob));
    GFX_THROW_INFO(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader));
    // Bind PixelShader
    context->PSSetShader(pixelShader.Get(), nullptr, 0);

    // Create VertexShader
    wrl::ComPtr<ID3D11VertexShader> vertexShader;
    GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &blob));
    GFX_THROW_INFO(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader));
    // Bind VertexShader
    context->VSSetShader(vertexShader.Get(), nullptr, 0);


    wrl::ComPtr<ID3D11InputLayout> inputLayout;
    constexpr D3D11_INPUT_ELEMENT_DESC ied[] = {
        {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    GFX_THROW_INFO(device->CreateInputLayout(
        ied,
        std::size(ied),
        blob->GetBufferPointer(),
        blob->GetBufferSize(),
        &inputLayout
    ));

    context->IASetInputLayout(inputLayout.Get());

    context->OMSetRenderTargets(1, target.GetAddressOf(), nullptr);

    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D11_VIEWPORT vp;
    vp.Width = 800;
    vp.Height = 600;
    vp.MinDepth = 0;
    vp.MaxDepth = 1;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    context->RSSetViewports(1, &vp);


    GFX_THROW_INFO_ONLY(context->DrawIndexed(std::size(indices), 0, 0));
}
