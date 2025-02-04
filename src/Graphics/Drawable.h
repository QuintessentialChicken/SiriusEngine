//
// Created by Leon on 02/02/2025.
//

#ifndef DRAWABLE_H
#define DRAWABLE_H
#include <DirectXMath.h>
#include <memory>

#include "Graphics.h"


class Bindable;

class Drawable {
    template<class T>
    friend class DrawableBase;

public:
    Drawable() = default;

    Drawable(const Drawable&) = delete;

    virtual ~Drawable() = default;

    [[nodiscard]] virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;

    void Draw(Graphics& gfx) const noexcept;

    virtual void SetTransform(const DirectX::XMFLOAT3& position) noexcept = 0;

    virtual void AddTransform(const DirectX::XMFLOAT3& distance) noexcept = 0;

    virtual void SetRotation(const DirectX::XMFLOAT3& rotation) noexcept = 0;

    virtual void SetScale(const DirectX::XMFLOAT3& scale) noexcept = 0;

    DirectX::XMFLOAT3 position = {0.0f, 0.0f, 0.0f};
    DirectX::XMFLOAT3 rotation = {0.0f, 0.0f, 0.0f,};
    DirectX::XMFLOAT3 scale = {1.0f, 1.0f, 1.0f};

protected:
    void AddBind(std::unique_ptr<Bindable> bind) noexcept;

    void AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf) noexcept;

private:
    [[nodiscard]] virtual const std::vector<std::unique_ptr<Bindable> >& GetStaticBinds() const noexcept = 0;

    const IndexBuffer* indexBuffer = nullptr;

    std::vector<std::unique_ptr<Bindable>> binds;
};


#endif //DRAWABLE_H
