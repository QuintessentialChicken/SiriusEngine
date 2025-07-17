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
        Vulkan/RenderApi_Vulkan.cpp
        Vulkan/RenderApi_Vulkan.h
        Vulkan/Buffer_Vulkan.cpp
        Vulkan/Buffer_Vulkan.h
        Vulkan/PipelineState_Vulkan.cpp
        Vulkan/PipelineState_Vulkan.h
        Vulkan/Shader_Vulkan.cpp
        Vulkan/Shader_Vulkan.h
)


target_link_libraries(Graphics PRIVATE Gdiplus.lib Vulkan::Vulkan)
