//
// Created by Leon on 02/02/2025.
//

#ifndef INPUTLAYOUT_H
#define INPUTLAYOUT_H
#include "Bindable.h"


class InputLayout : public Bindable {
public:
    InputLayout( Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode );
    void Bind( Graphics& gfx ) noexcept override;
protected:
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};



#endif //INPUTLAYOUT_H
