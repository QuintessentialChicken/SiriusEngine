// PipelineState.h
#ifndef PIPELINE_STATE_H
#define PIPELINE_STATE_H

#include "InputLayout.h"

// Primitive topology
enum class PrimitiveTopology {
    PointList,
    LineList,
    LineStrip,
    TriangleList,
    TriangleStrip
};

// Pipeline state description
class PipelineStateDesc {
    PrimitiveTopology topology = PrimitiveTopology::TriangleList;
    IInputLayout* inputLayout = nullptr;
    Shader_Vulkan* vertexShader = nullptr;
    Shader_Vulkan* pixelShader = nullptr;
    Shader_Vulkan* geometryShader = nullptr;
    Shader_Vulkan* hullShader = nullptr;
    Shader_Vulkan* domainShader = nullptr;
};

// Pipeline state interface
class IPipelineState {
public:
    virtual ~IPipelineState() = default;
    virtual void Bind() = 0;
};


#endif // PIPELINE_STATE_H