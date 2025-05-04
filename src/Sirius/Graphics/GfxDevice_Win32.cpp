//
// Created by Leon on 26/04/2025.
//


#include <array>
#include <optional>

#include "GfxDevice.h"
#include "../../resource.h"
#include "WndProc.h"
#include "External/imgui_impl_win32.h"

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

LRESULT HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
    // Get the pointer to the window and delegate the message to the HandleMsg function
    // auto* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
    if (msg == WM_NCCREATE) {
        // Get the CREATESTRUCT we passed using lparam in CreateWindow() containing the initialization parameters of our window
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        // Get a pointer to the window from the CREATESTRUCT
        // auto* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        // Store the pointer in WINAPI-managed user data
        // SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // Set the window procedure in WINAPI-managed user data to HandleMsgThunk since we are leaving the setup
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&HandleMsgThunk));
        // SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(DefWindowProc));
        // Delegate the current msg to HandleMsg
        return HandleMsg(hWnd, msg, wParam, lParam);
    }

    // Delegate any message received before the WM_NCCREATE to the default window procedure
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

HWND GfxDevice::CreateDeviceWindow() {
    if (!winClass) {
        hInstance = GetModuleHandle(nullptr);
        WNDCLASSEX wc{};
        wc.cbSize = sizeof(wc);
        wc.style = CS_OWNDC;
        wc.lpfnWndProc = HandleMsgSetup;
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
