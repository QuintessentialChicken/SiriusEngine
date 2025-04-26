add_library(Graphics STATIC
#        Bindable.cpp
#        Bindable.h
        Camera.cpp
        Camera.h
        ConstantBuffer.h
        Cube.cpp
        Cube.h
        GfxDevice.h
        GfxDevice_D3D11.cpp
        GfxDevice_Win32.cpp
#        Drawable.cpp
#        Drawable.h
        DrawableBase.h
        GraphicsThrowMacros.h
        ImguiManager.cpp
        ImguiManager.h
        IndexBuffer.cpp
        IndexBuffer.h
        InputLayout.cpp
        InputLayout.h
        PixelShader.cpp
        PixelShader.h
        Plane.cpp
        Plane.h
        Sampler.cpp
        Sampler.h
        Surface.cpp
        Surface.h
        Texture.cpp
        Texture.h
        Topology.cpp
        Topology.h
        TransformCBuf.cpp
        TransformCBuf.h
        VertexBuffer.h
        VertexShader.cpp
        VertexShader.h
)

target_link_libraries(Graphics PRIVATE Gdiplus.lib)