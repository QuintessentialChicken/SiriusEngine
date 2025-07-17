//
// Created by Leon on 10/05/2025.
//

#ifndef BUFFER_H
#define BUFFER_H
#include <DirectXMath.h>

#include "Vulkan/Buffer_Vulkan.h"


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

class TransformBuffer {
public:
    struct Transforms {
        DirectX::XMMATRIX model;
        DirectX::XMMATRIX modelViewProj;
    };

    TransformBuffer();

    void Update(const DirectX::XMMATRIX& model, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection);

    void Bind() const;

private:
    ConstantBuffer_Vulkan constBuffer;
};

#endif //BUFFER_H
