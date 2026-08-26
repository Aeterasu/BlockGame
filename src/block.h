#pragma once

#include <cstdint>
#include <glm/glm.hpp>

namespace blockgame
{
	enum class Block : std::uint8_t
	{
		NONE = 0,
		RED = 1,
		BLUE = 2,
		GREEN = 3,
	};
} // namespace blockgame
