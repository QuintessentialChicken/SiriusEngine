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

    [[nodiscard]] virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;

    void Draw(Graphics& gfx) const noexcept;

    virtual void Update(float dt) noexcept = 0;

    virtual ~Drawable() = default;

protected:
    void AddBind(std::unique_ptr<Bindable> bind) noexcept;

    void AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf) noexcept;

private:
    [[nodiscard]] virtual const std::vector<std::unique_ptr<Bindable> >& GetStaticBinds() const noexcept = 0;

    const IndexBuffer* indexBuffer = nullptr;
    std::vector<std::unique_ptr<Bindable>> binds;
};


#endif //DRAWABLE_H
