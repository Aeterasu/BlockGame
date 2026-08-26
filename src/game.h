#pragma once

#include "block.h"
#include "sprite.h"

#include <vector>

namespace blockgame
{
	struct Game
	{
		static constexpr glm::ivec2 GRID_SIZE = glm::ivec2{8, 8};

		const double BLOCK_SPAWN_TIME = 1.5;
		double blockSpawnTimeRemaining = 0.0;

		std::vector<Block> blocks;
		std::vector<SpriteHandle> blockSpriteHandles;

		glm::ivec2 currentCursorPosition{0, GRID_SIZE.y - 1};

		Sprite blockSprite;
		SpriteHandle blockSpriteHandle;

		SpriteHandle gridSpriteHandle;

		Sprite cursorSprite;
		SpriteHandle cursorSpriteHandle;

		void Init();
		void Tick(const double delta);
		void UpdateBlock(const glm::ivec2 gridPosition, const Block newState);

		glm::vec2 GridPositionToRealPosition(const glm::ivec2 gridPosition);
		size_t GridPositionToId(const glm::ivec2 gridPosition);
		glm::ivec2 IdToGridPosition(const size_t id);

		void MoveCursor(const glm::ivec2 dir);
	};
} // namespace blockgame
