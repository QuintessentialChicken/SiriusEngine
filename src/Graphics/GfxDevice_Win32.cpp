//
// Created by Leon on 26/04/2025.
//


#include <optional>

#include "GfxDevice.h"
#include "resource.h"
#include "Core/WndProc.h"
#include "External/imgui_impl_win32.h"

static std::string windowTitle;


void GfxDevice::SetWindowTitle(const std::string& title) {
    windowTitle = title;
    if (hwndMain) {
        SetWindowText(hwndMain, windowTitle.c_str());
    }
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
        // /*********** KEYBOARD MESSAGES ***********/
        // case WM_SYSKEYDOWN:
        // case WM_KEYDOWN:
        //     if (!AUTOREPEAT || kbd.autorepeatEnabled) {
        //         kbd.OnKeyPressed(wParam);
        //     }
        //     break;
        // case WM_SYSKEYUP:
        // case WM_KEYUP:
        //     kbd.OnKeyReleased(wParam);
        //     break;
        // case WM_CHAR:
        //     kbd.OnChar(static_cast<char>(wParam));
        //     break;
        // /*********** END KEYBOARD MESSAGES ***********/
        // /************** MOUSE MESSAGES ***************/
        // case WM_MOUSEMOVE: {
        //     if (const auto [x, y] = MAKEPOINTS(lParam); x >= 0 && x < width && y >= 0 && y < height) {
        //         mouse.OnMouseMove(x, y);
        //         if (!mouse.IsInWindow()) {
        //             SetCapture(hWnd);
        //             mouse.OnMouseEnter();
        //         }
        //     }
        //     // not in client -> log move / maintain capture if button down
        //     else {
        //         if (wParam & (MK_LBUTTON | MK_RBUTTON)) {
        //             mouse.OnMouseMove(x, y);
        //         }
        //         // button up -> release capture / log event for leaving
        //         else {
        //             ReleaseCapture();
        //             mouse.OnMouseLeave();
        //         }
        //     }
        //     break;
        // }
        // case WM_LBUTTONDOWN: {
        //     const auto [x, y]{MAKEPOINTS(lParam)};
        //     mouse.OnLeftPressed(x, y);
        //     break;
        // }
        // case WM_LBUTTONUP: {
        //     const auto [x, y]{MAKEPOINTS(lParam)};
        //     mouse.OnLeftReleased(x, y);
        //     break;
        // }
        // case WM_RBUTTONDOWN: {
        //     const auto [x, y]{MAKEPOINTS(lParam)};
        //     mouse.OnRightPressed(x, y);
        //     break;
        // }
        // case WM_RBUTTONUP: {
        //     const auto [x, y]{MAKEPOINTS(lParam)};
        //     mouse.OnRightReleased(x, y);
        //     break;
        // }
        // case WM_MOUSEWHEEL: {
        //     const auto [x, y] = MAKEPOINTS(lParam);
        //     const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
        //     mouse.OnWheelDelta(x, y, delta);
        //     break;
        // }
        default: ;
        /************ END MOUSE MESSAGES *************/
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
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
