#include "Window.h"

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept : hInstance{GetModuleHandle(nullptr)} {
    WNDCLASSEX wc{};
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = HandleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = GetName();
    wc.hIconSm = nullptr;

    RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass() noexcept {
    UnregisterClass(GetName(), hInstance);
}

const char * Window::WindowClass::GetName() noexcept {
    return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept {
    return wndClass.hInstance;
}

Window::Window(int width, int height, const char *title) noexcept : width{width}, height{height} {
    RECT wr;
    wr.left = 100;
    wr.right = wr.left + width;
    wr.top = 100;
    wr.bottom = wr.top + height;
    AdjustWindowRectEx(&wr, WS_OVERLAPPEDWINDOW, false, false);
    hWnd = CreateWindow(
        WindowClass::GetName(),
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width,
        height,
        nullptr,
        nullptr,
        WindowClass::GetInstance(),
        this
    );
    ShowWindow(hWnd, SW_SHOW);

}

Window::~Window() {
    DestroyWindow(hWnd);
}

LRESULT Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
    if (msg == WM_NCCREATE) {
        // Get the CREATESTRUCT we passed using lparam in CreateWindow() containing the initialization parameters of our window
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        // Get a pointer to the window from the CREATESTRUCT
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        // Store the pointer in WINAPI-managed user data
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // Set the window procedure in WINAPI-managed user data to HandleMsgThunk since we are leaving the setup
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
        // Delegate the current msg to HandleMsg
        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }

    // Delegate any message received before the WM_NCCREATE to the default window procedure
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
    // Get the pointer to the window and delegate the message to the HandleMsg function
    auto* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
    switch (msg) {
        case WM_CLOSE:
            PostQuitMessage(EXIT_SUCCESS);
            return 0;
        default: DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}


