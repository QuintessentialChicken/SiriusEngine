//
// Created by Leon on 17/01/2025.
//
#include <Windows.h>

#include "Mouse.h"



std::pair<int, int> Mouse::GetPos() const noexcept {
    return {x, y};
}

std::optional<Mouse::RawDelta> Mouse::ReadRawDelta() noexcept {
    if (rawDeltaBuffer.empty()) {
        return std::nullopt;
    }
    const RawDelta d = rawDeltaBuffer.front();
    rawDeltaBuffer.pop();
    return d;
}

int Mouse::GetPosX() const noexcept {
    return x;
}

int Mouse::GetPosY() const noexcept {
    return y;
}

bool Mouse::IsInWindow() const noexcept {
    return isInWindow;
}

bool Mouse::LeftIsPressed() const noexcept {
    return leftIsPressed;
}

bool Mouse::RightIsPressed() const noexcept {
    return rightIsPressed;
}

Mouse::Event Mouse::Read() noexcept {
    if (buffer.empty()) {
        return {};
    }
    const Event event{buffer.front()};
    buffer.pop();
    return event;
}

bool Mouse::IsEmpty() const noexcept {
    return buffer.empty();
}

void Mouse::Clear() noexcept {
    buffer = std::queue<Event>();
}

void Mouse::EnableRaw() noexcept {
    rawEnabled = true;
}

void Mouse::DisableRaw() noexcept {
    rawEnabled = false;
}

bool Mouse::RawEnabled() const noexcept {
    return rawEnabled;
}

void Mouse::OnMouseMove(const int x, const int y) noexcept {
    this->x = x;
    this->y = y;

    buffer.emplace(Event::Type::Move, *this);
    TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept {
    isInWindow = false;
    buffer.emplace( Event::Type::Leave,*this );
    TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept {
    isInWindow = true;
    buffer.emplace( Event::Type::Enter,*this );
    TrimBuffer();
}

void Mouse::OnRawDelta(int dx, int dy) noexcept {
    rawDeltaBuffer.emplace(dx, dy);
}

void Mouse::OnLeftPressed(int x, int y) noexcept {
    leftIsPressed = true;
    buffer.emplace(Event::Type::LPress, *this);
    TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y) noexcept {
    leftIsPressed = false;
    buffer.emplace(Event::Type::LRelease, *this);
    TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y) noexcept {
    rightIsPressed = true;
    buffer.emplace(Event::Type::RPress, *this);
    TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y) noexcept {
    rightIsPressed = false;
    buffer.emplace(Event::Type::RRelease, *this);
    TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) noexcept {
    buffer.emplace(Event::Type::WheelUp, *this);
    TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept {
    buffer.emplace(Event::Type::WheelDown, *this);
    TrimBuffer();
}

void Mouse::TrimBuffer() noexcept {
    while (buffer.size() > bufferSize) {
        buffer.pop();
    }
}

void Mouse::TrimRawInputBuffer() noexcept {
    while (rawDeltaBuffer.size() > bufferSize) {
        rawDeltaBuffer.pop();
    }
}

void Mouse::OnWheelDelta(const int x, const int y, const int delta) noexcept {
    wheelDeltaCarry += delta;
    while( wheelDeltaCarry >= WHEEL_DELTA )
    {
        wheelDeltaCarry -= WHEEL_DELTA;
        OnWheelUp( x,y );
    }
    while( wheelDeltaCarry <= -WHEEL_DELTA )
    {
        wheelDeltaCarry += WHEEL_DELTA;
        OnWheelDown( x,y );
    }
}
