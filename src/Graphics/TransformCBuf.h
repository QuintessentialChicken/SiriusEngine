//
// Created by Leon on 02/02/2025.
//

#ifndef TRANSFORMCBUF_H
#define TRANSFORMCBUF_H
#include "Bindable.h"
#include "ConstantBuffer.h"
#include "Drawable.h"


class TransformCBuf : public Bindable {
public:
    explicit TransformCBuf(const Drawable& parent );
    void Bind() noexcept override;
private:
    static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> vcbuf;
    const Drawable& parent;
};



#endif //TRANSFORMCBUF_H
