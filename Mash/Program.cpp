#include <iostream>
#include <cstdlib>
#include <random>

#include "Window.h"
#include "Renderer.h"
#include "conio.h"


int main()
{
	std::string IoRequest;

	Window& pWindow = Window::getInstance();

	int renderCutoff = -1; // use -1 for false, any greater value will run that number of updates then freezes rendering
	if (renderCutoff > 0)
	{
		std::cout << "(Program.cpp) -- TEST MODE -- Running for: " << renderCutoff << " ticks.." << std::endl;
	}

	bool running = true;
	bool rendering = true;

	LARGE_INTEGER cpuFrequency;
	QueryPerformanceFrequency(&cpuFrequency);

	LARGE_INTEGER lastCounter;
	QueryPerformanceCounter(&lastCounter);

	while (running)
	{

		LARGE_INTEGER currentCounter;
		QueryPerformanceCounter(&currentCounter);

		int64_t counterElapsed = currentCounter.QuadPart - lastCounter.QuadPart;

		float delta = (float)counterElapsed / (float)cpuFrequency.QuadPart;	// in seconds

		lastCounter = currentCounter;

		if (renderCutoff == 0)
		{
			std::cout << "(Program.cpp) -- TEST MODE -- Ticks completed, rendering is frozen" << std::endl;
			rendering = false;
		}

		if (!pWindow.processMessages()) running = false;
		
		if (rendering)
		{

			Window::setGameUpdate([&](float delta)
			{
					std::cout << "(Program.cpp) -- Delta: " << delta << std::endl;

					Renderer::setPixel(50, 150, { 255,255,255 });

					Renderer::fillRect({ 150,150,150,150 }, { 0,255,0 });

					Renderer::fillGradient({ 450,150,150,150 }, { 255,0,0 }, { 0, 0, 255 });
			});

			Renderer::setClearColor({ 0, 0, 0 });

			pWindow.Render();

			Renderer::clearBuffer();

			Window::getInstance().update(delta);
		}
		
		Sleep(10);
		
		if ((renderCutoff) >= 0)
		{
			renderCutoff--;
		}
	}
	return 0;
}