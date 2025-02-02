//
// Created by Leon on 02/02/2025.
//

#ifndef BINDABLE_H
#define BINDABLE_H
#include "Graphics/Graphics.h"


class Bindable {
public:
    virtual void Bind(Graphics& gfx) noexcept = 0;

    virtual ~Bindable() = default;

protected:
    static ID3D11DeviceContext* GetContext(Graphics& gfx) noexcept;

    static ID3D11Device* GetDevice(Graphics& gfx) noexcept;

    static DxgiInfoManager& GetInfoManager(Graphics& gfx) noexcept;
};

#endif //BINDABLE_H
