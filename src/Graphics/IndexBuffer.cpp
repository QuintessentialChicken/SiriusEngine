//
// Created by Leon on 02/02/2025.
//

#include "IndexBuffer.h"

#include "GfxDevice.h"

IndexBuffer::IndexBuffer(const std::vector<unsigned short>& indices): count(indices.size()) {
    D3D11_BUFFER_DESC ibd = {};
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.ByteWidth = static_cast<UINT>(count * sizeof(unsigned short));
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    ibd.StructureByteStride = sizeof(unsigned short);
    D3D11_SUBRESOURCE_DATA isd = {};
    isd.pSysMem = indices.data();
    if (FAILED(GfxDevice::device->CreateBuffer(&ibd, &isd, &indexBuffer))) {
        throw std::runtime_error("Failed to create index buffer");
    }
}

void IndexBuffer::Bind() noexcept {
    GfxDevice::context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}

UINT IndexBuffer::GetCount() const noexcept {
    return count;
}
