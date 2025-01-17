#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <sstream>
#include <Windows.h>

#include "Window.h"


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    try {
        Window wnd(800, 600, "Hello World");
        MSG msg;
        BOOL g_result;
        while ((g_result = GetMessage(&msg, nullptr, 0, 0)) > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (wnd.kbd.KeyIsPressed(VK_SPACE)) {
                MessageBox(nullptr, "Hello there", "Space key was pressed", MB_OK);
            }
            while (!wnd.mouse.IsEmpty()) {
                const auto e = wnd.mouse.Read();
                switch (e.GetType()) {
                    case Mouse::Event::Type::Leave:
                        wnd.SetTitle("Gone!");
                        break;
                    case Mouse::Event::Type::Move:
                        std::ostringstream oss;
                        oss << "Mouse position: (" << e.GetPosX() << ", " << e.GetPosY() << ")";
                        wnd.SetTitle(oss.str());
                        break;
                }


                // if (const auto event{wnd.mouse.Read()}; event.GetType() == Mouse::Event::Type::Move) {
                //     std::ostringstream oss;
                //     oss << "Mouse position: (" << event.GetPosX() << ", " << event.GetPosY() << ")";
                //     wnd.SetTitle(oss.str());
                // }
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
    return -1;
}
