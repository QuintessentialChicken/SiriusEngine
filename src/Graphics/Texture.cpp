//
// Created by Lms on 05/02/2025.
//

#include "Texture.h"

#include <d3d11.h>

#include "GfxDevice.h"
#include "Surface.h"

namespace wrl = Microsoft::WRL;

Texture::Texture(const Surface& s )
{
    // create texture resource
    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = s.GetWidth();
    textureDesc.Height = s.GetHeight();
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = s.GetBufferPtr();
    sd.SysMemPitch = s.GetWidth() * sizeof( Surface::Color );
    wrl::ComPtr<ID3D11Texture2D> pTexture;
    if (FAILED(GfxDevice::device->CreateTexture2D(&textureDesc,&sd,&pTexture
    ))) { throw std::runtime_error("Failed to create texture");}

    // create the resource view on the texture
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;
    if (FAILED(GfxDevice::device->CreateShaderResourceView(pTexture.Get(),&srvDesc,&pTextureView))) { throw std::runtime_error("Failed to create texture view");}
}

void Texture::Bind() noexcept
{
    GfxDevice::context->PSSetShaderResources( 0u,1u,pTextureView.GetAddressOf() );
}