//
// Created by Leon on 02/02/2025.
//

#ifndef PIXELSHADER_H
#define PIXELSHADER_H

#include <string>
#include <wrl/client.h>

#include "Bindable.h"
#include "VertexShader.h"

class PixelShader : public Bindable {
public:
    explicit PixelShader(const std::wstring& path );
    void Bind() noexcept override;
protected:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
};



#endif //PIXELSHADER_H
