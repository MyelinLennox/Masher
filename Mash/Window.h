#pragma once

#include <Windows.h>
#include <functional>
#include <iostream>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window {
private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;

public:
	Window();
	Window(const Window&) = delete;
	Window& operator = (const Window&) = delete;
	~Window();

	std::function<void(float delta)> update;

	inline static Window& getInstance()
	{
		std::cout << "(Window.h) -- getInstance(): called" << std::endl;

		static Window window;
		return window;
	}

	bool processMessages();
	HWND getHandle() const { return m_hWnd; }

	inline static void setGameUpdate(const std::function<void(float delta)>& update)
	{ 
		std::cout << "(Window.h) -- setGameUpdate() called" << std::endl;

		getInstance().update = update;
	}

	void Render();
};