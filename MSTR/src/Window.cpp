#include "Window.h"

#include <glad/glad.h>
#include "Core.h"

constexpr float triangulo[] = {
	 0.0f,  0.5f,
	-0.5f, -0.5f,
	 0.5f, -0.5f
};

constexpr unsigned indices[] = {
	0,1,2
};

Window::Window(int width, int height) :
	m_Width(width), m_Height(height)
{
	if (m_Window = SDL_CreateWindow("Borabora", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		m_Width, m_Height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE))
	{
		m_Dc = SDL_GL_CreateContext(m_Window);
		m_ID = SDL_GetWindowID(m_Window);
	}
	else
	{
		SDL_Log("Unable to create window: %s", SDL_GetError());
	}

	if (
		SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_CONTEXT_MAJOR_VERSION, 3) ||
		SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_CONTEXT_MINOR_VERSION, 3) ||
		SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) ||
		SDL_GL_SetSwapInterval(1)
		)
	{
		SDL_Log("Unable to set OpenGL attributes: %s", SDL_GetError());
	}

	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		SDL_Log("Unable to load window procedure to glad: %s", SDL_GetError());
	}

	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	GLCall(glGenBuffers(1, &vb));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vb));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(triangulo), triangulo, GL_STATIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(0)));

	GLCall(glGenBuffers(1, &ib));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
}

Window::Window(Window&& other) noexcept :
	m_Window(other.m_Window), m_Dc(other.m_Dc), m_ID(other.m_ID),
	m_Width(other.m_Width), m_Height(other.m_Height),
	vao(other.vao), vb(other.vb), ib(other.ib)
{
	other.m_Window = nullptr;
}

Window& Window::operator=(Window&& other) noexcept
{
	if (this != &other)
	{
		if (m_Window)
		{
			MakeContextCurrent();

			GLCall(glDeleteVertexArrays(1, &vao));
			GLCall(glDeleteBuffers(1, &vb));
			GLCall(glDeleteBuffers(1, &ib));

			SDL_GL_DeleteContext(m_Dc);
			SDL_DestroyWindow(m_Window);
		}

		m_Window = other.m_Window;
		m_Dc = other.m_Dc;
		m_ID = other.m_ID;

		m_Width = other.m_Width;
		m_Height = other.m_Height;

		vao = other.vao;
		vb = other.vb;
		ib = other.ib;

		other.m_Window = nullptr;
	}

	return *this;
}

Window::~Window()
{
	if (m_Window)
	{
		MakeContextCurrent();

		GLCall(glDeleteVertexArrays(1, &vao));
		GLCall(glDeleteBuffers(1, &vb));
		GLCall(glDeleteBuffers(1, &ib));

		SDL_GL_DeleteContext(m_Dc);
		SDL_DestroyWindow(m_Window);
	}
}

bool Window::MakeContextCurrent() const
{
	if (SDL_GL_MakeCurrent(m_Window, m_Dc) != 0)
	{
		SDL_Log("Unable to make context current for this window: %s", SDL_GetError());
		return false;
	}

	return true;
}

void Window::RunLoop()
{
	MakeContextCurrent();

#ifndef NDEBUG
	GLCall(glClearColor(0.0f, 1.0f, 0.5f, 1.0f));
#else
	GLCall(glClearColor(1.0f, 0.0f, 0.5f, 1.0f));
#endif
	GLCall(glClear(GL_COLOR_BUFFER_BIT));

	GLCall(glBindVertexArray(vao));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vb));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib));

	GLCall(glDrawElements(GL_TRIANGLES, std::size(indices), GL_UNSIGNED_INT, static_cast<void*>(0)));

	SDL_GL_SwapWindow(m_Window);
}