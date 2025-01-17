#include <Windows.h>

#include "Window.h"


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    try {
        const Window wnd(800, 600, "Hello World");
        MSG msg;
        BOOL g_result;
        while ((g_result = GetMessage(&msg, nullptr, 0, 0)) > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (wnd.kbd.KeyIsPressed(VK_SPACE)) {
                MessageBox(nullptr, "Hello there", "Space key was pressed", MB_OK);
            }
        }

        if (g_result == -1) {
            return -1;
        }
        return msg.wParam;
    } catch (const SiriusException &e) {
        MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    } catch (const std::exception &e) {
        MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
    } catch (...) {
        MessageBox(nullptr, "No details available", "Unknown exception", MB_OK | MB_ICONEXCLAMATION);
    }
}
