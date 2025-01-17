#include "Keyboard.h"

bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept {
    return keyStates[keycode];
}

Keyboard::Event Keyboard::ReadKey() noexcept {
    if (keyBuffer.empty()) {
        return {};
    }
    const Event event = keyBuffer.front();
    keyBuffer.pop();
    return event;
}

bool Keyboard::KeyIsEmpty() const noexcept {
    return keyBuffer.empty();
}

void Keyboard::ClearKey() noexcept {
    keyBuffer = std::queue<Event>();
}

char Keyboard::ReadChar() noexcept {
    if (charBuffer.empty()) {
        return {};
    }
    const char charcode = charBuffer.front();
    charBuffer.pop();
    return charcode;
}

bool Keyboard::CharIsEmpty() const noexcept {
    return charBuffer.empty();
}

void Keyboard::ClearChar() noexcept {
    charBuffer = std::queue<char>();
}

void Keyboard::Clear() noexcept {
    ClearKey();
    ClearChar();
}

void Keyboard::EnableAutorepeat() noexcept {
    autorepeatEnabled = true;
}

void Keyboard::DisableAutorepeat() noexcept {
    autorepeatEnabled = false;
}

bool Keyboard::AutorepeatIsEnabled() const noexcept {
    return autorepeatEnabled;
}

void Keyboard::OnKeyPressed(unsigned char keycode) noexcept {
    keyStates[keycode] = true;
    keyBuffer.emplace(Event::Type::Press, keycode);
    TrimBuffer(keyBuffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode) noexcept {
    keyStates[keycode] = false;
    keyBuffer.emplace(Event::Type::Release, keycode);
    TrimBuffer(keyBuffer);
}

void Keyboard::ClearState() noexcept {
    keyStates.reset();
}

void Keyboard::OnChar(char character) noexcept {
    charBuffer.emplace(character);
    TrimBuffer(charBuffer);
}

template<typename T>
void Keyboard::TrimBuffer(std::queue<T> &buffer) noexcept {
    while( buffer.size() > bufferSize )
    {
        buffer.pop();
    }
}
