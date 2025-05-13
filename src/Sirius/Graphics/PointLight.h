//
// Created by Leon on 09/05/2025.
//

#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include <DirectXMath.h>

#include "Buffer.h"
#include "Core/Component.h"


class PointLight : public Component {
public:
    void Init() override;

    void Update() override;

    void Reset() noexcept;

private:
    struct PointLightCBuf {
        DirectX::XMFLOAT3 pos;
        float padding;
    };

    PointLightCBuf cbufData{};
    DirectX::XMFLOAT3 pos = {0.0f, 0.0f, 0.0f};;
    mutable std::unique_ptr<IConstantBuffer> cbuf;
};


#endif //POINTLIGHT_H
