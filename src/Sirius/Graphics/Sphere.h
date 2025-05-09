//
// Created by Leon on 09/05/2025.
//

#ifndef SPHERE_H
#define SPHERE_H
#include "DrawableBase.h"


class Sphere : public DrawableBase<Sphere> {
public:


    Sphere();
    Sphere(int latDiv,int longDiv);
    void Update(float dt) noexcept override;
    DirectX::XMMATRIX GetTransformXM() const noexcept override;
    [[nodiscard]] DirectX::XMMATRIX GetTransformXMAlt() const noexcept override;

    // TODO Unify in Drawable
    void SetTransform(const DirectX::XMFLOAT3& position) noexcept override;

    void AddTransform(const DirectX::XMFLOAT3& distance) noexcept override;

    void SetRotation(const DirectX::XMFLOAT3& rotation) noexcept override;

    void SetScale(const DirectX::XMFLOAT3& scale) noexcept override;

    std::vector<Vertex> vertices;
    std::vector<unsigned short> indices;

};



#endif //SPHERE_H
