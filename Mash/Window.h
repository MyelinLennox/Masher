#pragma once

#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window {
public:
	Window();
	Window(const Window&) = delete;
	Window& operator = (const Window&) = delete;
	~Window();

	void Render();

	bool ProcessMessages();
	HWND getHandle() const { return m_hWnd; }
private :
	HINSTANCE m_hInstance;
	HWND m_hWnd;
};