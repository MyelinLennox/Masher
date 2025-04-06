#pragma once

#include <windows.h>
#include <stdint.h>

struct RGBColor
{
	uint8_t red, green, blue;
};

struct Rect
{
	int x, y, width, height;
};

class Renderer
{
	friend LRESULT CALLBACK WindowProc(
		HWND hWnd, 
		UINT uMsg, 
		WPARAM wParam, 
		LPARAM lParam
	);

	friend class Window;

	struct BitmapBuffer
	{
		int width, height;
		BITMAPINFO info;
		void* memory;
		int pitch; //byte form (length of row)
	};

private:
	static const int bytesPerPixel = 4;

	HWND hWnd;
	BitmapBuffer buffer;
	RGBColor clearColor;

public:
	inline static void SetClearColor(const RGBColor& color) { getInstance().clearColor = color; }

	static void SetPixel(int x, int y, const RGBColor& color);

	static void FillRect(const Rect& rect, const RGBColor& color);

private:
	Renderer() : hWnd(nullptr) { buffer = {}; clearColor = { 255,255,255 }; }

	Renderer(const Renderer&) = delete;
	Renderer& operator = (const Renderer&) = delete;

	~Renderer() {}

	inline static Renderer& getInstance()
	{
		static Renderer renderer;
		return renderer;
	}

public:
	inline static void setWindowHandle(HWND handle) { getInstance().hWnd = handle; }

	static void getWindowDimensions(int* width, int* height);

	static void resizeFrameBuffer(int width, int height);

	static void copyBufferToWindow(HDC deviceContext, int windowWidth, int windowHeight);

	static void clearBuffer();

};

