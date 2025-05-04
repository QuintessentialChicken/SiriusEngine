#pragma once
#include <queue>
#include <bitset>

// TODO Rework Keyboard Class
class Keyboard {
    friend class Window;

    public:
	class Event
	{
	public:
		enum class Type
		{
			Press,
			Release,
			Invalid
		};
	private:
		Type type;
		unsigned char code;
	public:
		Event() noexcept : type(Type::Invalid), code(0) {};
        Event(Type type, unsigned char code) noexcept : type(type), code(code) {}
        [[nodiscard]] bool IsPress() const noexcept { return type == Type::Press; }
        [[nodiscard]] bool IsRelease() const noexcept { return type == Type::Release; }
        [[nodiscard]] bool IsValid() const noexcept { return type != Type::Invalid; }
        [[nodiscard]] unsigned char GetCode() const noexcept { return code; }
	};
public:
	Keyboard() = default;
	Keyboard( const Keyboard& ) = delete;
	Keyboard& operator=( const Keyboard& ) = delete;
	// key event stuff
	bool KeyIsPressed( unsigned char keycode ) const noexcept;
	Event ReadKey() noexcept;
	bool KeyIsEmpty() const noexcept;
	void ClearKey() noexcept;
	// char event stuff
	char ReadChar() noexcept;
	bool CharIsEmpty() const noexcept;
	void ClearChar() noexcept;
	void Clear() noexcept;
	// autorepeat control
	void EnableAutorepeat() noexcept;
	void DisableAutorepeat() noexcept;
	bool AutorepeatIsEnabled() const noexcept;
private:
	void OnKeyPressed( unsigned char keycode ) noexcept;
	void OnKeyReleased( unsigned char keycode ) noexcept;
	void OnChar( char character ) noexcept;
	void ClearState() noexcept;
	template<typename T>
	static void TrimBuffer( std::queue<T>& buffer ) noexcept;
private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;
	bool autorepeatEnabled = false;
	std::bitset<nKeys> keyStates;
	std::queue<Event> keyBuffer;
	std::queue<char> charBuffer;
};


