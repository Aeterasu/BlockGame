#pragma once

#include <stdint.h>

#include "pico.h"
#include "video.h"

#define BOARD_SIZE_X 10u
#define BOARD_SIZE_Y 20u

typedef enum
{
	TILE_EMPTY,
	TILE_COLOR,
	TILE_DESTROY,
	TILE_BUCKET,
	TILE_RAINBOW,
	TILE_NOISE,
} TileVariant;

typedef struct
{
	TileVariant variant;
	PicoColor color;
} Tile;

typedef struct
{
	uint64_t score;
} Scoring;

typedef struct
{
	DrawableItem2D tiles[BOARD_SIZE_X][BOARD_SIZE_Y];
} GameGraphics;

typedef struct
{
	Tile board[BOARD_SIZE_X][BOARD_SIZE_Y];
	Scoring scoring;

	GameGraphics graphics;
} GameState;

void game_init(GameState* game_state);
void game_fixed_tick(GameState* game_state, double delta);
void game_visual_tick(GameState* game_state, double delta);
