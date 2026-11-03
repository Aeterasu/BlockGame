#pragma once

#include <SDL3/SDL.h>

#include <glad/gles2.h>

void video_init(void);
void video_deinit(void);
void video_clear_frame(void);
void video_render_frame(void);
