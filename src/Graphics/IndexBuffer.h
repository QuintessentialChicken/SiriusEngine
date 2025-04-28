//
// Created by Leon on 02/02/2025.
//

#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H
#include <d3d11.h>
#include <vector>
#include <wrl/client.h>

#include "Bindable.h"


class IndexBuffer : public Bindable {
public:
    explicit IndexBuffer(const std::vector<unsigned short>& indices);

    void Bind() noexcept override;

    [[nodiscard]] UINT GetCount() const noexcept;

private:
    UINT count;
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
};


#endif //INDEXBUFFER_H
