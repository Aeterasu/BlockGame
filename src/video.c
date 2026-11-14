#include "video.h"
#include "file.h"
#include "log.h"
#include "vector.h"

static SDL_Window* window = NULL;
static SDL_GLContext gl_context = NULL;

// clang-format off
static GLfloat quad_vertices[] = 
{
	// position, uv data
	// x, y, z, u, v
	-0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
};

static GLuint quad_indices[] =
{
	0, 1, 2,
	2, 3, 0
};

static const GLuint PER_VERTEX_DATA_COUNT = 5;
// clang-format on

static GLuint vao = 0;
static GLuint vbo = 0;
static GLuint ebo = 0;

static GLuint texture_atlas_sampler_id = 0;

static GLuint shader_program = 0;
static GLint texture_atlas_uniform = 0;

static GLint projection_uniform = -1;
static GLint model_uniform = -1;

static GLint atlas_size_uniform = -1;
static GLint texture_offset_uniform = -1;
static GLint sprite_size_uniform = -1;

static DrawableItem2D drawables[MAX_DRAWABLE_ITEM_COUNT];
static uint16_t active_drawables = 0;
static int drawable_limit_warned = 0;

void video_init(void)
{
	if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
	{
		log_error("Failed to initialize SDL video: %s", SDL_GetError());
	}

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

	if (!window)
	{
		log_error("Failed to create window: %s", SDL_GetError());
	}

	gl_context = SDL_GL_CreateContext(window);

	if (!gl_context)
	{
		log_error("Failed to create GL context: %s", SDL_GetError());
	}

	SDL_GL_MakeCurrent(window, gl_context);
	SDL_GL_SetSwapInterval(1);

	if (!gladLoadGLES2((GLADloadfunc)SDL_GL_GetProcAddress))
	{
		log_error("Failed to load GLES2 functions");
	}

	glViewport(0, 0, 270, 270);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// test quad

	shader_program = video_load_shader_program("asset/shader/2d.vert", "asset/shader/default.frag");
	glUseProgram(shader_program);

	texture_atlas_uniform = glGetUniformLocation(shader_program, "u_texture");

	projection_uniform = glGetUniformLocation(shader_program, "u_projection");
	model_uniform = glGetUniformLocation(shader_program, "u_model");

	atlas_size_uniform = glGetUniformLocation(shader_program, "u_atlas_size");
	texture_offset_uniform = glGetUniformLocation(shader_program, "u_texture_offset");
	sprite_size_uniform = glGetUniformLocation(shader_program, "u_sprite_size");

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_indices), quad_indices, GL_DYNAMIC_DRAW);

	GLsizei stride = PER_VERTEX_DATA_COUNT * sizeof(GLfloat);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (const void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (const void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	const char* vendor = (const char*)glGetString(GL_VENDOR);
	const char* renderer = (const char*)glGetString(GL_RENDERER);
	const char* version = (const char*)glGetString(GL_VERSION);

	log_info("VIDEO INITIALIZED: vendor=%s renderer=%s version=%s", vendor ? vendor : "unknown",
			 renderer ? renderer : "unknown", version ? version : "unknown");
}

void video_upload_texture_atlas(const char* path)
{
	SDL_Surface* loaded_surface = IMG_Load(path);

	if (!loaded_surface)
	{
		log_error("Failed to load texture atlas: %s (%s)", path, SDL_GetError());
		return;
	}

	SDL_Surface* texture_atlas_surface = SDL_ConvertSurface(loaded_surface, SDL_PIXELFORMAT_RGBA32);

	if (!texture_atlas_surface)
	{
		log_error("Failed to convert texture atlas: %s", SDL_GetError());
		SDL_DestroySurface(loaded_surface);
		return;
	}

	SDL_DestroySurface(loaded_surface);

	glGenTextures(1, &texture_atlas_sampler_id);
	glBindTexture(GL_TEXTURE_2D, texture_atlas_sampler_id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_atlas_surface->w, texture_atlas_surface->h, 0, GL_RGBA,
				 GL_UNSIGNED_BYTE, texture_atlas_surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, texture_atlas_sampler_id);

	SDL_DestroySurface(texture_atlas_surface);
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

	active_drawables = 0;
}

void video_render_frame(void)
{
	Mat3f projection = mat3f_orthographic(0.0, 270.0f, 270.0f, 0.0f);

	Transform2D t1 = {.position = {40.0f, 150.0f}, .scale = {26.0f, 26.0f}, .rotation = 0.5345f};
	Transform2D t2 = {.position = {200.0f, 90.0f}, .scale = {65.0f, 65.0f}, .rotation = 4.345345f};
	Transform2D t3 = {.position = {150.0f, 190.0f}, .scale = {16.0f, 16.0f}, .rotation = 0.145345f};

	Rect2f r1 = {.size = {18.0f, 18.0f}, .position = {0.0f, 0.0f}};
	Rect2f r2 = {.size = {18.0f, 18.0f}, .position = {18.0f, 18.0f}};
	Rect2f r3 = {.size = {18.0f, 18.0f}, .position = {18.0f, 0.0f}};

	video_draw_item((DrawableItem2D){t1, r1});
	video_draw_item((DrawableItem2D){t2, r2});
	video_draw_item((DrawableItem2D){t3, r3});

	glUseProgram(shader_program);
	glBindVertexArray(vao);

	glActiveTexture(GL_TEXTURE0);
	// TODO: dont rebind shit every frame
	glBindTexture(GL_TEXTURE_2D, texture_atlas_sampler_id);
	glUniform1i(texture_atlas_uniform, 0);
	glUniformMatrix3fv(projection_uniform, 1, GL_FALSE, projection.m);

	for (uint16_t i = 0; i < active_drawables; i++)
	{
		glUniformMatrix3fv(model_uniform, 1, GL_FALSE, mat3f_get_from_transform_2d(drawables[i].transform).m);
		glUniform2f(atlas_size_uniform, 36.0f, 36.0f);

		Vec2f offset = drawables[i].texture_atlas_rect.position;
		Vec2f size = drawables[i].texture_atlas_rect.size;

		glUniform2f(texture_offset_uniform, offset.x, offset.y);
		glUniform2f(sprite_size_uniform, size.x, size.y);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
	}

	SDL_GL_SwapWindow(window);
}

GLuint video_compile_shader(const char* path, const GLenum type)
{
	char* source = read_file(path);

	if (!source)
	{
		log_error("Failed to read shader: %s", path);
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
			log_error("Shader compilation failed (%s): %s", path, log);
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
			log_error("Shader linking failed: %s", log);
			free(log);
		}

		glDeleteProgram(program);
		return 0;
	}

	return program;
}

void video_draw_item(const DrawableItem2D item)
{
	if (active_drawables >= MAX_DRAWABLE_ITEM_COUNT)
	{
		if (!drawable_limit_warned)
		{
			log_warning("Drawable limit exceeded (%u)", MAX_DRAWABLE_ITEM_COUNT);
			drawable_limit_warned = 1;
		}

		return;
	}

	drawables[active_drawables] = item;

	active_drawables++;
}
