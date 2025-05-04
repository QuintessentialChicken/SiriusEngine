//
// Created by Leon on 26/04/2025.
//

#ifndef WNDPROC_H
#define WNDPROC_H

inline LRESULT (__stdcall * g_WindowProc)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) = DefWindowProc;

inline HWND hwndMain = nullptr;
inline HINSTANCE hInstance = nullptr;
inline unsigned winClass;

inline int windowWidth = 800;

inline int windowHeight = 600;

#endif //WNDPROC_H
