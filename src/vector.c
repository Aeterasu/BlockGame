#include <math.h>

#include "vector.h"

Mat3f mat3f_identity(void)
{
	// clang-format off
	return (Mat3f)
	{{
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
	}};
	// clang-format on
}

// TODO: this is mildly ugly. can avoid a copy?
Mat3f mat3f_multiply(const Mat3f a, const Mat3f b)
{
	Mat3f result = {0};

	for (int col = 0; col < 3; col++)
	{
		for (int row = 0; row < 3; row++)
		{
			for (int k = 0; k < 3; k++)
			{
				result.m[col * 3 + row] += a.m[k * 3 + row] * b.m[col * 3 + k];
			}
		}
	}

	return result;
}

Mat3f mat3f_translate(const float x, const float y)
{
	// clang-format off
	return (Mat3f)
	{{
		1, 0, 0,
		0, 1, 0,
		x, y, 1
	}};
	// clang-format on
}

Mat3f mat3f_rotate(const float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);

	// clang-format off
	
	return (Mat3f)
	{{
		c, s, 0,
		-s, c, 0,
		0, 0, 1
	}};

	// clang-format on
}

Mat3f mat3f_scale(const float x, const float y)
{
	// clang-format off
	return (Mat3f)
	{{
		x, 0, 0,
		0, y, 0,
		0, 0, 1
	}};
	// clang-format on
}

Mat3f mat3f_orthographic(const float left, const float right, const float bottom, const float top)
{
	// clang-format off
	Mat3f result = 
	{{
		2.0f / (right - left), 0, 0,
		0, 2.0f / (top - bottom), 0,
		-(right + left) / (right - left),
		-(top + bottom) / (top - bottom), 
		1
	}};
	// clang-format on

	return result;
}

Mat3f mat3f_get_from_transform_2d(const Transform2D transform)
{
	Mat3f t = mat3f_translate(transform.position.x, transform.position.y);
	Mat3f r = mat3f_rotate(transform.rotation);
	Mat3f s = mat3f_scale(transform.scale.x, transform.scale.y);

	return mat3f_multiply(t, mat3f_multiply(r, s));
}
