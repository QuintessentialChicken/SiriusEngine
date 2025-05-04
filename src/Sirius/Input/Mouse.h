//
// Created by Leon on 17/01/2025.
//

#ifndef MOUSE_H
#define MOUSE_H

class Mouse {
public:
    static int GetX() noexcept;
    static int GetY() noexcept;
    static bool IsInWindow() noexcept;
    static bool LeftIsPressed() noexcept;
    static bool RightIsPressed() noexcept;

    static void SetPos(int x, int y) noexcept;
    static void SetIsInWindow(bool isInWindow) noexcept;
    static void SetLeftIsPressed(bool leftIsPressed) noexcept;
    static void SetRightIsPressed(bool rightIsPressed) noexcept;
    static void SetWheelDelta(int wheelDelta) noexcept;
    static void SetRawEnabled(bool rawEnabled) noexcept;
private:
    static int x, y;
    static bool leftIsPressed, rightIsPressed;
    static bool isInWindow;
    static int wheelDeltaCarry;
    static bool rawEnabled;
};

#endif //MOUSE_H
