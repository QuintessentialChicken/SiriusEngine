//
// Created by Leon on 04/05/2025.
//

#include "Input.h"

#include <iostream>

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics/WndProc.h"
#define AUTOREPEAT (lParam & 0x40000000)

void Input::Init() {
    g_WindowProc = InputWindowProc;
}

LRESULT CALLBACK InputWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        /*********** KEYBOARD MESSAGES ***********/
        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
            if (!AUTOREPEAT || Keyboard::IsAutorepeatEnabled()) {
                Keyboard::OnKeyPressed(wParam);
            }
            break;
        case WM_SYSKEYUP:
        case WM_KEYUP:
            Keyboard::OnKeyReleased(wParam);
            break;
        case WM_CHAR:
            Keyboard::OnChar(static_cast<char>(wParam));
            break;
        /*********** END KEYBOARD MESSAGES ***********/
        /************** MOUSE MESSAGES ***************/
        case WM_MOUSEMOVE: {
            if (const auto [x, y] = MAKEPOINTS(lParam); x >= 0 && x < windowWidth && y >= 0 && y < windowHeight) {
                Mouse::SetPos(x, y);
                if (!Mouse::IsInWindow()) {
                    SetCapture(hWnd);
                    Mouse::SetIsInWindow(true);
                }
            }
            // not in client -> log move / maintain capture if button down
            else {
                if (wParam & (MK_LBUTTON | MK_RBUTTON)) {
                    Mouse::SetPos(x, y);
                }
                // button up -> release capture / log event for leaving
                else {
                    ReleaseCapture();
                    Mouse::SetIsInWindow(false);
                }
            }
            break;
        }
        case WM_LBUTTONDOWN: {
            const auto [x, y]{MAKEPOINTS(lParam)};
            Mouse::SetLeftIsPressed(true);
            break;
        }
        case WM_LBUTTONUP: {
            const auto [x, y]{MAKEPOINTS(lParam)};
            Mouse::SetLeftIsPressed(false);
            break;
        }
        case WM_RBUTTONDOWN: {
            const auto [x, y]{MAKEPOINTS(lParam)};
            Mouse::SetRightIsPressed(true);
            break;
        }
        case WM_RBUTTONUP: {
            const auto [x, y]{MAKEPOINTS(lParam)};
            Mouse::SetRightIsPressed(false);
            break;
        }
        case WM_MOUSEWHEEL: {
            const auto [x, y] = MAKEPOINTS(lParam);
            const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
            Mouse::SetWheelDelta(delta);
            break;
        }
        /************ END MOUSE MESSAGES *************/
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
