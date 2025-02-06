//
// Created by Leon on 06/02/2025.
//

#ifndef PLANE_H
#define PLANE_H
#include "DrawableBase.h"


class Plane : public DrawableBase<Plane> {
public:
    explicit Plane(Graphics& gfx);

    [[nodiscard]] DirectX::XMMATRIX GetTransformXM() const noexcept override;

    void SetTransform(const DirectX::XMFLOAT3& position) noexcept override;

    void AddTransform(const DirectX::XMFLOAT3& distance) noexcept override;

    void SetRotation(const DirectX::XMFLOAT3& rotation) noexcept override;

    void SetScale(const DirectX::XMFLOAT3& scale) noexcept override;

    std::vector<Vertex> vertices{
        {{-1.0f, 0.0f, -1.0f},{ 0.0f,0.0f }},
        {{1.0f, 0.0f, -1.0f}, { 1.0f,0.0f }},
        {{-1.0f, 0.0f, 1.0f}, { 0.0f,1.0f }},
        {{1.0f, 0.0f, 1.0f}, { 1.0f,1.0f } },
    };

    const std::vector<unsigned short> indices {
        0, 1, 2, 1, 3, 2
    };
};


#endif //PLANE_H
