#include "Window.h"

// STATIC VARIABLES INITIALIZATION
std::unique_ptr<Window> Window::pInstance = nullptr;

HINSTANCE Window::hInstance;
std::wstring Window::className;

// CLASS DEFINITION
Window::Window() noexcept
{
	// Initialization
	hInstance = GetModuleHandle(nullptr);
	className = L"DirectX 2D Engine";

	// Window Class Registration
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.

}

Window::~Window()
{
	DestroyWindow(hWnd);
}

Window& Window::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<Window>();
	return *pInstance;
}
