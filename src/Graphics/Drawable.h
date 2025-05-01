//
// Created by Leon on 02/02/2025.
//

#ifndef DRAWABLE_H
#define DRAWABLE_H
#include <DirectXMath.h>
#include <memory>
#include <vector>


class Bindable;

class Drawable {
    template<class T>
    friend class DrawableBase;

public:
    virtual ~Drawable() = default;

    [[nodiscard]] virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;

    void Draw() const noexcept;

    virtual void SetTransform(const DirectX::XMFLOAT3& position) noexcept = 0;

    virtual void AddTransform(const DirectX::XMFLOAT3& distance) noexcept = 0;

    virtual void SetRotation(const DirectX::XMFLOAT3& rotation) noexcept = 0;

    virtual void SetScale(const DirectX::XMFLOAT3& scale) noexcept = 0;

    struct Vertex {
        DirectX::XMFLOAT3 pos;
        struct
        {
            float u;
            float v;
        } tex;
    };

    // TODO Create a wrapper for Drawable vectors using properties
    // const std::vector<Vertex> vertices = {{{1.0f, 2.0f, 3.0f}}};
    // const std::vector<unsigned short> indices;

    float mass = 100.0f;
    float width = 1.0f;
    float height = 1.0f;
    float inertia = mass * (width * width + height * height) / 12.0f;
    float angularVelocity = 0.0f;
    float angle = 0.0f;
    float torque = 0.0f;
    DirectX::XMFLOAT3 force = {0.0f, 0.0f, 0.0f};
    DirectX::XMFLOAT3 position = {0.0f, 0.0f, 0.0f};
    DirectX::XMFLOAT3 rotation = {0.0f, 0.0f, 0.0f,};
    DirectX::XMFLOAT3 scale = {1.0f, 1.0f, 1.0f};
    DirectX::XMFLOAT3 linearVelocity = {0.0f, 0.0f, 0.0f};

protected:
    void AddBind(std::unique_ptr<Bindable> bind) noexcept;

    void AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf) noexcept;

private:
    [[nodiscard]] virtual const std::vector<std::unique_ptr<Bindable> >& GetStaticBinds() const noexcept = 0;

    const IndexBuffer* indexBuffer = nullptr;

    std::vector<std::unique_ptr<Bindable> > binds;
};


#endif //DRAWABLE_H
