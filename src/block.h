#include "vec2i.h"

namespace BlockGame
{
	enum class Color
	{
		NONE = 0,
		RED = 1,
		BLUE = 2,
		GREEN = 3,
	};

	struct Block
	{
		Vec2i position;
		Color color;
	};
}
