#include "pch.h"

#include "vld.h"
#include "Renderer.h"

#include <string>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* args[])
{
	SDL_Init(SDL_INIT_VIDEO);

	const std::string windowTitle{ "Rasterizer - Fratczak Jakub (2DAE10)" };

	SDL_Window* pWindow = SDL_CreateWindow(
		windowTitle.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT, NULL);

	if (!pWindow)
		return 1;

	SDL_SetRelativeMouseMode(SDL_bool(true));

	Renderer renderer{ pWindow };

	std::cout << CONTROLS;

	Timer timer{};
	timer.Start();

	bool isLooping{ true };
	float printTimer{};
	while (isLooping)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				isLooping = false;
				break;

			case SDL_KEYUP:
				switch (event.key.keysym.scancode)
				{
				}
				break;

			case SDL_MOUSEWHEEL:

				break;
			}
		}

		renderer.Update(timer);
		renderer.Render();

		timer.Update();
		printTimer += timer.GetElapsed();
		if (printTimer >= 1.0f)
		{
			printTimer = 0.0f;
			SDL_SetWindowTitle(pWindow, (windowTitle + " - dFPS: " + std::to_string(timer.GetdFPS())).c_str());
		}
	}
	timer.Stop();

	SDL_DestroyWindow(pWindow);
	SDL_Quit();
	return 0;
}