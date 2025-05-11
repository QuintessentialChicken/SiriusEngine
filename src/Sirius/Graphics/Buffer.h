//
// Created by Leon on 10/05/2025.
//

#ifndef BUFFER_H
#define BUFFER_H
#include <DirectXMath.h>
#include <intsafe.h>
#include <memory>

enum class ShaderStage {
    Vertex = 1,
    Pixel = 2,
    Geometry = 4,
    Hull = 8,
    Domain = 16,
    Compute = 32,
    All = Vertex | Pixel | Geometry | Hull | Domain | Compute
};

// Allow bitwise operations on ShaderStage
inline ShaderStage operator|(ShaderStage a, ShaderStage b) {
    return static_cast<ShaderStage>(static_cast<int>(a) | static_cast<int>(b));
}

inline ShaderStage operator&(ShaderStage a, ShaderStage b) {
    return static_cast<ShaderStage>(static_cast<int>(a) & static_cast<int>(b));
}

// Abstract buffer interfaces
class IVertexBuffer {
public:
    virtual ~IVertexBuffer() = default;

    virtual void Bind() = 0;

    virtual void Update(const void* data, size_t size) = 0;
};

class IIndexBuffer {
public:
    virtual ~IIndexBuffer() = default;

    virtual void Bind() = 0;

    // virtual void Update(const void* data, size_t size) = 0;
    [[nodiscard]] virtual UINT GetCount() const = 0;
};

class IConstantBuffer {
public:
    virtual ~IConstantBuffer() = default;

    virtual void Bind(ShaderStage stages, UINT slot) = 0;

    void Bind(UINT slot) { Bind(ShaderStage::All, slot); }

    virtual void Update(const void* data, size_t size) = 0;
};

class TransformBuffer {
public:
    struct Transforms {
        DirectX::XMMATRIX model;
        DirectX::XMMATRIX modelViewProj;
    };

    TransformBuffer();

    void Update(const DirectX::XMMATRIX& model, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection) const;

    void Bind() const;

private:
    std::unique_ptr<IConstantBuffer> constBuffer;
};

#endif //BUFFER_H
