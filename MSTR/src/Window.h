#pragma once

#define SDL_MAIN_HANDLED
#include <SDL.h>

class Window
{
public:
	Window(int width = 640, int height = 480);

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	Window(Window&& other) noexcept;
	Window& operator=(Window&& other) noexcept;
	~Window();

	bool MakeContextCurrent() const;

	unsigned GetID() const { return m_ID; }
	void RunLoop();

private:
	SDL_Window* m_Window = nullptr;
	SDL_GLContext m_Dc{};
	unsigned m_ID;

	int m_Width, m_Height;

	// TEMP MAYBE

	unsigned vao;
	unsigned vb;
	unsigned ib;
};