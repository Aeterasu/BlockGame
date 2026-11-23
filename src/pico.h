#pragma once

#include "color.h"

static inline Color pico_black(void)
{
	return color_linear(0.0f, 0.0f, 0.0f, 1.0f);
}

static inline Color pico_dark_blue(void)
{
	return color_linear(0.113725f, 0.168627f, 0.325490f, 1.0f);
}

static inline Color pico_dark_purple(void)
{
	return color_linear(0.494118f, 0.145098f, 0.325490f, 1.0f);
}

static inline Color pico_dark_green(void)
{
	return color_linear(0.0f, 0.529412f, 0.317647f, 1.0f);
}

static inline Color pico_brown(void)
{
	return color_linear(0.670588f, 0.321569f, 0.211765f, 1.0f);
}

static inline Color pico_dark_grey(void)
{
	return color_linear(0.372549f, 0.341176f, 0.309804f, 1.0f);
}

static inline Color pico_light_grey(void)
{
	return color_linear(0.760784f, 0.764706f, 0.780392f, 1.0f);
}

static inline Color pico_white(void)
{
	return color_linear(1.0f, 0.945098f, 0.909804f, 1.0f);
}

static inline Color pico_red(void)
{
	return color_linear(1.0f, 0.0f, 0.301961f, 1.0f);
}

static inline Color pico_orange(void)
{
	return color_linear(1.0f, 0.639216f, 0.0f, 1.0f);
}

static inline Color pico_yellow(void)
{
	return color_linear(1.0f, 0.925490f, 0.152941f, 1.0f);
}

static inline Color pico_green(void)
{
	return color_linear(0.0f, 0.894118f, 0.211765f, 1.0f);
}

static inline Color pico_blue(void)
{
	return color_linear(0.160784f, 0.678431f, 1.0f, 1.0f);
}

static inline Color pico_lavender(void)
{
	return color_linear(0.513725f, 0.462745f, 0.611765f, 1.0f);
}

static inline Color pico_pink(void)
{
	return color_linear(1.0f, 0.466667f, 0.658824f, 1.0f);
}

static inline Color pico_light_peach(void)
{
	return color_linear(1.0f, 0.8f, 0.666667f, 1.0f);
}
