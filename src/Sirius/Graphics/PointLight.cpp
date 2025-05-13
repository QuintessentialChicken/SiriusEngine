//
// Created by Leon on 09/05/2025.
//

#include "PointLight.h"

void PointLight::Reset() noexcept
{
    pos = { 0.0f,0.0f,0.0f };
}

void PointLight::Init() {
}

void PointLight::Update() {
    cbufData.pos = pos;
    cbuf->Update(&cbufData, sizeof(cbufData) );
    cbuf->Bind(ShaderStage::Pixel, 0);
}
