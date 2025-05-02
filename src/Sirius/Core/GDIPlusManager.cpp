//
// Created by Lms on 05/02/2025.
//

#include "GDIPlusManager.h"
#include <windows.h>
#include <algorithm>


namespace Gdiplus
{
    using std::min;
    using std::max;
}
#include <gdiplus.h>


ULONG_PTR GDIPlusManager::token = 0;
int GDIPlusManager::refCount = 0;

GDIPlusManager::GDIPlusManager()
{
    if( refCount++ == 0 )
    {
        Gdiplus::GdiplusStartupInput input;
        input.GdiplusVersion = 1;
        input.DebugEventCallback = nullptr;
        input.SuppressBackgroundThread = false;
        Gdiplus::GdiplusStartup( &token,&input,nullptr );
    }
}

GDIPlusManager::~GDIPlusManager()
{
    if( --refCount == 0 )
    {
        Gdiplus::GdiplusShutdown( token );
    }
}