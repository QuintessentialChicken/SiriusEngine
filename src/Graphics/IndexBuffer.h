// //
// // Created by Leon on 02/02/2025.
// //
//
// #ifndef INDEXBUFFER_H
// #define INDEXBUFFER_H
// #include "Bindable.h"
//
//
// class IndexBuffer : public Bindable {
// public:
//     IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices);
//
//     void Bind(Graphics& gfx) noexcept override;
//
//     [[nodiscard]] UINT GetCount() const noexcept;
//
// private:
//     UINT count;
//     Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
// };
//
//
// #endif //INDEXBUFFER_H
