// //
// // Created by Leon on 09/05/2025.
// //
//
// #ifndef POINTLIGHT_H
// #define POINTLIGHT_H
// #include <DirectXMath.h>
//
// #include "ConstantBuffer.h"
// #include "Sphere.h"
//
//
// class PointLight {
// public:
//     explicit PointLight(float radius = 0.5f );
//     void SpawnControlWindow() noexcept;
//     void Reset() noexcept;
//     void Draw() const noexcept;
//     void Bind() const noexcept;
// private:
//     struct PointLightCBuf
//     {
//         DirectX::XMFLOAT3 pos;
//         float padding;
//     };
//     DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f };;
//     mutable PixelConstantBuffer<PointLightCBuf> cbuf;
//     mutable Sphere mesh;
//
// };
//
//
// #endif //POINTLIGHT_H
