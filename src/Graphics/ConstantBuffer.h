// //
// // Created by Leon on 02/02/2025.
// //
//
// #ifndef CONSTANTBUFFER_H
// #define CONSTANTBUFFER_H
// #include "Bindable.h"
// #include "GraphicsThrowMacros.h"
//
// template<typename C>
// class ConstantBuffer : public Bindable {
// public:
//     explicit ConstantBuffer(Graphics& gfx) {
//         INFOMAN(gfx);
//
//         D3D11_BUFFER_DESC cbd;
//         cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//         cbd.Usage = D3D11_USAGE_DYNAMIC;
//         cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//         cbd.MiscFlags = 0u;
//         cbd.ByteWidth = sizeof(C);
//         cbd.StructureByteStride = 0u;
//         GFX_THROW_INFO(GetDevice( gfx )->CreateBuffer( &cbd,nullptr,&constantBuffer ));
//     }
//
//     ConstantBuffer(Graphics& gfx, const C& consts) {
//         INFOMAN(gfx);
//
//         D3D11_BUFFER_DESC cbd;
//         cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//         cbd.Usage = D3D11_USAGE_DYNAMIC;
//         cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//         cbd.MiscFlags = 0u;
//         cbd.ByteWidth = sizeof(consts);
//         cbd.StructureByteStride = 0u;
//
//         D3D11_SUBRESOURCE_DATA csd = {};
//         csd.pSysMem = &consts;
//         GFX_THROW_INFO(GetDevice( gfx )->CreateBuffer( &cbd,&csd,&constantBuffer ));
//     }
//
//     void Update(Graphics& gfx, const C& consts) {
//         INFOMAN(gfx);
//
//         D3D11_MAPPED_SUBRESOURCE msr;
//         GFX_THROW_INFO(GetContext( gfx )->Map(constantBuffer.Get(),0u,D3D11_MAP_WRITE_DISCARD,0u, &msr));
//         memcpy(msr.pData, &consts, sizeof(consts));
//         GetContext(gfx)->Unmap(constantBuffer.Get(), 0u);
//     }
//
// protected:
//     Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
// };
//
// template<typename C>
// class VertexConstantBuffer : public ConstantBuffer<C> {
//     using ConstantBuffer<C>::constantBuffer;
//     using Bindable::GetContext;
//
// public:
//     using ConstantBuffer<C>::ConstantBuffer;
//
//     void Bind(Graphics& gfx) noexcept override {
//         GetContext(gfx)->VSSetConstantBuffers(0u, 1u, constantBuffer.GetAddressOf());
//     }
// };
//
// template<typename C>
// class PixelConstantBuffer : public ConstantBuffer<C> {
//     using ConstantBuffer<C>::constantBuffer;
//     using Bindable::GetContext;
//
// public:
//     using ConstantBuffer<C>::ConstantBuffer;
//
//     void Bind(Graphics& gfx) noexcept override {
//         GetContext(gfx)->PSSetConstantBuffers(0u, 1u, constantBuffer.GetAddressOf());
//     }
// };
//
// #endif //CONSTANTBUFFER_H
