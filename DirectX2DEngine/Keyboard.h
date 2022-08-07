#pragma once
#include <memory>
#include <bitset>
#include <queue>

#define KBD Keyboard::GetInstance()

class Keyboard
{
private:
	friend class Window;
public:
	struct Event
	{
		enum class Type
		{
			Pressed,
			Released,
			None
		};
		unsigned char keycode = 0u;
		Type type = Type::None;
	};
public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
	~Keyboard();

	static Keyboard& GetInstance();

	bool KeyIsPressed(unsigned char keycode) const;
	Keyboard::Event ReadKey() const;
	char ReadChar() const;
	bool KeyIsEmpty() const;

private:
	void OnKeyPressed(unsigned char code);
	void OnKeyReleased(unsigned char code);
	void OnChar(char code);

	void FlushBuffers();
	template <typename T> void TrimBuffer(std::queue<T>& buffer);

	void PopLastEvents();

private:
	static std::unique_ptr<Keyboard> pInstance;

	const unsigned int bufferSize = 4u;
	std::bitset<256> keyStates;
	std::queue<Event> eventBuffer;
	std::queue<char> charBuffer;
};
