#pragma once

#include "block.h"
#include "scoring.h"
#include "sprite.h"
#include "text.h"

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

		glm::ivec2 cursorGridPosition{0, GRID_SIZE.y - 1};
		glm::vec2 cursorRealPosition{0.0f, 0.0f};

		bool isDragging = false;

		glm::ivec2 bombGridPosition{0, 0};
		bool isBombActive = false;
		double bombTimeLeft = 0.0;
		const double BOMB_TIMER = 1.0;

		Sprite blockSprite;
		SpriteHandle blockSpriteHandle;

		SpriteHandle gridSpriteHandle;

		Sprite cursorSprite;
		SpriteHandle cursorSpriteHandle;

		Sprite bombSprite;
		SpriteHandle bombSpriteHandle;

		Scoring scoring;
		Label scoreLabel;
		uint64_t lastDisplayedScore = 0;
		uint64_t displayedScore = 0;
		double scoringLerpTime = 0.0;

		void Init();
		void Tick(const double delta);
		void UpdateBlock(const glm::ivec2 gridPosition, const Block newState);

		glm::vec2 GridPositionToRealPosition(const glm::ivec2 gridPosition);
		size_t GridPositionToId(const glm::ivec2 gridPosition);
		glm::ivec2 IdToGridPosition(const size_t id);
		Block GetBlockAtPosition(const glm::ivec2 gridPosition);

		bool IsValidGridPosition(const glm::ivec2 gridPosition);

		std::vector<size_t> GetConnectedGroup(const glm::ivec2 startPos);
		void MoveCursor(const glm::ivec2 dir);

		void PlaceBomb();
		void ExplodeBomb();
	};
} // namespace blockgame
