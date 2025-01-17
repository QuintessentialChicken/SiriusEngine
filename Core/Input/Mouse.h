//
// Created by Leon on 17/01/2025.
//

#ifndef MOUSE_H
#define MOUSE_H
#include <optional>
#include <queue>
#include <utility>
#include <winuser.h>



class Mouse {
    friend class Window;

public:
    struct RawDelta {
        int x, y;
    };

    class Event {
    public:
        enum class Type {
            LPress,
            LRelease,
            RPress,
            RRelease,
            WheelUp,
            WheelDown,
            Move,
            Enter,
            Leave,
            Invalid
        };
        Event() noexcept : type( Type::Invalid ), leftIsPressed( false ), rightIsPressed( false ), x( 0 ), y( 0 ) {}
        Event(Type type, const Mouse &parent) noexcept : type{type}, leftIsPressed{parent.leftIsPressed}, rightIsPressed{parent.rightIsPressed}, x{parent.x}, y{parent.y} {}

        [[nodiscard]] Type GetType() const noexcept {
            return type;
        }

        [[nodiscard]] std::pair<int, int> GetPos() const noexcept {
            return {x, y};
        }

        [[nodiscard]] int GetPosX() const noexcept {
            return x;
        }

        [[nodiscard]] int GetPosY() const noexcept {
            return y;
        }

        [[nodiscard]] bool LeftIsPressed() const noexcept {
            return leftIsPressed;
        }

        [[nodiscard]] bool RightIsPressed() const noexcept {
            return rightIsPressed;
        }

    private:
        Type type;
        bool leftIsPressed;
        bool rightIsPressed;
        int x;
        int y;
    };

    Mouse() = default;

    Mouse(const Mouse &) = delete;

    Mouse &operator=(const Mouse &) = delete;

    [[nodiscard]] std::pair<int, int> GetPos() const noexcept;

    std::optional<RawDelta> ReadRawDelta() noexcept;

    [[nodiscard]] int GetPosX() const noexcept;

    [[nodiscard]] int GetPosY() const noexcept;

    [[nodiscard]] bool IsInWindow() const noexcept;

    [[nodiscard]] bool LeftIsPressed() const noexcept;

    [[nodiscard]] bool RightIsPressed() const noexcept;

    [[nodiscard]] Event Read() noexcept;

    [[nodiscard]] bool IsEmpty() const noexcept;

    void Clear() noexcept;

    void EnableRaw() noexcept;

    void DisableRaw() noexcept;

    [[nodiscard]] bool RawEnabled() const noexcept;

private:
    void OnMouseMove(int x, int y) noexcept;

    void OnMouseLeave() noexcept;

    void OnMouseEnter() noexcept;

    void OnRawDelta(int dx, int dy) noexcept;

    void OnLeftPressed(int x, int y) noexcept;

    void OnLeftReleased(int x, int y) noexcept;

    void OnRightPressed(int x, int y) noexcept;

    void OnRightReleased(int x, int y) noexcept;

    void OnWheelUp(int x, int y) noexcept;

    void OnWheelDown(int x, int y) noexcept;

    void TrimBuffer() noexcept;

    void TrimRawInputBuffer() noexcept;

    void OnWheelDelta(int x, int y, int delta) noexcept;

    static constexpr unsigned int bufferSize = 16u;
    int x;
    int y;
    bool leftIsPressed = false;
    bool rightIsPressed = false;
    bool isInWindow = false;
    int wheelDeltaCarry = 0;
    bool rawEnabled = false;
    std::queue<Event> buffer;
    std::queue<RawDelta> rawDeltaBuffer;
};


#endif //MOUSE_H
