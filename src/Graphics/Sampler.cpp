//
// Created by Lms on 05/02/2025.
//

#include "Sampler.h"

#include "GfxDevice.h"


Sampler::Sampler()
{
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

    if (FAILED(GfxDevice::device->CreateSamplerState( &samplerDesc,&pSampler))) {
        throw std::runtime_error("Failed to create sampler");
    }
}

void Sampler::Bind() noexcept
{
    GfxDevice::context->PSSetSamplers( 0,1,pSampler.GetAddressOf() );
}