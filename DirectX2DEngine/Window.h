#pragma once
#include <Windows.h>
#include <memory>
#include <string>

#include "Mouse.h"
#include "Keyboard.h"

#define WND Window::GetInstance()

class Window
{
private:
	friend class Graphics;
public:
	Window() noexcept;
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	static Window& GetInstance();

	bool ProcessMessages();

	int GetWidth() const;
	int GetHeight() const;

	float GetRatio() const;

	HWND GetHWND() const;

	bool IsPlaying() const;

private:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:

	// Class Instance
	static std::unique_ptr<Window> pInstance;

	// Window Creation Variables
	HWND hWnd;
	static HINSTANCE hInstance;
	static std::wstring className;

	// Window Constants
	static constexpr int width = 800;
	static constexpr int height = 640;

	static bool bIsPlaying;
};

