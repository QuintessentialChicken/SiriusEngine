//
// Created by Leon on 02/02/2025.
//

#ifndef INPUTLAYOUT_H
#define INPUTLAYOUT_H
#include <d3d11.h>
#include <vector>
#include <wrl/client.h>

#include "Bindable.h"


class InputLayout : public Bindable {
public:
    InputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode );
    void Bind() noexcept override;
protected:
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};



#endif //INPUTLAYOUT_H
