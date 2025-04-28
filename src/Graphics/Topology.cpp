//
// Created by Leon on 02/02/2025.
//

#include "Topology.h"

#include <d3d11.h>

#include "GfxDevice.h"

Topology::Topology(const D3D11_PRIMITIVE_TOPOLOGY type) : type{type} {}

void Topology::Bind() noexcept {
    GfxDevice::context->IASetPrimitiveTopology( type );
}
