#include <sstream>

#include "Window.h"

#include "WindowsThrowMacros.h"
#include "../resource.h"
#include "External/imgui_impl_win32.h"

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept : hInstance{GetModuleHandle(nullptr)} {
    WNDCLASSEX wc{};
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = HandleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hIcon = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = GetName();
    wc.hIconSm = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_HAND), IMAGE_ICON, 16, 16, 0));;

    RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass() noexcept {
    UnregisterClass(GetName(), hInstance);
}

const char* Window::WindowClass::GetName() noexcept {
    return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept {
    return wndClass.hInstance;
}

Window::Window(int width, int height, const char* title) : width{width}, height{height} {
    RECT wr;
    wr.left = 100;
    wr.right = wr.left + width;
    wr.top = 100;
    wr.bottom = wr.top + height;
    if (AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE) == 0) {
        throw CHWND_LAST_EXCEPT();
    }
    hWnd = CreateWindow(
        WindowClass::GetName(),
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        wr.right - wr.left,
        wr.bottom - wr.top,
        nullptr,
        nullptr,
        WindowClass::GetInstance(),
        this
    );

    if (hWnd == nullptr) {
        throw CHWND_LAST_EXCEPT();
    }

    ShowWindow(hWnd, SW_SHOWDEFAULT);
    ImGui_ImplWin32_Init(hWnd);
    gfx = std::make_unique<Graphics>(hWnd);
}

Window::~Window() {
    ImGui_ImplWin32_Shutdown();
    DestroyWindow(hWnd);
}

void Window::SetTitle(const std::string& title) const {
    if (SetWindowText(hWnd, title.c_str()) == 0) {
        throw CHWND_LAST_EXCEPT();
    }
}

std::optional<int> Window::ProcessMessage() {
    MSG message;
    if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
        if (message.message == WM_QUIT) {
            return static_cast<int>(message.wParam);
        }
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    return {};
}

Graphics& Window::GetGraphics() const {
    return *gfx;
}

LRESULT Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
    if (msg == WM_NCCREATE) {
        // Get the CREATESTRUCT we passed using lparam in CreateWindow() containing the initialization parameters of our window
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        // Get a pointer to the window from the CREATESTRUCT
        auto* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        // Store the pointer in WINAPI-managed user data
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // Set the window procedure in WINAPI-managed user data to HandleMsgThunk since we are leaving the setup
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
        // SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(DefWindowProc));
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
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
        return true;
    }
    switch (msg) {
        case WM_CLOSE:
            PostQuitMessage(EXIT_SUCCESS);
            break;
        case WM_KILLFOCUS:
            kbd.ClearState();
            break;
        /*********** KEYBOARD MESSAGES ***********/
        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
            if (!AUTOREPEAT || kbd.autorepeatEnabled) {
                kbd.OnKeyPressed(wParam);
            }
            break;
        case WM_SYSKEYUP:
        case WM_KEYUP:
            kbd.OnKeyReleased(wParam);
            break;
        case WM_CHAR:
            kbd.OnChar(static_cast<char>(wParam));
            break;
        /*********** END KEYBOARD MESSAGES ***********/
        /************** MOUSE MESSAGES ***************/
        case WM_MOUSEMOVE: {
            if (const auto [x, y] = MAKEPOINTS(lParam); x >= 0 && x < width && y >= 0 && y < height) {
                mouse.OnMouseMove(x, y);
                if (!mouse.IsInWindow()) {
                    SetCapture(hWnd);
                    mouse.OnMouseEnter();
                }
            }
            // not in client -> log move / maintain capture if button down
            else {
                if (wParam & (MK_LBUTTON | MK_RBUTTON)) {
                    mouse.OnMouseMove(x, y);
                }
                // button up -> release capture / log event for leaving
                else {
                    ReleaseCapture();
                    mouse.OnMouseLeave();
                }
            }
            break;
        }
        case WM_LBUTTONDOWN: {
            const auto [x, y]{MAKEPOINTS(lParam)};
            mouse.OnLeftPressed(x, y);
            break;
        }
        case WM_LBUTTONUP: {
            const auto [x, y]{MAKEPOINTS(lParam)};
            mouse.OnLeftReleased(x, y);
            break;
        }
        case WM_RBUTTONDOWN: {
            const auto [x, y]{MAKEPOINTS(lParam)};
            mouse.OnRightPressed(x, y);
            break;
        }
        case WM_RBUTTONUP: {
            const auto [x, y]{MAKEPOINTS(lParam)};
            mouse.OnRightReleased(x, y);
            break;
        }
        case WM_MOUSEWHEEL: {
            const auto [x, y] = MAKEPOINTS(lParam);
            const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
            mouse.OnWheelDelta(x, y, delta);
            break;
        }
        default: ;
        /************ END MOUSE MESSAGES *************/
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept {
    char* pMsgBuf = nullptr;
    // windows will allocate memory for err string and make our pointer point to it
    const DWORD nMsgLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hr,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
    );
    // 0 string length returned indicates a failure
    if (nMsgLen == 0) {
        return "Unidentified error code";
    }
    // copy error string from windows-allocated buffer to std::string
    std::string errorString = pMsgBuf;
    // free windows buffer
    LocalFree(pMsgBuf);
    return errorString;
}


Window::HrException::HrException(int line, const char* file, HRESULT hr) noexcept
    : Exception(line, file),
      hr(hr) {
}

const char* Window::HrException::what() const noexcept {
    std::ostringstream oss;
    oss << GetType() << std::endl
            << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
            << std::dec << " (" << (unsigned long) GetErrorCode() << ")" << std::endl
            << "[Description] " << GetErrorDescription() << std::endl
            << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Window::HrException::GetType() const noexcept {
    return "Sirius Window Exception";
}

HRESULT Window::HrException::GetErrorCode() const noexcept {
    return hr;
}

std::string Window::HrException::GetErrorDescription() const noexcept {
    return Exception::TranslateErrorCode(hr);
}


const char* Window::NoGfxException::GetType() const noexcept {
    return "Sirius Window Exception [No Graphics]";
}
