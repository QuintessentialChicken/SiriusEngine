//
// Created by Lms on 05/02/2025.
//

#ifndef SAMPLER_H
#define SAMPLER_H
#include "Bindable.h"

class Sampler : public Bindable
{
public:
    explicit Sampler( Graphics& gfx );
    void Bind( Graphics& gfx ) noexcept override;
protected:
    Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
};



#endif //SAMPLER_H
