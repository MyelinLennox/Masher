#include <iostream>
#include "Window.h"
#include "Renderer.h"

int main()
{
	Window* pWindow = new Window();

	bool running = true;
	while (running)
	{
		if (!pWindow->ProcessMessages())
		{
			running = false;
		}
		
		Renderer::SetClearColor({ 255, 0, 0 });

		pWindow->Render();

		Sleep(10);
	}

	delete pWindow;
	return 0;

}