//
// Created by Leon on 11/05/2025.
//

#include "PipelineState_D3D11.h"

PipelineState_D3D11::PipelineState_D3D11(const PipelineStateDesc& desc, ID3D11DeviceContext* context)
: context{context}, desc{desc} {
    // D3D11 doesn't have a monolithic pipeline state object
    // State objects are bound separately
}

void PipelineState_D3D11::Bind() {
    context->IASetPrimitiveTopology(GetD3D11Topology(desc.topology));

    // Set input layout
    if (desc.inputLayout) {
        desc.inputLayout->Bind();
    }

    // Set shaders
    if (desc.vertexShader) {
        desc.vertexShader->Bind();
    }

    if (desc.pixelShader) {
        desc.pixelShader->Bind();
    }

    if (desc.geometryShader) {
        desc.geometryShader->Bind();
    }

    if (desc.hullShader) {
        desc.hullShader->Bind();
    }

    if (desc.domainShader) {
        desc.domainShader->Bind();
    }

}

D3D11_PRIMITIVE_TOPOLOGY PipelineState_D3D11::GetD3D11Topology(const PrimitiveTopology topology) {
    switch (topology) {
        case PrimitiveTopology::PointList:     return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
        case PrimitiveTopology::LineList:      return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
        case PrimitiveTopology::LineStrip:     return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
        case PrimitiveTopology::TriangleList:  return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        case PrimitiveTopology::TriangleStrip: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
        default:
            return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    }
}
