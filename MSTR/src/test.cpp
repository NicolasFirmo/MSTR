#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <glad\glad.h>

#define CONCAT(a,b) THIS_IS_STUPID(a,b)
#define THIS_IS_STUPID(a,b) a ## b
#define APPLINE(x) CONCAT(x,__LINE__)

#define PRAGMA(x) __pragma(x)

#ifndef NDEBUG
#define	DEBUG_BREAK() __debugbreak()
#define DebugLog(x) std::cerr << x;
#else
#define DebugLog(x)
#endif

#ifndef NDEBUG
#define NIC_ASSERT(x,s) if(!(x)){ DebugLog("ASSERTION FALIED: " << s << "\nLINE: " << __LINE__ << "\nFILE: " << __FILE__ << '\n'); DEBUG_BREAK();}
#else
#define NIC_ASSERT(x,s) PRAGMA(warning(disable: 552 553 834)) x; PRAGMA(warning(default: 552 553 834))
#endif

#ifndef NDEBUG
#define GLCall(x) while(glGetError()); x; while(auto error = glGetError())NIC_ASSERT(false, std::hex << "[OpenGL error] (0x0" << error << std::dec << "): " << #x)
#else
#define GLCall(x) x
#endif

int main()
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("Borabora", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetSwapInterval(1);

	SDL_GLContext dc = SDL_GL_CreateContext(window);

	gladLoadGLLoader(SDL_GL_GetProcAddress);

	SDL_Event event;

	constexpr float triangulo[] = {
		 0.0f,  0.5f,
		-0.5f, -0.5f,
		 0.5f, -0.5f
	};

	constexpr unsigned indices[] = {
		0,1,2
	};

	GLuint vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	GLuint vb;
	GLCall(glGenBuffers(1, &vb));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vb));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(triangulo), triangulo, GL_STATIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(0)));

	GLuint ib;
	GLCall(glGenBuffers(1, &vb));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vb));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	bool running = true;
	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
			}
		}
#ifndef NDEBUG
		GLCall(glClearColor(0.0f, 1.0f, 0.5f, 1.0f));
#else
		GLCall(glClearColor(1.0f, 0.0f, 0.5f, 1.0f));
#endif
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		GLCall(glDrawElements(GL_TRIANGLES, std::size(indices), GL_UNSIGNED_INT, static_cast<void*>(0)));

		SDL_GL_SwapWindow(window);
	}

	SDL_DestroyWindow(window);

	SDL_Quit();
}