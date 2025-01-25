#pragma once
#include <string>
#include <Windows.h>

#include "SiriusException.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Graphics/Graphics.h"

class Window {
public:
    class Exception : public SiriusException {
        using SiriusException::SiriusException;

    public:
        static std::string TranslateErrorCode(HRESULT hr) noexcept;
    };

    class HrException : public Exception {
    public:
        HrException(int line, const char *file, HRESULT hr) noexcept;

        const char *what() const noexcept override;

        const char *GetType() const noexcept override;

        HRESULT GetErrorCode() const noexcept;

        std::string GetErrorDescription() const noexcept;

    private:
        HRESULT hr;
    };

    class NoGfxException : public Exception {
    public:
        using Exception::Exception;

        const char *GetType() const noexcept override;
    };

private:
    class WindowClass {
    public:
        static const char *GetName() noexcept;

        static HINSTANCE GetInstance() noexcept;

        WindowClass(const WindowClass &) = delete;

        WindowClass &operator=(const WindowClass &) = delete;

    private:
        WindowClass() noexcept;

        ~WindowClass() noexcept;

        static constexpr auto wndClassName = "Sirius Direct3D";
        static WindowClass wndClass;
        HINSTANCE hInstance;
    };

public:
    Window(int width, int height, const char *title);

    ~Window();

    Window(const Window &) = delete;

    Window &operator=(const Window &) = delete;

    void SetTitle(const std::string &title) const;

    static std::optional<int> ProcessMessage();

    [[nodiscard]] Graphics &GetGraphics() const;

    Keyboard kbd;
    Mouse mouse;

private:
    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

    static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

    int width;
    int height;
    HWND hWnd;
    std::unique_ptr<Graphics> gfx;
};

#define AUTOREPEAT (lParam & 0x40000000)
#define CHWND_EXCEPT( hr ) Window::HrException( __LINE__,__FILE__,hr )
#define CHWND_LAST_EXCEPT() Window::HrException( __LINE__,__FILE__,GetLastError() )
#define CHWND_NOGFX_EXCEPT() Window::NoGfxHrException( __LINE__,__FILE__ )