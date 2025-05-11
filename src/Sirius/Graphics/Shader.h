//
// Created by Leon on 11/05/2025.
//

#ifndef SHADER_H
#define SHADER_H


enum class ShaderType {
    Vertex,
    Pixel,
    Geometry,
    Compute,
    Hull,
    Domain
};

class IShader {
public:
    virtual ~IShader() = default;
    virtual void Bind() = 0;
    [[nodiscard]] virtual const void* GetBytecode() const = 0;
    [[nodiscard]] virtual size_t GetBytecodeSize() const = 0;
};




#endif //SHADER_H
