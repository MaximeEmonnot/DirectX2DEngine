#include "Keyboard.h"

// STATIC VARIABLES INITIALIZATION
std::unique_ptr<Keyboard> Keyboard::pInstance = nullptr;

Keyboard::~Keyboard()
{
	FlushBuffers();
}

Keyboard& Keyboard::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<Keyboard>();
	return *pInstance;
}

bool Keyboard::KeyIsPressed(unsigned char keycode) const
{
	return keyStates[keycode];
}

Keyboard::Event Keyboard::ReadKey() const
{
	return eventBuffer.empty() ? Event() : eventBuffer.front();
}

char Keyboard::ReadChar() const
{
	return charBuffer.empty() ? 0 : charBuffer.front();
}

bool Keyboard::KeyIsEmpty() const
{
	return eventBuffer.empty();
}

void Keyboard::OnKeyPressed(unsigned char code)
{
	keyStates[code] = true;
	eventBuffer.push({ code, Event::Type::Pressed });
	TrimBuffer(eventBuffer);
}

void Keyboard::OnKeyReleased(unsigned char code)
{
	keyStates[code] = false;
	eventBuffer.push({ code, Event::Type::Released });
	TrimBuffer(eventBuffer);
}

void Keyboard::OnChar(char code)
{
	charBuffer.push(code);
	TrimBuffer(charBuffer);
}

void Keyboard::FlushBuffers()
{
	eventBuffer = std::queue<Event>();
	charBuffer = std::queue<char>();
}

void Keyboard::PopLastEvents()
{
	if (!eventBuffer.empty()) eventBuffer.pop();
	if (!charBuffer.empty()) charBuffer.pop();
}

template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer)
{
	if (buffer.size() > bufferSize) buffer.pop();
}