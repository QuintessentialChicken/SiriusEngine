//
// Created by Leon on 02/02/2025.
//

#include "Bindable.h"

ID3D11DeviceContext* Bindable::GetContext(Graphics& gfx) noexcept {
    return gfx.context.Get();
}

ID3D11Device* Bindable::GetDevice(Graphics& gfx) noexcept {
    return gfx.device.Get();
}

DxgiInfoManager& Bindable::GetInfoManager(Graphics& gfx) noexcept {
#ifndef NDEBUG
    return gfx.infoManager;
#else
    throw std::logic_error( "YouFuckedUp! (tried to access gfx.infoManager in Release config)" );
#endif
}
