add_library(Core STATIC
        Component.h
        dxerr.cpp
        dxerr.h
        DXGetErrorDescription.inl
        DXGetErrorString.inl
        DxgiInfoManager.cpp
        DxgiInfoManager.h
        DXTrace.inl
        Entrypoint_Engine.cpp
        Entrypoint_Engine.h
        Model.cpp
        Model.h
        SiriusException.cpp
        SiriusException.h
        Timer.cpp
        Timer.h
        WindowsThrowMacros.h
        GDIPlusManager.cpp
        GDIPlusManager.h
        Fsm.cpp
        Fsm.h
)
target_link_libraries(Core PRIVATE d3d11.lib dxguid.lib D3DCompiler.lib)
