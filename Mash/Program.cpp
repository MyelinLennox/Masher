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

	std::mt19937 generator(std::time(nullptr));

	std::uniform_int_distribution<int> intDistribution1(1, 255);
	std::uniform_int_distribution<int> intDistribution2(1, 255);
	std::uniform_int_distribution<int> intDistribution3(1, 255);

	int randomInt1 = intDistribution1(generator);
	int randomInt2 = intDistribution2(generator);
	int randomInt3 = intDistribution3(generator);

	std::uniform_int_distribution<int> intDistribution4(1, 255);
	std::uniform_int_distribution<int> intDistribution5(1, 255);
	std::uniform_int_distribution<int> intDistribution6(1, 255);

	int randomInt4 = intDistribution4(generator);
	int randomInt5 = intDistribution5(generator);
	int randomInt6 = intDistribution6(generator);

	int tenth = 0;

	while (running)
	{
		randomInt1++;
		randomInt2++;
		randomInt3++;
		randomInt4++;
		randomInt5++;
		randomInt6++;
		
		tenth++;

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

					Renderer::fillGradient({ 450,150,150,150 }, { uint8_t(randomInt1),uint8_t(randomInt2),uint8_t(randomInt3) }, { uint8_t(randomInt4),uint8_t(randomInt5),uint8_t(randomInt6) });
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