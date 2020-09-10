#include "MSTR.h"

#include <glad\glad.h>
#include "Core.h"

MSTR MSTR::s_Instance;

bool MSTR::Init()
{
	if (s_Instance.m_Running)
		return false;

	std::thread t{ [&]() {

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
	}

	s_Instance.m_Running = true;
	auto& windows = s_Instance.m_Windows;
	SDL_Event event;

	while (s_Instance.m_Running)
	{
		if (s_Instance.m_CreatingWindow)
		{
			windows.emplace_back();
			s_Instance.m_CreatingWindow = false;
		}

		while (SDL_PollEvent(&event))
		{
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_CLOSE:
				windows.erase(std::remove_if(windows.begin(), windows.end(), [&](const Window& w) {return event.window.windowID == w.GetID(); }));
				break;
			}

			switch (event.type)
			{
			case SDL_QUIT:
				s_Instance.m_Running = false;
			}
		}
	
		for (auto& window : windows)
			window.RunLoop();
	}

	s_Instance.m_Windows.clear();
	SDL_Quit();

	} };

	t.detach();
	return s_Instance.m_Running;
}

void MSTR::ShutDown()
{
	s_Instance.m_Running = false;
}
