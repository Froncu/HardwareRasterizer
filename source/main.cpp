#include "pch.h"

#include "vld.h"
#include "Renderer.h"
#include "Scene.h"
#include "Timer.h"

#include <string>

#pragma region EntryPoint
int main([[maybe_unused]] int argc, [[maybe_unused]] char* args[])
{
	SDL_Init(SDL_INIT_VIDEO);

	const std::string windowTitle{ "Rasterizer - Fratczak Jakub (2DAE10)" };
	SDL_Window* const pWindow
	{
		SDL_CreateWindow(
		windowTitle.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT, NULL)
	};

	SDL_SetRelativeMouseMode(SDL_bool(true));

	std::cout << CONTROLS;

	Renderer renderer{ pWindow };
	Scene scene{ renderer.GetDevice() };

	Timer timer{};
	timer.Start();
	while (true)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				return 0;

			case SDL_KEYUP:
				switch (event.key.keysym.scancode)
				{
				case SDL_SCANCODE_F2:
					scene.ToggleFilteringType();
					break;
				}
				break;

			case SDL_MOUSEWHEEL:
				scene.GetCamera().IncrementFieldOfViewAngle(-event.wheel.preciseY / 20.0f);
				break;
			}
		}

		timer.Update();
		scene.Update(timer);
		renderer.Render(scene);

		static float printTimer{};
		printTimer += timer.GetElapsed();
		if (printTimer >= 1.0f)
		{
			printTimer = 0.0f;
			SDL_SetWindowTitle(pWindow, (windowTitle + " - dFPS: " + std::to_string(timer.GetdFPS())).c_str());
		}
	}

	SDL_DestroyWindow(pWindow);
	SDL_Quit();

	return 0;
}
#pragma endregion EntryPoint