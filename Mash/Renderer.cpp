#include "Renderer.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>

void Renderer::setPixel(int x, int y, const RGBColor& color)
{
	std::cout << "(Renderer.cpp) -- setPixel() called" << std::endl;

	BitmapBuffer& buffer = getInstance().buffer;

	if (x < 0 || x > buffer.width || y < 0 || y > buffer.height) {
		return;
	}

	uint32_t colorValue = (color.red << 16) | (color.green << 8) | (color.blue << 0);

	uint8_t* row = (uint8_t*)buffer.memory + x * bytesPerPixel + y * buffer.pitch;
	uint32_t* pixel = (uint32_t*)row;
	*pixel = colorValue;
}

void Renderer::fillRect(const Rect& rect, const RGBColor& color)
{
	std::cout << "(Renderer.cpp) -- fillRect() called" << std::endl;
	BitmapBuffer& buffer = getInstance().buffer;

	int minX = rect.x;
	int minY = rect.y;
	int maxX = rect.x + rect.width;
	int maxY = rect.y + rect.height;

	if (minX < 0) minX = 0;
	if (minY < 0) minY = 0;
	if (maxX > buffer.width) maxX = buffer.width;
	if (maxY > buffer.height) maxY = buffer.height;

	uint32_t rawColor = (color.red << 16) | (color.green << 8) | (color.blue << 0);

	uint8_t* row = (uint8_t*)buffer.memory + minX * bytesPerPixel + minY * buffer.pitch;
	for (int y = minY; y < maxY; y++)
	{
		uint32_t* pixel = (uint32_t*)row;
		for (int x = minX; x < maxX; x++)
		{
			*pixel++ = rawColor;
		}
		row += buffer.pitch;
	}
}

void Renderer::fillRoundedRect(const RoundedRect& roundedRect, const RGBColor& color)
{
	std::cout << "(Renderer.cpp) -- fillRoundedRect() called" << std::endl;
	BitmapBuffer& buffer = getInstance().buffer;
}

void Renderer::fillGradient(const Rect& rect, const RGBColor& color1, const RGBColor& color2)
{
	std::cout << "(Renderer.cpp) -- fillGradient() called" << std::endl;

	BitmapBuffer& buffer = getInstance().buffer;

	int minX = rect.x;
	int minY = rect.y;
	int maxX = rect.x + rect.width;
	int maxY = rect.y + rect.height;

	if (minX < 0) minX = 0;
	if (minY < 0) minY = 0;
	if (maxX > buffer.width) maxX = buffer.width;
	if (maxY > buffer.height) maxY = buffer.height;

	uint8_t* row = (uint8_t*)buffer.memory + minX * bytesPerPixel + minY * buffer.pitch;

	for (int y = minY; y < maxY; y++)
	{
		uint32_t* pixel = (uint32_t*)row;
		for (int x = minX; x < maxX; x++)
		{
			float magnitude = (y / float(rect.height));

			int red = color1.red + magnitude * (color2.red - color1.red);
			int green = color1.green + magnitude * (color2.green - color1.green);
			int blue = color1.blue + magnitude * (color2.blue - color1.blue);

			uint32_t color = (red << 16) | (green << 8) | (blue << 0);

			*pixel++ = color;
		}
		row += buffer.pitch;
	}
}

void Renderer::getWindowDimensions(int* width, int* height)
{
	std::cout << "(Renderer.cpp) -- getWindowDimensions() called" << std::endl;
	RECT clientRect;
	GetClientRect(getInstance().hWnd, &clientRect);

	*width = clientRect.right - clientRect.left;
	*height = clientRect.bottom - clientRect.top;
}

void Renderer::resizeFrameBuffer(int width, int height)
{
	std::cout << "(Renderer.cpp) -- resizeFrameBuffer() called" << std::endl;

	BitmapBuffer& buffer = getInstance().buffer;

	if (buffer.memory) 
	{
		VirtualFree(buffer.memory, 0, MEM_RELEASE);
	}

	buffer.width = width;
	buffer.height = height;
	buffer.info.bmiHeader.biSize = sizeof(buffer.info.bmiHeader);
	buffer.info.bmiHeader.biWidth = buffer.width;
	buffer.info.bmiHeader.biHeight = -buffer.height;
	buffer.info.bmiHeader.biPlanes = 1;
	buffer.info.bmiHeader.biBitCount = 32;
	buffer.info.bmiHeader.biCompression = BI_RGB;
	buffer.pitch = buffer.width * bytesPerPixel;

	int bufferSize = buffer.width * buffer.height * bytesPerPixel;
	buffer.memory = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	buffer.pitch = buffer.width * bytesPerPixel;
}

void Renderer::copyBufferToWindow(HDC deviceContext, int width, int height)
{
	std::cout << "(Renderer.cpp) -- copyBufferToWindow() called" << std::endl;

	BitmapBuffer& buffer = getInstance().buffer;

	StretchDIBits(
		deviceContext,
		0, 0, width, height,
		0, 0, buffer.width, buffer.height,
		buffer.memory,
		&buffer.info,
		DIB_RGB_COLORS,
		SRCCOPY
	);
}

void Renderer::clearBuffer()
{
	std::cout << "(Renderer.cpp) -- clearBuffer() called" << std::endl;

	BitmapBuffer& buffer = getInstance().buffer;

	fillRect({ 0, 0, buffer.width, buffer.height }, getInstance().clearColor);
}

void Renderer::setClearColor(const RGBColor& color)
{
	std::cout << "(Renderer.cpp) -- setClearColor called" << std::endl;
	getInstance().clearColor = color;
}