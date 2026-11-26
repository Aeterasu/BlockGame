#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <glad/gles2.h>

#include "color.h"
#include "vector.h"

#define MAX_DRAWABLE_ITEM_COUNT 4096u

#define DEFAULT_WINDOW_WIDTH 240u
#define DEFAULT_WINDOW_HEIGHT 320u

typedef struct
{
	Transform2D transform;
	Rect2f texture_atlas_rect;
	Color tint;
} DrawableItem2D;

void video_init(void);
void video_deinit(void);

void video_resize_viewport(void);

void video_clear_frame(void);
void video_render_frame(void);

GLuint video_compile_shader(const char* path, const GLenum type);
GLuint video_load_shader_program(const char* vertex_path, const char* fragment_path);

void video_upload_texture_atlas(const char* path);

void video_draw_item(DrawableItem2D item);

void video_set_clear_color(Color color);
