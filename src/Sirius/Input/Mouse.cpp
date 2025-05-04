//
// Created by Leon on 17/01/2025.
//
#include <windows.h>

#include "Mouse.h"

int Mouse::x, Mouse::y;
bool Mouse::leftIsPressed, Mouse::rightIsPressed;
bool Mouse::isInWindow;
int Mouse::wheelDeltaCarry;
bool Mouse::rawEnabled;

int Mouse::GetX() noexcept {
    return x;
}

int Mouse::GetY() noexcept {
    return y;
}

bool Mouse::IsInWindow() noexcept {
    return isInWindow;
}

bool Mouse::LeftIsPressed() noexcept {
    return leftIsPressed;
}

bool Mouse::RightIsPressed() noexcept {
    return rightIsPressed;
}

void Mouse::SetPos(int x, int y) noexcept {
    Mouse::x = x;
    Mouse::y = y;
}

void Mouse::SetIsInWindow(bool isInWindow) noexcept {
    Mouse::isInWindow = isInWindow;
}

void Mouse::SetLeftIsPressed(bool leftIsPressed) noexcept {
    Mouse::leftIsPressed = leftIsPressed;
}

void Mouse::SetRightIsPressed(bool rightIsPressed) noexcept {
    Mouse::rightIsPressed = rightIsPressed;
}

void Mouse::SetWheelDelta(int wheelDelta) noexcept {
    while( wheelDeltaCarry >= WHEEL_DELTA )
    {
        wheelDeltaCarry -= WHEEL_DELTA;
    }
    while( wheelDeltaCarry <= -WHEEL_DELTA )
    {
        wheelDeltaCarry += WHEEL_DELTA;
    }
}

void Mouse::SetRawEnabled(bool rawEnabled) noexcept {
}
