#include <stdbool.h>
#include <stdint.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <SDL3/SDL_main.h>

#include "log.h"
#include "video.h"

#ifndef GAME_NAME
#define GAME_NAME "Game"
#endif

#define GAMEPLAY_TARGET_FPS 60.0
#define GAMEPLAY_DELTA_TIME (1.0 / GAMEPLAY_TARGET_FPS)

#define VISUAL_DT_DISCARD_THRESHOLD 0.25

static bool is_running = true;

static double accumulator = 0.0;
static double previous_time = 0.0;

double get_engine_time_seconds(void)
{
	return (double)SDL_GetTicksNS() / 1e9;
}

static void teardown(void)
{
	log_info("ENGINE TEARDOWN...");
	video_deinit();
}

static void gameplay_tick(double delta)
{
}

static void visual_tick(double delta)
{
	video_clear_frame();
	video_render_frame();
}

static void main_iterate(void)
{
	double current_time = get_engine_time_seconds();
	double frame_time = current_time - previous_time;

	if (frame_time < 0.0)
	{
		frame_time = 0.0;
	}

	previous_time = current_time;

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_EVENT_QUIT:
				is_running = false;
#ifdef __EMSCRIPTEN__
				emscripten_cancel_main_loop();
#endif
				break;
			case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
				video_resize_viewport();
				break;
		}
	}

	if (frame_time > VISUAL_DT_DISCARD_THRESHOLD)
	{
		frame_time = VISUAL_DT_DISCARD_THRESHOLD;
	}

	accumulator += frame_time;

	if (accumulator >= GAMEPLAY_DELTA_TIME)
	{
		gameplay_tick(GAMEPLAY_DELTA_TIME);
		// todo: check this in practice, should we not actually discard the accumulator?
		accumulator = 0.0;
	}

	visual_tick(frame_time);
}

int main(int argc, char* argv[])
{
	log_info("LAUNCHING %s...", GAME_NAME);

	video_init();
	video_upload_texture_atlas("asset/texture/texture_1.png");

	previous_time = get_engine_time_seconds();

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(main_iterate, 0, 1);
#else
	while (is_running)
	{
		main_iterate();
	}

#endif
	teardown();

	return 0;
}
