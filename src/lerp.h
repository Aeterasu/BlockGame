#pragma once
#include <cmath>
#include <cstdint>

inline float lerp(float from, float to, float weight)
{
	return from + (to - from) * weight;
}

inline uint64_t lerpInt64(uint64_t from, uint64_t to, double weight)
{
	if (to >= from)
	{
		return from + static_cast<uint64_t>((to - from) * weight);
	}
	else
	{
		return from - static_cast<uint64_t>((from - to) * weight);
	}
}

inline float lerpQuadIn(float from, float to, float weight)
{
	return lerp(from, to, weight * weight);
}
inline float lerpQuadOut(float from, float to, float weight)
{
	float t = 1.0f - (1.0f - weight) * (1.0f - weight);
	return lerp(from, to, t);
}
inline float lerpQuadInOut(float from, float to, float weight)
{
	float t = weight < 0.5f ? 2.0f * weight * weight : 1.0f - std::pow(-2.0f * weight + 2.0f, 2.0f) / 2.0f;
	return lerp(from, to, t);
}

inline float lerpCubicIn(float from, float to, float weight)
{
	return lerp(from, to, weight * weight * weight);
}
inline float lerpCubicOut(float from, float to, float weight)
{
	float t = 1.0f - std::pow(1.0f - weight, 3.0f);
	return lerp(from, to, t);
}
inline float lerpCubicInOut(float from, float to, float weight)
{
	float t = weight < 0.5f ? 4.0f * weight * weight * weight : 1.0f - std::pow(-2.0f * weight + 2.0f, 3.0f) / 2.0f;
	return lerp(from, to, t);
}

inline float lerpExpoIn(float from, float to, float weight)
{
	float t = weight <= 0.0f ? 0.0f : std::pow(2.0f, 10.0f * weight - 10.0f);
	return lerp(from, to, t);
}
inline float lerpExpoOut(float from, float to, float weight)
{
	float t = weight >= 1.0f ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * weight);
	return lerp(from, to, t);
}
inline float lerpExpoInOut(float from, float to, float weight)
{
	float t = 0.0f;

	if (weight <= 0.0f)
	{
		t = 0.0f;
	}
	else if (weight >= 1.0f)
	{
		t = 1.0f;
	}
	else if (weight < 0.5f)
	{
		t = std::pow(2.0f, 20.0f * weight - 10.0f) / 2.0f;
	}
	else
	{
		t = (2.0f - std::pow(2.0f, -20.0f * weight + 10.0f)) / 2.0f;
	}

	return lerp(from, to, t);
}

inline float lerpBounceOut(float from, float to, float weight)
{
	float t = 0.0f;

	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (weight < 1.0f / d1)
	{
		t = n1 * weight * weight;
	}
	else if (weight < 2.0f / d1)
	{
		weight -= 1.5f / d1;
		t = n1 * weight * weight + 0.75f;
	}
	else if (weight < 2.5f / d1)
	{
		weight -= 2.25f / d1;
		t = n1 * weight * weight + 0.9375f;
	}
	else
	{
		weight -= 2.625f / d1;
		t = n1 * weight * weight + 0.984375f;
	}

	return lerp(from, to, t);
}

inline float lerpBackIn(float from, float to, float weight)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;

	float t = c3 * weight * weight * weight - c1 * weight * weight;

	return lerp(from, to, t);
}
inline float lerpBackOut(float from, float to, float weight)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;

	float t = 1.0f + c3 * std::pow(weight - 1.0f, 3.0f) + c1 * std::pow(weight - 1.0f, 2.0f);

	return lerp(from, to, t);
}
inline float lerpBackInOut(float from, float to, float weight)
{
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;

	float t = 0.0f;

	if (weight < 0.5f)
	{
		t = (std::pow(2.0f * weight, 2.0f) * ((c2 + 1.0f) * 2.0f * weight - c2)) / 2.0f;
	}
	else
	{
		t = (std::pow(2.0f * weight - 2.0f, 2.0f) * ((c2 + 1.0f) * (weight * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
	}

	return lerp(from, to, t);
}
