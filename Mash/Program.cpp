#include <iostream>
#include "Window.h"
#include "Renderer.h"



int main()
{
	Window* pWindow = new Window();


	/*     window heartbeat     */

	bool running = true;

	while (running)
	{
		if (!pWindow->ProcessMessages())
		{
			running = false;
		}
		
		//Renderer::SetClearColor({ 255, 0, 0 });

		Renderer::clearBuffer();

		int winWidth, winHeight;
		Renderer::getWindowDimensions(&winWidth, &winHeight);
		Renderer::FillGradient({ 0, 0 }, { winWidth, winHeight }, { 0, 0, 255 }, { 0, 255, 0 });


		pWindow->Render();

		Sleep(10);
	}

	delete pWindow;
	return 0;
}