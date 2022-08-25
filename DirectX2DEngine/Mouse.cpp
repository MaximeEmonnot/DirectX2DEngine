#include "Mouse.h"

// STATIC VARIABLES INITIALIZATION
std::unique_ptr<Mouse> Mouse::pInstance = nullptr;

Mouse::~Mouse()
{
	FlushBuffer();
}

Mouse& Mouse::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<Mouse>();
	return *pInstance;
}

Mouse::EventType Mouse::Read() const
{
	return buffer.empty() ? EventType::None : buffer.front();
}

FVec2D Mouse::GetPosition() const
{
	return position;
}

bool Mouse::LeftIsPressed() const
{
	return bLeftPressed;
}

bool Mouse::MiddleIsPressed() const
{
	return bMiddlePressed;
}

bool Mouse::RightIsPressed() const
{
	return bRightPressed;
}

void Mouse::OnMouseMove(int x, int y)
{
	buffer.push(EventType::Move);
	position = IVec2D(x, y);
	TrimBuffer();
}

void Mouse::OnLeftPressed()
{
	bLeftPressed = true;
	buffer.push(EventType::LPress);
	TrimBuffer();
}

void Mouse::OnLeftReleased()
{
	bLeftPressed = false;
	buffer.push(EventType::LRelease);
	TrimBuffer();
}

void Mouse::OnMiddlePressed()
{
	bMiddlePressed = true;
	buffer.push(EventType::MPress);
	TrimBuffer();
}

void Mouse::OnMiddleReleased()
{
	bMiddlePressed = false;
	buffer.push(EventType::MRelease);
	TrimBuffer();
}

void Mouse::OnRightPressed()
{
	bRightPressed = true;
	buffer.push(EventType::RPress);
	TrimBuffer();
}

void Mouse::OnRightReleased()
{
	bRightPressed = false;
	buffer.push(EventType::RRelease);
	TrimBuffer();
}

void Mouse::OnWheelUp()
{
	buffer.push(EventType::WheelUp);
	TrimBuffer();
}

void Mouse::OnWheelDown()
{
	buffer.push(EventType::WheelDown);
	TrimBuffer();
}

void Mouse::FlushBuffer()
{
	buffer = std::queue<EventType>();
}

void Mouse::TrimBuffer()
{
	if (buffer.size() > sizeBuffer) buffer.pop();
}

void Mouse::PopLastEvent()
{
	if (!buffer.empty()) buffer.pop();
}
