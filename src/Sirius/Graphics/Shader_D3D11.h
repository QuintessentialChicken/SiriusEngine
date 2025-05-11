//
// Created by Leon on 11/05/2025.
//

#ifndef SHADER_D3D11_H
#define SHADER_D3D11_H
#include "Shader.h"
#include <d3d11.h>
#include <string>
#include <wrl/client.h>

class Shader_D3D11 : public IShader {
public:
    Shader_D3D11(ShaderType type, const std::wstring& path, ID3D11Device* device, ID3D11DeviceContext* context);

    void Bind() override;

    [[nodiscard]] const void* GetBytecode() const override;

    [[nodiscard]] size_t GetBytecodeSize() const override;
private:
    ShaderType type;
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

    // Shader objects
    Microsoft::WRL::ComPtr<ID3DBlob> byteCode;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometryShader;
    Microsoft::WRL::ComPtr<ID3D11ComputeShader> computeShader;
    Microsoft::WRL::ComPtr<ID3D11HullShader> hullShader;
    Microsoft::WRL::ComPtr<ID3D11DomainShader> domainShader;

};



#endif //SHADER_D3D11_H
