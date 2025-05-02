//
// Created by Leon on 02/02/2025.
//

#ifndef CUBE_H
#define CUBE_H
#include "DrawableBase.h"

class Cube : public DrawableBase<Cube> {
public:
    explicit Cube();

    [[nodiscard]] DirectX::XMMATRIX GetTransformXM() const noexcept override;

    void SetTransform(const DirectX::XMFLOAT3& position) noexcept override;

    void AddTransform(const DirectX::XMFLOAT3& distance) noexcept override;

    void SetRotation(const DirectX::XMFLOAT3& rotation) noexcept override;

    void SetScale(const DirectX::XMFLOAT3& scale) noexcept override;


    const std::vector<Vertex> vertices{
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

    // template<class V>
    // static IndexedTriangleList<V> MakeSkinned() {
    //     namespace dx = DirectX;
    //
    //     constexpr float side = 1.0f / 2.0f;
    //
    //     std::vector<V> vertices(14);
    //     vertices[0].pos = {-side, -side, -side};
    //     vertices[0].tex = {2.0f / 3.0f, 0.0f / 4.0f};
    //     vertices[1].pos = {side, -side, -side};
    //     vertices[1].tex = {1.0f / 3.0f, 0.0f / 4.0f};
    //     vertices[2].pos = {-side, side, -side};
    //     vertices[2].tex = {2.0f / 3.0f, 1.0f / 4.0f};
    //     vertices[3].pos = {side, side, -side};
    //     vertices[3].tex = {1.0f / 3.0f, 1.0f / 4.0f};
    //     vertices[4].pos = {-side, -side, side};
    //     vertices[4].tex = {2.0f / 3.0f, 3.0f / 4.0f};
    //     vertices[5].pos = {side, -side, side};
    //     vertices[5].tex = {1.0f / 3.0f, 3.0f / 4.0f};
    //     vertices[6].pos = {-side, side, side};
    //     vertices[6].tex = {2.0f / 3.0f, 2.0f / 4.0f};
    //     vertices[7].pos = {side, side, side};
    //     vertices[7].tex = {1.0f / 3.0f, 2.0f / 4.0f};
    //     vertices[8].pos = {-side, -side, -side};
    //     vertices[8].tex = {2.0f / 3.0f, 4.0f / 4.0f};
    //     vertices[9].pos = {side, -side, -side};
    //     vertices[9].tex = {1.0f / 3.0f, 4.0f / 4.0f};
    //     vertices[10].pos = {-side, -side, -side};
    //     vertices[10].tex = {3.0f / 3.0f, 1.0f / 4.0f};
    //     vertices[11].pos = {-side, -side, side};
    //     vertices[11].tex = {3.0f / 3.0f, 2.0f / 4.0f};
    //     vertices[12].pos = {side, -side, -side};
    //     vertices[12].tex = {0.0f / 3.0f, 1.0f / 4.0f};
    //     vertices[13].pos = {side, -side, side};
    //     vertices[13].tex = {0.0f / 3.0f, 2.0f / 4.0f};
    //
    //     return {
    //         std::move(vertices), {
    //             0, 2, 1, 2, 3, 1,
    //             4, 8, 5, 5, 8, 9,
    //             2, 6, 3, 3, 6, 7,
    //             4, 5, 7, 4, 7, 6,
    //             2, 10, 11, 2, 11, 6,
    //             12, 3, 7, 12, 7, 13
    //         }
    //     };
    // }

private:
    float r = 0.0f;
    float roll = 0.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;
    float theta = 0.0f;
    float phi = 0.0f;
    float chi = 0.0f;
    // speed (delta/s)
    float droll = 1.0f;
    float dpitch = 1.0f;
    float dyaw = 1.0f;
    float dtheta = 1.0f;
    float dphi = 1.0f;
    float dchi = 1.0f;
};

#endif //CUBE_H
