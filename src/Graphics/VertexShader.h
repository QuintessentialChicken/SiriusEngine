// //
// // Created by Leon on 02/02/2025.
// //
//
// #ifndef VERTEXSHADER_H
// #define VERTEXSHADER_H
//
// #include "Bindable.h"
//
// class VertexShader : public Bindable
// {
// public:
//     VertexShader( Graphics& gfx,const std::wstring& path );
//     void Bind( Graphics& gfx ) noexcept override;
//     [[nodiscard]] ID3DBlob* GetBytecode() const noexcept;
// protected:
//     Microsoft::WRL::ComPtr<ID3DBlob> bytecodeBlob;
//     Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
// };
//
//
//
// #endif //VERTEXSHADER_H
