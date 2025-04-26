add_library(Core
        App.cpp
        App.h
        dxerr.cpp
        dxerr.h
        DXGetErrorDescription.inl
        DXGetErrorString.inl
#        DxgiInfoManager.cpp
#        DxgiInfoManager.h
        DXTrace.inl
        Game.cpp
        Game.h
        SiriusException.cpp
        SiriusException.h
        Timer.cpp
        Timer.h
#        Window.cpp
#        Window.h
        WindowsThrowMacros.h
        GDIPlusManager.cpp
        GDIPlusManager.h
        WndProc.h
)
target_link_libraries(Core PRIVATE d3d11.lib dxguid.lib D3DCompiler.lib)
