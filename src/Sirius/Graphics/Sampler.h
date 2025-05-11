//
// Created by Lms on 05/02/2025.
//

#ifndef SAMPLER_H
#define SAMPLER_H
#include <d3d11.h>
#include <wrl/client.h>

class Sampler
{
public:
    explicit Sampler();
    void Bind() noexcept;
protected:
    Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
};



#endif //SAMPLER_H
