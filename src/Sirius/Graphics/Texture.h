//
// Created by Lms on 05/02/2025.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include <wrl/client.h>

#include "Bindable.h"
#include "PixelShader.h"
#include "Surface.h"

class Texture : public Bindable
{
public:
    explicit Texture(const Surface& s );
    void Bind() noexcept override;
protected:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
};



#endif //TEXTURE_H
