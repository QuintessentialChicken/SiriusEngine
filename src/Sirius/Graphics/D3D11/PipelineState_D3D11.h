//
// Created by Leon on 11/05/2025.
//

#ifndef PIPELINESTATE_D3D11_H
#define PIPELINESTATE_D3D11_H
#include "Graphics/PipelineState.h"


class PipelineState_D3D11 : public IPipelineState {
public:
    PipelineState_D3D11(const PipelineStateDesc& desc, ID3D11DeviceContext* context);

    void Bind() override;

private:
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
    PipelineStateDesc desc;

    // Helper to convert API-agnostic topology to D3D11 topology
    static D3D11_PRIMITIVE_TOPOLOGY GetD3D11Topology(PrimitiveTopology topology);
};


#endif //PIPELINESTATE_D3D11_H
