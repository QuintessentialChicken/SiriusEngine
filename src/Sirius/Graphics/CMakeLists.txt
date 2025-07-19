find_package(Vulkan REQUIRED)
include_directories("$ENV{VULKAN_SDK}/Include")

add_library(Graphics STATIC
        Buffer.cpp
        Buffer.h
        Camera.cpp
        Camera.h
        GfxDevice.h
        GfxDevice_Win32.cpp
        ImguiManager.cpp
        ImguiManager.h
        InputLayout.h
        Material.cpp
        Material.h
        Mesh.cpp
        Mesh.h
        PipelineState.h
        Plane.cpp
        Plane.h
        PointLight.cpp
        PointLight.h
        Renderer.cpp
        Renderer.h
        Sampler.cpp
        Sampler.h
        Shader.h
        Surface.cpp
        Surface.h
        Texture.cpp
        Texture.h
        Transform.cpp
        Transform.h
        WndProc.h
        RenderApi_Vulkan.cpp
        RenderApi_Vulkan.h
        PipelineState.cpp
        Shader.cpp
)


target_link_libraries(Graphics PRIVATE Gdiplus.lib Vulkan::Vulkan)
