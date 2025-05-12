//
// Created by Leon on 11/05/2025.
//

#ifndef INPUTLAYOUT_D3D11_H
#define INPUTLAYOUT_D3D11_H
#include <vector>

#include "Buffer_D3D11.h"
#include "Graphics/InputLayout.h"


// D3D11 implementation
class InputLayout_D3D11 : public IInputLayout {
public:
    InputLayout_D3D11(
        const std::vector<InputLayoutElement>& elements,
        const void* shaderBytecode,
        size_t bytecodeSize,
        ID3D11Device* device,
        ID3D11DeviceContext* context
    );

    void Bind() override;

private:
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

    // Helper to convert API-agnostic format to D3D11 format
    static DXGI_FORMAT GetDXGIFormat(DataFormat format);
};



#endif //INPUTLAYOUT_D3D11_H
