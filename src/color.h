#pragma once

#include <stdint.h>

typedef struct
{
	float r;
	float g;
	float b;
	float a;
} Color;

// clang-format off
#define COLOR_WHITE_INIT {1.0f, 1.0f, 1.0f, 1.0f}
static const Color COLOR_WHITE = COLOR_WHITE_INIT;

#define COLOR_BLACK_INIT {0.0f, 0.0f, 0.0f, 1.0f}
static const Color COLOR_BLACK = COLOR_BLACK_INIT;
// clang-format on

// todo: add helper for hex/web colours?
static inline Color color_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	return (Color){r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f};
}

static inline Color color_linear(float r, float g, float b, float a)
{
	return (Color){r, g, b, a};
}
