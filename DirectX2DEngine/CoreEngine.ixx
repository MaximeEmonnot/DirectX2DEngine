module;

#include <Windows.h>

export module CoreEngine;

import <memory>;
import <queue>;
import <bitset>;
import <condition_variable>;
import <thread>;
import <mutex>;
import <functional>;
import <chrono>;


import MathEngine;
import DebugEngine;
import NetworkEngine;

export
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
	~Mouse()
	{
		FlushBuffer();
	}

	static Mouse& GetInstance()
	{
		if (!pInstance)
			pInstance = std::make_unique<Mouse>();
		return *pInstance;
	}

	// Reads the last frame's Mouse event.
	Mouse::EventType	Read() const
	{
		return buffer.empty() ? EventType::None : buffer.front();
	}

	FVec2D				GetPosition() const
	{
		return position;
	}

	bool				LeftIsPressed() const
	{
		return bLeftPressed;
	}
	bool				MiddleIsPressed() const
	{
		return bMiddlePressed;
	}
	bool				RightIsPressed() const
	{
		return bRightPressed;
	}

	void				OnMouseMove(int x, int y)
	{
		buffer.push(EventType::Move);
		position = IVec2D(x, y);
		TrimBuffer();
	}
	void				OnLeftPressed()
	{
		bLeftPressed = true;
		buffer.push(EventType::LPress);
		TrimBuffer();
	}
	void				OnLeftReleased()
	{
		bLeftPressed = false;
		buffer.push(EventType::LRelease);
		TrimBuffer();
	}
	void				OnMiddlePressed()
	{
		bMiddlePressed = true;
		buffer.push(EventType::MPress);
		TrimBuffer();
	}
	void				OnMiddleReleased()
	{
		bMiddlePressed = false;
		buffer.push(EventType::MRelease);
		TrimBuffer();
	}
	void				OnRightPressed()
	{
		bRightPressed = true;
		buffer.push(EventType::RPress);
		TrimBuffer();
	}
	void				OnRightReleased()
	{
		bRightPressed = false;
		buffer.push(EventType::RRelease);
		TrimBuffer();
	}
	void				OnWheelUp()
	{
		buffer.push(EventType::WheelUp);
		TrimBuffer();
	}
	void				OnWheelDown()
	{
		buffer.push(EventType::WheelDown);
		TrimBuffer();
	}

	void				FlushBuffer()
	{
		buffer = std::queue<EventType>();
	}
	void				TrimBuffer()
	{
		if (buffer.size() > sizeBuffer) buffer.pop();
	}

	void				PopLastEvent()
	{
		if (!buffer.empty()) buffer.pop();
	}

private:
	static std::unique_ptr<Mouse>	pInstance;

	FVec2D							position = FVec2D(0.f, 0.f);
	bool							bLeftPressed = false;
	bool							bMiddlePressed = false;
	bool							bRightPressed = false;
	const size_t					sizeBuffer = 4u;
	std::queue<EventType>			buffer;
};
// STATIC VARIABLES INITIALIZATION
std::unique_ptr<Mouse> Mouse::pInstance = nullptr;

export Mouse& MOUSE = Mouse::GetInstance();
export FVec2D MOUSE_POSITION = MOUSE.GetPosition();


export
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
		unsigned char	keycode = 0u;
		Type			type = Type::None;
	};
public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
	~Keyboard()
	{
		FlushBuffers();
	}

	static Keyboard& GetInstance()
	{
		if (!pInstance)
			pInstance = std::make_unique<Keyboard>();
		return *pInstance;
	}

	// Returns the status (Pressed / Released) of the key associated with the keycode.
	bool				KeyIsPressed(unsigned char keycode) const
	{
		return keyStates[keycode];
	}

	// Reads the last frame key stroke.
	Keyboard::Event		ReadKey() const
	{
		return eventBuffer.empty() ? Event() : eventBuffer.front();
	}

	// Reads the last frame character typed.
	char				ReadChar() const
	{
		return charBuffer.empty() ? 0 : charBuffer.front();
	}

	// Returns the event buffer's emptiness.
	bool				KeyIsEmpty() const
	{
		return eventBuffer.empty();
	}

	void				OnKeyPressed(unsigned char code)
	{
		keyStates[code] = true;
		eventBuffer.push({ code, Event::Type::Pressed });
		TrimBuffer(eventBuffer);
	}
	void				OnKeyReleased(unsigned char code)
	{
		keyStates[code] = false;
		eventBuffer.push({ code, Event::Type::Released });
		TrimBuffer(eventBuffer);
	}
	void				OnChar(char code)
	{
		charBuffer.push(code);
		TrimBuffer(charBuffer);
	}

	void				FlushBuffers()
	{
		eventBuffer = std::queue<Event>();
		charBuffer = std::queue<char>();
	}
	template <typename T>
	void				TrimBuffer(std::queue<T>& buffer)
	{
		if (buffer.size() > bufferSize) buffer.pop();
	}

	void				PopLastEvents()
	{
		if (!eventBuffer.empty()) eventBuffer.pop();
		if (!charBuffer.empty()) charBuffer.pop();
	}

private:
	static std::unique_ptr<Keyboard>	pInstance;

	const unsigned int					bufferSize = 4u;
	std::bitset<256>					keyStates;
	std::queue<Event>					eventBuffer;
	std::queue<char>					charBuffer;
};

// STATIC VARIABLES INITIALIZATION
std::unique_ptr<Keyboard> Keyboard::pInstance = nullptr;

export Keyboard& KBD = Keyboard::GetInstance();



export
class ThreadPool
{
public:
	ThreadPool()
	{
		for (size_t i = 0; i < numThreads; ++i) {
			threads.emplace_back([&] {
				while (true) {
					std::function<void()> task;
					{
						// We wait for a notify...
						std::unique_lock<std::mutex> lock(eventMutex);
						eventVar.wait(lock, [&] { return bStopping || !tasks.empty(); });
						if (bStopping) break;

						//...so that we can execute a new task
						task = std::move(tasks.front());
						tasks.pop();
					}
					try {
						task();
					}
					catch (EngineException& e)
					{
						SHOW_MESSAGE("DirectX 2D Engine - Engine Exception", e.what());
					}
					catch (std::exception& e)
					{
						SHOW_MESSAGE("DirectX 2D Engine - Standard Library Exception", e.what());
					}
					catch (...)
					{
						SHOW_MESSAGE("DirectX 2D Engine - Unknown Exception", "An unknown exception has been caught.");
					}
				}
				});
		}
	}
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
	~ThreadPool()
	{
		{
			std::unique_lock<std::mutex> lock(eventMutex);
			bStopping = true;
		}
		// We notify all waiting threads so we can make them join safely
		eventVar.notify_all();

		for (std::thread& thread : threads) thread.join();
	}

	static ThreadPool& GetInstance()
	{
		if (!pInstance)
			pInstance = std::make_unique<ThreadPool>();
		return *pInstance;
	}

	// Enqueues a task that will be executed by one of the threads in the pool.
	void				Enqueue(std::function<void()> task)
	{
		{
			std::unique_lock<std::mutex> lock(eventMutex);
			tasks.emplace(std::move(task));
		}
		eventVar.notify_one();
	}

private:
	static std::unique_ptr<ThreadPool>	pInstance;

	const size_t						numThreads = 8u;
	std::vector<std::thread>			threads;
	std::queue<std::function<void()>>	tasks;
	std::condition_variable				eventVar;
	std::mutex							eventMutex;
	bool								bStopping = false;
};

// STATIC VARIABLES INITIALIZATION
std::unique_ptr<ThreadPool> ThreadPool::pInstance = nullptr;

export ThreadPool& THREAD = ThreadPool::GetInstance();



export
class Timer {
public:
	Timer()
		:
		last(std::chrono::steady_clock::now()),
		deltaTime(0.f)
	{}
	Timer(const Timer&) = delete;
	Timer& operator= (const Timer&) = delete;

	static Timer& GetInstance()
	{
		if (!pInstance)
			pInstance = std::make_unique<Timer>();
		return *pInstance;
	}

	void			Update()
	{
		const std::chrono::steady_clock::time_point old = last;
		last = std::chrono::steady_clock::now();
		deltaTime = std::chrono::duration<float>(last - old).count();
	}

	float			DeltaTime() const
	{
		return deltaTime;
	}
private:
	static std::unique_ptr<Timer>			pInstance;

	std::chrono::steady_clock::time_point	last;
	float									deltaTime;
};
// STATIC VARIABLES INITIALIZATION
std::unique_ptr<Timer> Timer::pInstance = nullptr;


// Updates the DELTATIME value.
export void TICKCLOCK() { Timer::GetInstance().Update(); }
export float DELTATIME = Timer::GetInstance().DeltaTime();



export
class Window
{
private:
	friend class Graphics;
public:
	Window() noexcept
	{
		// Initialization
		hInstance = GetModuleHandle(nullptr);
		className = L"DirectX 2D Engine";

		// Window Class Registration
		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(WNDCLASSEX));

		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = WindowProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = nullptr;
		wc.hCursor = nullptr;
		wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = className.c_str();
		wc.hIconSm = nullptr;

		RegisterClassEx(&wc);

		// Window Creation

		RECT wr;
		wr.left = 0;
		wr.top = 0;
		wr.right = width + wr.left;
		wr.bottom = height + wr.top;
		AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

		hWnd = CreateWindow(
			className.c_str(),
			L"DirectX 2D Engine - Emonnot",
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
			nullptr,
			nullptr,
			hInstance,
			nullptr
		);

		ShowWindow(hWnd, SW_SHOWDEFAULT);
	}
	~Window()
	{
		DestroyWindow(hWnd);
	}
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	static Window& GetInstance()
	{
		if (!pInstance)
			pInstance = std::make_unique<Window>();
		return *pInstance;
	}

	// Processes any Windows events. Returns always true until a closing event is caught.
	bool						ProcessMessages()
	{
		MSG msg;
		MOUSE.PopLastEvent();
		KBD.PopLastEvents();
		ZeroMemory(&msg, sizeof(MSG));
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT) return false;
		}
		return true;
	}

	int							GetWidth() const
	{
		return width;
	}
	int							GetHeight() const
	{
		return height;
	}

	float						GetWidthHeightRatio() const
	{
		return static_cast<float>(width) / static_cast<float>(height);
	}

	HWND						GetHWND() const
	{
		return hWnd;
	}

	bool						IsPlaying() const
	{
		return bIsPlaying;
	}

private:
	static LRESULT CALLBACK		WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
			//*** CLOSE WINDOW EVENTS ***//
		case WM_CLOSE:
		case WM_DESTROY:
			PostQuitMessage(0);
			bIsPlaying = false;
			NETWORK.Disconnect();
			return 0;
			//*** END CLOSE WINDOW EVENTS ***//

		// We keep updating the DeltaTime value to avoid accidents when the window doesn't move anymore (i.e. absurd DeltaTime values used for physics)
		case WM_MOVING:
			TICKCLOCK();
			break;

			//*** KEYBOARD EVENTS ***//
		case WM_KEYDOWN:
		{
			KBD.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
		case WM_KEYUP:
		{
			KBD.OnKeyReleased(static_cast<unsigned char>(wParam));
		}
		break;
		case WM_CHAR:
		{
			KBD.OnChar(static_cast<char>(wParam));
		}
		break;
		//*** END KEYBOARD EVENTS ***//


		//*** MOUSE EVENTS ***//
		case WM_MOUSEMOVE:
		{
			const int x = LOWORD(lParam);
			const int y = HIWORD(lParam);
			MOUSE.OnMouseMove(x - width / 2, -y + height / 2);
		}
		break;
		case WM_LBUTTONDOWN:
		{
			MOUSE.OnLeftPressed();
		}
		break;
		case WM_LBUTTONUP:
		{
			MOUSE.OnLeftReleased();
		}
		break;
		case WM_MBUTTONDOWN:
		{
			MOUSE.OnMiddlePressed();
		}
		break;
		case WM_MBUTTONUP:
		{
			MOUSE.OnMiddleReleased();
		}
		break;
		case WM_RBUTTONDOWN:
		{
			MOUSE.OnRightPressed();
		}
		break;
		case WM_RBUTTONUP:
		{
			MOUSE.OnRightReleased();
		}
		break;
		case WM_MOUSEWHEEL:
		{
			if (GET_WHEEL_DELTA_WPARAM(wParam) >= 0) MOUSE.OnWheelUp();
			else MOUSE.OnWheelDown();
		}
		break;
		//*** END MOUSE EVENTS ***//

		default:
			break;
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

private:

	// Class Instance
	static std::unique_ptr<Window>	pInstance;

public:
	// Window Creation Variables
	HWND							hWnd;
	static HINSTANCE				hInstance;
	static std::wstring				className;

	// Window Constants
	static constexpr int			width = 800;
	static constexpr int			height = 640;

	static bool						bIsPlaying;
};
// STATIC VARIABLES INITIALIZATION
std::unique_ptr<Window> Window::pInstance = nullptr;
HINSTANCE Window::hInstance;
std::wstring Window::className;
bool Window::bIsPlaying = true;


export Window& WND = Window::GetInstance();