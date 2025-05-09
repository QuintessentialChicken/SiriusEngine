//
// Created by Leon on 02/02/2025.
//

#ifndef CUBE_H
#define CUBE_H
#include "DrawableBase.h"
constexpr float side = 1.0f / 2.0f;

class Cube : public DrawableBase<Cube> {
public:
    explicit Cube();

    [[nodiscard]] DirectX::XMMATRIX GetTransformXM() const noexcept override;
    [[nodiscard]] DirectX::XMMATRIX GetTransformXMAlt() const noexcept override;

    void SetNormalsIndependentFlat() noexcept;

    void Update(float dt) noexcept override;

    void SetTransform(const DirectX::XMFLOAT3& position) noexcept override;

    void AddTransform(const DirectX::XMFLOAT3& distance) noexcept override;

    void SetRotation(const DirectX::XMFLOAT3& rotation) noexcept override;

    void SetScale(const DirectX::XMFLOAT3& scale) noexcept override;


    std::vector<Vertex> vertices {
        {{-0.5f, -0.5f, -0.5f}},
        {{0.5f, -0.5f, -0.5f}},
        {{-0.5f, 0.5f, -0.5f}},
        {{0.5f, 0.5f, -0.5f}},
        {{-0.5f, -0.5f, 0.5f}},
        {{0.5f, -0.5f, 0.5f}},
        {{-0.5f, 0.5f, 0.5f}},
        {{0.5f, 0.5f, 0.5f}}
    };
    const std::vector<unsigned short> indices{
        0, 2, 1, 2, 3, 1,
        1, 3, 5, 3, 7, 5,
        2, 6, 3, 3, 6, 7,
        4, 5, 7, 4, 7, 6,
        0, 4, 2, 2, 4, 6,
        0, 1, 4, 1, 5, 4
    };


    std::vector<Vertex> independentVertices {
        { { -side,-side,-side }},// 0 near side
        { { side,-side,-side }},// 1
        { { -side,side,-side }},// 2
        { { side,side,-side }},// 3
        { { -side,-side,side }},// 4 far side
        { { side,-side,side }},// 5
        { { -side,side,side }},// 6
        { { side,side,side }},// 7
        { { -side,-side,-side }},// 8 left side
        { { -side,side,-side }},// 9
        { { -side,-side,side }},// 10
        { { -side,side,side }},// 11
        { { side,-side,-side }},// 12 right side
        { { side,side,-side }},// 13
        { { side,-side,side }},// 14
        { { side,side,side }},// 15
        { { -side,-side,-side }},// 16 bottom side
        { { side,-side,-side }},// 17
        { { -side,-side,side }},// 18
        { { side,-side,side }},// 19
        { { -side,side,-side }},// 20 top side
        { { side,side,-side }},// 21
        { { -side,side,side }},// 22
        { { side,side,side }}// 23
    };

    const std::vector<unsigned short> independentIndices{
        0,2, 1,    2,3,1,
        4,5, 7,    4,7,6,
        8,10, 9,  10,11,9,
        12,13,15, 12,15,14,
        16,17,18, 18,17,19,
        20,23,21, 20,22,23
    };


};

#endif //CUBE_H
