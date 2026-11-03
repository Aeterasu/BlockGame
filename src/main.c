#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <glad/gles2.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

SDL_Window* window = NULL;
SDL_GLContext gl_context = NULL;
bool is_running = true;

static void tick(void)
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SDL_GL_SwapWindow(window);

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_QUIT)
		{
			is_running = false;
#ifdef __EMSCRIPTEN__
			emscripten_cancel_main_loop();
#endif
		}
	}
}

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_PropertiesID window_properties = SDL_CreateProperties();

	SDL_SetStringProperty(window_properties, SDL_PROP_WINDOW_CREATE_TITLE_STRING, "Game");
	SDL_SetBooleanProperty(window_properties, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, true);
	SDL_SetNumberProperty(window_properties, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, 270);
	SDL_SetNumberProperty(window_properties, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, 270);
	SDL_SetNumberProperty(window_properties, SDL_PROP_WINDOW_CREATE_X_NUMBER, SDL_WINDOWPOS_CENTERED);
	SDL_SetNumberProperty(window_properties, SDL_PROP_WINDOW_CREATE_Y_NUMBER, SDL_WINDOWPOS_CENTERED);

	SDL_SetBooleanProperty(window_properties, SDL_PROP_WINDOW_CREATE_OPENGL_BOOLEAN, true);

	window = SDL_CreateWindowWithProperties(window_properties);

	SDL_DestroyProperties(window_properties);

	gl_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, gl_context);
	SDL_GL_SetSwapInterval(1);

	gladLoadGLES2((GLADloadfunc)SDL_GL_GetProcAddress);

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(tick, 0, 1);
#else
	while (is_running)
	{
		tick();
	}

	SDL_GL_DestroyContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();
#endif

	return 0;
}
