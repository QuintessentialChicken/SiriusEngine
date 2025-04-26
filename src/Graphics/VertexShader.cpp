// //
// // Created by Leon on 02/02/2025.
// //
//
// #include <d3dcompiler.h>
//
//
// #include "VertexShader.h"
// #include "GraphicsThrowMacros.h"
//
// VertexShader::VertexShader(Graphics& gfx, const std::wstring& path) {
//     INFOMAN(gfx);
//
//     GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(),&bytecodeBlob));
//     GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(
//         bytecodeBlob->GetBufferPointer(),
//         bytecodeBlob->GetBufferSize(),
//         nullptr,
//         &vertexShader
//     ));
// }
//
// void VertexShader::Bind(Graphics& gfx) noexcept {
//     GetContext(gfx)->VSSetShader(vertexShader.Get(), nullptr, 0);
// }
//
// ID3DBlob* VertexShader::GetBytecode() const noexcept {
//     return bytecodeBlob.Get();
// }
