#include "Window.h"
#include "Renderer.h"
#include <iostream>


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) 
	{
	case WM_CLOSE:
		std::cout << "(Window.cpp) -- WM_CLOSE called" << std::endl;
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		std::cout << "(Window.cpp) -- WM_DESTROY called" << std::endl;
		PostQuitMessage(0);
		return 0;
	
	case WM_PAINT:
		std::cout << "(Window.cpp) -- WM_PAINT called" << std::endl;

		PAINTSTRUCT paint;
		HDC deviceContext = BeginPaint(hWnd, &paint);

		int screenWidth, screenHeight;
		Renderer::getWindowDimensions(&screenWidth, &screenHeight);
		Renderer::copyBufferToWindow(deviceContext, screenWidth, screenHeight);

		EndPaint(hWnd, &paint);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

Window::Window() : m_hInstance(GetModuleHandle(nullptr))
{
	std::cout << "(Window.cpp) -- m_hInstance initialization started" << std::endl;
	const wchar_t* CLASS_NAME = L"(Window.cpp) -- Window Class";

	WNDCLASS wndClass = {};
	wndClass.lpszClassName = CLASS_NAME;
	wndClass.hInstance = m_hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_ERROR);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.lpfnWndProc = WindowProc;

	RegisterClass(&wndClass);

	DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	RECT windowSize 
	{ 
		0,     // left
		0,     // top
		1200,  // right
		800    // bottom
	};

	Renderer::resizeFrameBuffer(windowSize.right, windowSize.bottom);

	int width = windowSize.right - windowSize.left;
	int height = windowSize.bottom - windowSize.top;

	AdjustWindowRect(&windowSize, style, FALSE); // fix window size so dimensions apply to canvas instead of border

	m_hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		L"MASHER!",
		style,
		(screenWidth - width) / 2,
		(screenHeight - height) / 2,
		width,
		height,
		NULL,
		NULL,
		m_hInstance,
		NULL
	);

	ShowWindow(m_hWnd, SW_SHOW);

	std::cout << "(Window.cpp) -- m_hWnd window created" << std::endl;
	
	Renderer::setWindowHandle(m_hWnd);
}

Window::~Window()
{
	const wchar_t* CLASS_NAME = L"Window Class";
	UnregisterClass(CLASS_NAME, m_hInstance);

	std::cout << "(Window.cpp) -- Window class unregistered" << std::endl;
}

void Window::Render()
{
	std::cout << "(Window.cpp) -- Render() called" << std::endl;

	LARGE_INTEGER current_counter;
	QueryPerformanceCounter(&current_counter);


	HDC deviceContext = GetDC(m_hWnd);

	int width, height;
	Renderer::getWindowDimensions(&width, &height);
	Renderer::copyBufferToWindow(deviceContext, width, height);

	ReleaseDC(m_hWnd, deviceContext);
}

bool Window::processMessages()
{

	MSG msg = {};

	while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)) 
	{
		if (msg.message == WM_QUIT)
		{
			return false;
		}

		if (msg.message == WM_KEYDOWN)
		{
			std::cout << "(Window.cpp) -- Key pressed: " << msg.wParam << std::endl;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}