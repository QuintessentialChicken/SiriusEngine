// PipelineState.h
#ifndef PIPELINE_STATE_H
#define PIPELINE_STATE_H

#include <d3d11.h>
#include <wrl/client.h>
#include "InputLayout.h"
#include "Shader.h"

// Primitive topology
enum class PrimitiveTopology {
    PointList,
    LineList,
    LineStrip,
    TriangleList,
    TriangleStrip
};

// Pipeline state description
struct PipelineStateDesc {
    PrimitiveTopology topology = PrimitiveTopology::TriangleList;
    IInputLayout* inputLayout = nullptr;
    IShader* vertexShader = nullptr;
    IShader* pixelShader = nullptr;
    IShader* geometryShader = nullptr;
    IShader* hullShader = nullptr;
    IShader* domainShader = nullptr;
};

// Pipeline state interface
class IPipelineState {
public:
    virtual ~IPipelineState() = default;
    virtual void Bind() = 0;
};


#endif // PIPELINE_STATE_H