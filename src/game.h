#pragma once

#include "block.h"
#include "quad.h"
#include "scoring.h"
#include "text.h"

#include <array>
#include <cstdint>
#include <variant>
#include <vector>

namespace blockgame
{
	enum class Difficulty : uint8_t
	{
		LEVEL_1,
		LEVEL_2,
		LEVEL_3,
		LEVEL_4,
		LEVEL_5,
	};

	// limited time, score as much as possible
	struct BlitzMode
	{
		const double TIME = 180.0;
		double timeLeft = 180.0;

		void Tick(double delta);
	};

	// endless fun
	struct EndlessMode
	{
	};

	// difficulty is high, clear 400 blocks as fast as possible
	struct SpeedrunMode
	{
		const uint64_t TARGET_CLEARS = 500;
		uint64_t currentClears = 0;
		double time = 0.0;
	};

	using GameMode = std::variant<BlitzMode, EndlessMode, SpeedrunMode>;

	struct Game
	{
		static constexpr glm::ivec2 GRID_SIZE = glm::ivec2{8, 8};

		GameMode gameMode;

		bool isGameOver = false;

		const int32_t BLOCK_SPAWN_TURNS = 6;
		int32_t blockSpawnTurnsRemaining = 0;

		std::array<Block, 64> blocks;
		std::array<QuadHandle, 64> blockHandles;
		std::array<glm::vec2, 64> blockRealPositions;

		glm::ivec2 cursorGridPosition{0, GRID_SIZE.y - 1};
		glm::vec2 cursorRealPosition{0.0f, 0.0f};

		bool isDragging = false;

		glm::ivec2 bombGridPosition{0, 0};
		bool isBombActive = false;
		int32_t bombTurnsLeft = 0;
		const int32_t BOMB_EXPLOSION_TURNS = 3;

		const double GAME_OVER_TIME_LIMIT = 1.0;
		double gameOverTimer = 0.0;
		bool potentialGameOver = false;

		Quad blockQuad;
		QuadHandle blockHandle;

		QuadHandle gridHandle;

		Quad cursorQuad;
		QuadHandle cursorHandle;

		Quad bombQuad;
		QuadHandle bombHandle;

		Scoring scoring;
		Label scoreLabel;

		template <typename GameMode_> void Init();

		void Tick(const double delta);
		void TickTurn();
		void UpdateBlock(const glm::ivec2 gridPosition, const Block newState);

		glm::vec2 GridPositionToRealPosition(const glm::ivec2 gridPosition);
		size_t GridPositionToId(const glm::ivec2 gridPosition);
		glm::ivec2 IdToGridPosition(const size_t id);
		Block GetBlockAtPosition(const glm::ivec2 gridPosition);

		bool IsValidGridPosition(const glm::ivec2 gridPosition);

		void CheckForGameOverState();

		glm::vec4 BlockTint(const Block block);

		std::vector<size_t> GetConnectedGroup(const glm::ivec2 startPos);
		void MoveCursor(const glm::ivec2 dir);

		void PlaceBomb();
		void ExplodeBomb();
	};
} // namespace blockgame
