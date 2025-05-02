//
// Created by Leon on 02/02/2025.
//

#ifndef VERTEXSHADER_H
#define VERTEXSHADER_H

#include <string>
#include <wrl/client.h>

#include "Bindable.h"
#include "VertexBuffer.h"

class VertexShader : public Bindable
{
public:
    explicit VertexShader(const std::wstring& path );
    void Bind() noexcept override;
    [[nodiscard]] ID3DBlob* GetBytecode() const noexcept;
protected:
    Microsoft::WRL::ComPtr<ID3DBlob> bytecodeBlob;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
};



#endif //VERTEXSHADER_H
