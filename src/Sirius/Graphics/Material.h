#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>
#include <string>
#include "Shader.h"
#include "Buffer.h"
#include "InputLayout.h"
#include "PipelineState.h"

class Material {
public:
    virtual ~Material() = default;

    Material(const std::wstring& vertexShaderPath, const std::wstring& pixelShaderPath);

    virtual void Bind();
    
    // Factory method for creating a Phong material
    static std::shared_ptr<Material> CreatePhongMaterial();
    
private:
    std::unique_ptr<IShader> vertexShader;
    std::unique_ptr<IShader> pixelShader;
    std::unique_ptr<IInputLayout> inputLayout;
    std::unique_ptr<IPipelineState> pipelineState;
};


class ColoredCubeMaterial : public Material {
public:
    ColoredCubeMaterial();

    // Set a specific face color
    void SetFaceColor(int faceIndex, const DirectX::XMFLOAT4& color);

    // Override the Bind method to bind our custom color buffer
    void Bind() override;

private:
    struct ColorBufferData {
        DirectX::XMFLOAT4 faceColors[6];
    };

    ColorBufferData colorData;
    std::unique_ptr<IConstantBuffer> colorBuffer;
    bool colorBufferDirty = true;
};

#endif // MATERIAL_H

