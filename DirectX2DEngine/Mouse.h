#pragma once
#include <memory>
#include <queue>

#define MOUSE Mouse::GetInstance()

class Mouse
{
private:
	friend class Window;
public:
	enum class EventType
	{
		LPress,
		LRelease,
		MPress,
		MRelease,
		RPress,
		RRelease,
		WheelUp,
		WheelDown,
		Move,
		None
	};
public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator= (const Mouse&) = delete;
	~Mouse();

	static Mouse& GetInstance();

	Mouse::EventType Read() const;

	bool LeftIsPressed() const;
	bool MiddleIsPressed() const;
	bool RightIsPressed() const;

private:
	void OnMouseMove(int x, int y);
	void OnLeftPressed();
	void OnLeftReleased();
	void OnMiddlePressed();
	void OnMiddleReleased();
	void OnRightPressed();
	void OnRightReleased();
	void OnWheelUp();
	void OnWheelDown();

	void FlushBuffer();
	void TrimBuffer();

	void PopLastEvent();

private:
	static std::unique_ptr<Mouse> pInstance;

	bool bLeftPressed = false;
	bool bMiddlePressed = false;
	bool bRightPressed = false;

	const size_t sizeBuffer = 4u;
	std::queue<EventType> buffer;
};

