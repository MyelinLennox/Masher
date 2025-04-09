#include "Renderer.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>

void Renderer::SetPixel(int x, int y, const RGBColor& color)
{
	BitmapBuffer& buffer = getInstance().buffer;

	if (x < 0 || x > buffer.width || y < 0 || y > buffer.height)
		return;

	uint32_t colorValue = (color.red << 16) | (color.green << 8) | (color.blue << 0);

	uint8_t* row = (uint8_t*)buffer.memory + x * bytesPerPixel + y * buffer.pitch;
	uint32_t* pixel = (uint32_t*)row;
	*pixel = colorValue;
}

void Renderer::FillGradient(const POINT& start, const POINT& end, const RGBColor& color1, const RGBColor& color2)  
{
   BitmapBuffer& buffer = getInstance().buffer;

   int minX = start.x;
   int minY = start.y;
   int maxX = buffer.width;
   int maxY = buffer.height;

   /*
   int maxX = end.x;
   int maxY = end.y;
   */

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
		   float imageX = (float)x / (float)buffer.width;
		   float imageY = (float)y / (float)buffer.width;

		   
		   std::cout << "Start: (" << start.x << ", " << start.y << ")  End: (" << end.x << ", " << end.y << ")" << std::endl;
		   std::cout << start.x << "/" << (float)buffer.width << " = " << start.x / (float)buffer.width << ", " << start.y << "/" << (float)buffer.height << " = " << start.y / (float)buffer.height << std::endl;
		   std::cout << end.x << "/" << (float)buffer.width << " = " << end.x / (float)buffer.width  << ", " << end.y << "/" << (float)buffer.height << " = " << end.y / (float)buffer.height << std::endl;
		   
		   float startX = start.x;
		   float startY = start.y;
		   float endX = end.x;
		   float endY = end.y;

		   POINT normalizedStart = { startX / (float)buffer.width, startY / (float)buffer.height}; // normalize the x and y value for start coords
		   POINT normalizedEnd = { endX / (float)buffer.width, endY / (float)buffer.height }; // normalize the x and y value for the end coords


		   std::cout << "normalizedStart.x: " << normalizedStart.x << ", normalizedStart.y: " << normalizedStart.y << std::endl; 
		   std::cout << "normalizedEnd.x: " << normalizedEnd.x << ", normalizedEnd.y: " << normalizedEnd.y << std::endl;


		   POINT direction = { normalizedEnd.x - normalizedStart.x, normalizedEnd.y - normalizedStart.y }; // combine points to get a normalized point

		   float mod = 1.0f / (direction.x * direction.x + direction.y * direction.y); // get normalized float value of direction

		   float gradientPos = ((imageX - normalizedStart.x) * direction.x + (imageY - normalizedStart.y) * direction.y) * mod;

		   float magnitude = std::clamp(gradientPos, 0.0f, 1.0f);

		   /*
		   std::cout << "direction.x: " << direction.x << ", direction.y: " << direction.y << std::endl;
		   std::cout << "normalizedStart.x: " << normalizedStart.x << ", normalizedStart.y: " << normalizedStart.y << std::endl;
		   std::cout << "normalizedEnd.x: " << normalizedEnd.x << ", normalizedEnd.y: " << normalizedEnd.y << std::endl;
		   std::cout << "imageX: " << imageX << ", imageY: " << imageY << std::endl;
		   std::cout << "grandPos: " << gradientPos << ", magnitude: " << magnitude << std::endl;
		   */

		   int red = color1.red + (color2.red - color1.red) * magnitude;
		   int green = color1.green + (color2.green - color1.green) * magnitude;
		   int blue = color1.blue + (color2.blue - color1.blue) * magnitude;

		   uint32_t color = (red << 16) | (green << 8) | (blue << 0);

		   *pixel++ = color;
	   }
	   row += buffer.pitch;
   }
}

void Renderer::FillRect(const Rect& rect, const RGBColor& color)
{
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

void Renderer::FillRoundedRect(const RoundedRect& roundedRect, const RGBColor& color)
{
	BitmapBuffer& buffer = getInstance().buffer;
}

void Renderer::getWindowDimensions(int* width, int* height)
{
	RECT clientRect;
	GetClientRect(getInstance().hWnd, &clientRect);

	*width = clientRect.right - clientRect.left;
	*height = clientRect.bottom - clientRect.top;
}

void Renderer::resizeFrameBuffer(int width, int height)
{
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
	BitmapBuffer& buffer = getInstance().buffer;

	FillRect({ 0, 0, buffer.width, buffer.height }, getInstance().clearColor);
}