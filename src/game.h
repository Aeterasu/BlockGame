#pragma once

#include "block.h"
#include "difficulty.h"
#include "quad.h"
#include "scoring.h"
#include "text.h"

#include <array>
#include <cstdint>
#include <variant>
#include <vector>

namespace blockgame
{
	// limited time, score as much as possible
	struct BlitzMode
	{
		const double TIME = 180.0;
		double timeLeft = 180.0;

		bool isTimeUp = false;

		Label timeLabel;

		void Init();
		void Tick(const double delta);
	};

	// endless fun
	struct EndlessMode
	{
		void Init();
		void Tick(const double delta);
	};

	// difficulty is high, clear a target count of blocks as fast as possible
	struct SpeedrunMode
	{
		const uint64_t TARGET_CLEARS = 500;
		uint64_t currentClears = 0;
		double time = 0.0;

		void Init();
		void Tick(const double delta);
	};

	using GameMode = std::variant<BlitzMode, EndlessMode, SpeedrunMode>;

	// different control types
	struct PlayerCursorControl
	{
		void UpdateMove(glm::ivec2 dir);
	};

	struct PlayerSokobanControl
	{
		void UpdateMove(glm::ivec2 dir);
	};

	using ControlType = std::variant<PlayerCursorControl, PlayerSokobanControl>;

	struct Game
	{
		static constexpr glm::ivec2 GRID_SIZE = glm::ivec2{8, 8};

		GameMode gameMode;

		bool isGameOver = false;

		const int32_t BLOCK_SPAWN_TURNS = 4;
		int32_t blockSpawnTurnsRemaining = 0;

		const int32_t BLOCK_TELEGRAPH_TURNS = 6;

		const size_t STARTING_BLOCKS_COUNT = 12;

		std::array<Block, 64> blocks;
		std::array<QuadHandle, 64> blockHandles;
		std::array<glm::vec2, 64> blockRealPositions;

		std::array<int32_t, 64> blockTelegraphProgress;
		std::array<float, 64> blockTelegraphRealProgress;
		std::array<Block, 64> blockTelegraphColors;
		std::array<QuadHandle, 64> blockTelegraphHandles;

		std::array<QuadHandle, 64> blockWarningHandles;

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
		Quad telegraphQuad;
		Quad warningQuad;

		QuadHandle gridHandle;

		Quad cursorQuad;
		QuadHandle cursorHandle;

		Quad bombQuad;
		QuadHandle bombHandle;

		Difficulty difficulty;

		Scoring scoring;
		Label scoreLabel;

		template <typename GameMode_> void Init();

		void Tick(const double delta);
		void UpdateInput();
		void TickTurn();
		void UpdateBlock(const glm::ivec2 gridPosition, const Block newState);
		void SpawnRandomBlock(const bool instant = false);

		glm::vec2 GridPositionToRealPosition(const glm::ivec2 gridPosition, const float size = 23.0f,
											 const float offset = 0.0f);
		size_t GridPositionToId(const glm::ivec2 gridPosition);
		glm::ivec2 IdToGridPosition(const size_t id);
		Block GetBlockAtPosition(const glm::ivec2 gridPosition);

		bool IsValidGridPosition(const glm::ivec2 gridPosition);

		void CheckForGameOverState();

		glm::vec4 GetBlockTint(const Block block);

		std::vector<size_t> GetConnectedGroup(const glm::ivec2 startPos);
		bool AttemptMoveBlocks(const glm::ivec2 start, const glm::ivec2 target);
		void MoveCursor(const glm::ivec2 dir);

		void PlaceBomb();
		void ExplodeBomb();
	};
} // namespace blockgame
