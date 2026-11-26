#include "game.h"
#include "color.h"
#include "log.h"
#include "rng.h"

#include <stdio.h>

#define TILE_SIZE 10.0f

static const PicoColor tile_color_variants[] = {PICO_RED, PICO_BLUE, PICO_GREEN};
static const uint8_t tile_color_variants_amount = 3u;

void game_init(GameState* const game_state)
{
	GameGraphics* graphics = &game_state->graphics;

	float board_offset_x = (DEFAULT_WINDOW_WIDTH - (BOARD_SIZE_X * TILE_SIZE)) * 0.5f;
	float board_offset_y = (DEFAULT_WINDOW_HEIGHT - (BOARD_SIZE_Y * TILE_SIZE)) * 0.5f;

	for (uint32_t x = 0; x < BOARD_SIZE_X; x++)
	{
		for (uint32_t y = 0; y < BOARD_SIZE_Y; y++)
		{
			DrawableItem2D* item = &graphics->tiles[x][y];
			item->transform = (Transform2D){
				.position = (Vec2f){board_offset_x + x * TILE_SIZE, board_offset_y + y * TILE_SIZE},
				.scale = (Vec2f){TILE_SIZE, TILE_SIZE},
				.rotation = 0.0f,
			};

			item->texture_atlas_rect = (Rect2f){(Vec2f){18.0f, 18.0f}, (Vec2f){0.0f, 0.0f}};

			int r = gameplay_random_uint8() * tile_color_variants_amount / 256;
			item->tint = pico_color(tile_color_variants[r]);
		}
	}
}

void game_fixed_tick(GameState* const game_state, const double delta)
{
}

void game_visual_tick(GameState* const game_state, const double delta)
{
	for (uint32_t x = 0; x < BOARD_SIZE_X; x++)
	{
		for (uint32_t y = 0; y < BOARD_SIZE_Y; y++)
		{
			video_draw_item(game_state->graphics.tiles[x][y]);
		}
	}
}
