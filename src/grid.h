#include <vector>
#include "block.h"

namespace BlockGame
{
	struct Grid
	{
		static constexpr Vec2i GRID_SIZE = Vec2i{8, 8};

		std::vector<Block> blocks;

		Block& get_at(Vec2i position){
			return blocks[position.y * GRID_SIZE.y + position.x];
		}
	};
}
