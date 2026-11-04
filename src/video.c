#include "video.h"

#include "file.h"

SDL_Window* window = NULL;
SDL_GLContext gl_context = NULL;

void video_init(void)
{
	SDL_InitSubSystem(SDL_INIT_VIDEO);

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

	glViewport(0, 0, 270, 270);

	// test quad

	GLuint shader_program = video_load_shader_program("asset/shader/2d.vert", "asset/shader/default.frag");
	glUseProgram(shader_program);

	GLfloat quad_vertices[] = {
		-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f,
	};

	GLuint quad_indices[] = {0, 1, 2, 2, 3, 0};
	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_indices), quad_indices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (const void*)0);
	glEnableVertexAttribArray(0);
}

void video_deinit(void)
{
	SDL_GL_DestroyContext(gl_context);
	SDL_DestroyWindow(window);

	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void video_clear_frame(void)
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void video_render_frame(void)
{
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

	SDL_GL_SwapWindow(window);
}

GLuint video_compile_shader(const char* path, const GLenum type)
{
	char* source = read_file(path);

	if (!source)
	{
		return 0;
	}

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar**)&source, NULL);
	glCompileShader(shader);

	free(source);

	GLint success = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		GLint log_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

		char* log = malloc((size_t)log_length + 1);
		if (log)
		{
			glGetShaderInfoLog(shader, log_length, NULL, log);
			log[log_length] = '\0';
			fprintf(stderr, "Shader compilation failed (%s):\n%s\n", path, log);
			free(log);
		}

		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

GLuint video_load_shader_program(const char* vertex_path, const char* fragment_path)
{
	GLuint vertex = video_compile_shader(vertex_path, GL_VERTEX_SHADER);

	if (!vertex)
	{
		return 0;
	}

	GLuint fragment = video_compile_shader(fragment_path, GL_FRAGMENT_SHADER);

	if (!fragment)
	{
		glDeleteShader(vertex);
		return 0;
	}

	GLuint program = glCreateProgram();

	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	GLint success = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		GLint log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

		char* log = malloc((size_t)log_length + 1);

		if (log)
		{
			glGetProgramInfoLog(program, log_length, NULL, log);
			log[log_length] = '\0';
			fprintf(stderr, "Shader linking failed:\n%s\n", log);
			free(log);
		}

		glDeleteProgram(program);
		return 0;
	}

	return program;
}
