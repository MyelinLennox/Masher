#pragma once

#include <Windows.h>
#include <stdint.h>

struct FPoint
{
	float x, y;
};

struct RGBColor
{
	uint8_t red, green, blue;
};

struct FillGradient
{
	POINT start, end;
	RGBColor color1, color2;
};

struct Rect
{
	int x, y, width, height;
};

struct RoundedRect // conflict with win api "RoundRect", simple rename works for now, fix later
{
	int x, y, width, height, radius;
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
	static void setClearColor(const RGBColor& color);

	static void setPixel(int x, int y, const RGBColor& color);

	static void fillRect(const Rect& rect, const RGBColor& color);

	static void fillRoundedRect(const RoundedRect& roundedRect, const RGBColor& color);

	static void fillGradient(const Rect& rect, const RGBColor& color1, const RGBColor& color2);


private:
	
	Renderer() : hWnd(nullptr) { buffer = {}; clearColor = { 255,255,255 }; }
	//Renderer() { buffer = {}; clearColor = { 255, 255, 255 }; }

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

