//
// Created by Lms on 05/02/2025.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include <wrl/client.h>

#include "Surface.h"

class Texture
{
public:
    explicit Texture(const Surface& s );
    void Bind() noexcept;
protected:
    // Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
};



#endif //TEXTURE_H
