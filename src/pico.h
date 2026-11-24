#pragma once

#include "color.h"

typedef enum
{
	PICO_BLACK,
	PICO_DARK_BLUE,
	PICO_DARK_PURPLE,
	PICO_DARK_GREEN,
	PICO_BROWN,
	PICO_DARK_GREY,
	PICO_LIGHT_GREY,
	PICO_WHITE,
	PICO_RED,
	PICO_ORANGE,
	PICO_YELLOW,
	PICO_GREEN,
	PICO_BLUE,
	PICO_LAVENDER,
	PICO_PINK,
	PICO_LIGHT_PEACH,
	PICO_COLOR_COUNT
} PicoColor;

static inline Color get_pico_black(void)
{
	return color_linear(0.0f, 0.0f, 0.0f, 1.0f);
}

static inline Color get_pico_dark_blue(void)
{
	return color_linear(0.113725f, 0.168627f, 0.325490f, 1.0f);
}

static inline Color get_pico_dark_purple(void)
{
	return color_linear(0.494118f, 0.145098f, 0.325490f, 1.0f);
}

static inline Color get_pico_dark_green(void)
{
	return color_linear(0.0f, 0.529412f, 0.317647f, 1.0f);
}

static inline Color get_pico_brown(void)
{
	return color_linear(0.670588f, 0.321569f, 0.211765f, 1.0f);
}

static inline Color get_pico_dark_grey(void)
{
	return color_linear(0.372549f, 0.341176f, 0.309804f, 1.0f);
}

static inline Color get_pico_light_grey(void)
{
	return color_linear(0.760784f, 0.764706f, 0.780392f, 1.0f);
}

static inline Color get_pico_white(void)
{
	return color_linear(1.0f, 0.945098f, 0.909804f, 1.0f);
}

static inline Color get_pico_red(void)
{
	return color_linear(1.0f, 0.0f, 0.301961f, 1.0f);
}

static inline Color get_pico_orange(void)
{
	return color_linear(1.0f, 0.639216f, 0.0f, 1.0f);
}

static inline Color get_pico_yellow(void)
{
	return color_linear(1.0f, 0.925490f, 0.152941f, 1.0f);
}

static inline Color get_pico_green(void)
{
	return color_linear(0.0f, 0.894118f, 0.211765f, 1.0f);
}

static inline Color get_pico_blue(void)
{
	return color_linear(0.160784f, 0.678431f, 1.0f, 1.0f);
}

static inline Color get_pico_lavender(void)
{
	return color_linear(0.513725f, 0.462745f, 0.611765f, 1.0f);
}

static inline Color get_pico_pink(void)
{
	return color_linear(1.0f, 0.466667f, 0.658824f, 1.0f);
}

static inline Color get_pico_light_peach(void)
{
	return color_linear(1.0f, 0.8f, 0.666667f, 1.0f);
}

static inline Color pico_color(PicoColor pico)
{
	switch (pico)
	{
		case PICO_BLACK:
			return get_pico_black();
		case PICO_DARK_BLUE:
			return get_pico_dark_blue();
		case PICO_DARK_PURPLE:
			return get_pico_dark_purple();
		case PICO_DARK_GREEN:
			return get_pico_dark_green();
		case PICO_BROWN:
			return get_pico_brown();
		case PICO_DARK_GREY:
			return get_pico_dark_grey();
		case PICO_LIGHT_GREY:
			return get_pico_light_grey();
		case PICO_WHITE:
			return get_pico_white();
		case PICO_RED:
			return get_pico_red();
		case PICO_ORANGE:
			return get_pico_orange();
		case PICO_YELLOW:
			return get_pico_yellow();
		case PICO_GREEN:
			return get_pico_green();
		case PICO_BLUE:
			return get_pico_blue();
		case PICO_LAVENDER:
			return get_pico_lavender();
		case PICO_PINK:
			return get_pico_pink();
		case PICO_LIGHT_PEACH:
			return get_pico_light_peach();
		default:
			return get_pico_black();
	}
}
