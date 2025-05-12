//
// Created by Leon on 26/04/2025.
//


#include <array>
#include <optional>

#include "GfxDevice.h"
#include "Renderer.h"
#include "../../resource.h"
#include "WndProc.h"
#include "Core/SiriusException.h"
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
        case WM_SIZE:
            Renderer::ResizeViewport(LOWORD(lParam), HIWORD(lParam));
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

std::string GfxDevice::Exception::TranslateErrorCode( HRESULT hr ) noexcept
{
    char* pMsgBuf = nullptr;
    // windows will allocate memory for err string and make our pointer point to it
    const DWORD nMsgLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,hr,MAKELANGID( LANG_NEUTRAL,SUBLANG_DEFAULT ),
        reinterpret_cast<LPSTR>(&pMsgBuf),0,nullptr
    );
    // 0 string length returned indicates a failure
    if( nMsgLen == 0 )
    {
        return "Unidentified error code";
    }
    // copy error string from windows-allocated buffer to std::string
    std::string errorString = pMsgBuf;
    // free windows buffer
    LocalFree( pMsgBuf );
    return errorString;
}


GfxDevice::HrException::HrException( int line,const char* file,HRESULT hr ) noexcept : Exception(line, file), hr( hr ) {}

const char* GfxDevice::HrException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
        << std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
        << "[Description] " << GetErrorDescription() << std::endl
        << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* GfxDevice::HrException::GetType() const noexcept
{
    return "Sirius Window Exception";
}

HRESULT GfxDevice::HrException::GetErrorCode() const noexcept
{
    return hr;
}

std::string GfxDevice::HrException::GetErrorDescription() const noexcept
{
    return Exception::TranslateErrorCode( hr );
}


const char* GfxDevice::NoGfxException::GetType() const noexcept
{
    return "Sirius Window Exception [No Graphics]";
}
