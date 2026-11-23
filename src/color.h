#pragma once

#include <stdint.h>

typedef struct
{
	float r;
	float g;
	float b;
	float a;
} Color;

// todo: add helper for hex/web colours?

static inline Color color_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	return (Color){r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f};
}

static inline Color color_linear(float r, float g, float b, float a)
{
	return (Color){r, g, b, a};
}

static inline Color color_white(void)
{
	return (Color){1.0f, 1.0f, 1.0f, 1.0f};
}

static inline Color color_black(void)
{
	return (Color){0.0f, 0.0f, 0.0f, 1.0f};
}
