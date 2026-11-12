#pragma once

typedef struct
{
	float m[9];
} Mat3f;

typedef struct
{
	float x, y;
} Vec2f;

typedef struct
{
	Vec2f size;
	Vec2f position;
} Rect2f;

typedef struct
{
	Vec2f position;
	Vec2f scale;
	float rotation;
} Transform2D;

Mat3f mat3f_identity(void);
Mat3f mat3f_multiply(Mat3f a, Mat3f b);
Mat3f mat3f_translate(float x, float y);
Mat3f mat3f_rotate(float angle); // in radians, obviously.
Mat3f mat3f_scale(float x, float y);
Mat3f mat3f_orthographic(float left, float right, float bottom, float top);

Mat3f mat3f_get_from_transform_2d(Transform2D transform);
