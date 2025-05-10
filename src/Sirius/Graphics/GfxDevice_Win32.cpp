//
// Created by Leon on 26/04/2025.
//


#include <array>
#include <optional>

#include "GfxDevice.h"
#include "../../resource.h"
#include "WndProc.h"
#include "External/imgui_impl_win32.h"

std::array<float, 4> GfxDevice::color;
static std::string windowTitle;


void GfxDevice::SetWindowTitle(const std::string& title) {
    windowTitle = title;
    if (hwndMain) {
        SetWindowText(hwndMain, windowTitle.c_str());
    }
}

void GfxDevice::SetBackgroundColor(std::array<float, 4> color) {
    GfxDevice::color = color;
}

LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
        return true;
    }
    switch (msg) {
        case WM_CLOSE:
            PostQuitMessage(EXIT_SUCCESS);
            break;
        case WM_KILLFOCUS:
            // kbd.ClearState();
            break;
        default:
            {}
    }
    return g_WindowProc(hWnd, msg, wParam, lParam);
}

HWND GfxDevice::CreateDeviceWindow() {
    if (!winClass) {
        hInstance = GetModuleHandle(nullptr);
        WNDCLASSEX wc{};
        wc.cbSize = sizeof(wc);
        wc.style = CS_OWNDC;
        wc.lpfnWndProc = HandleMsg;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance;
        wc.hIcon = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
        wc.hCursor = nullptr;
        wc.hbrBackground = nullptr;
        wc.lpszMenuName = nullptr;
        wc.lpszClassName = "GfxWindow";
        wc.hIconSm = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_HAND), IMAGE_ICON, 16, 16, 0));;

        RegisterClassEx(&wc);
    }
    RECT wr;
    wr.left = 100;
    wr.right = wr.left + windowWidth;
    wr.top = 100;
    wr.bottom = wr.top + windowHeight;
    if (AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE) == 0) {
        // throw CHWND_LAST_EXCEPT();
    }
    HWND result = CreateWindow(
        "GfxWindow",
        windowTitle.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        wr.right - wr.left,
        wr.bottom - wr.top,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    ShowWindow(result, SW_SHOWDEFAULT);
    ImGui_ImplWin32_Init(result);

    return result;
}

static std::optional<int> ProcessMessage() {

}
