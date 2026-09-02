#include "game.h"

#include "color.h"
#include "font_storage.h"
#include "lerp.h"
#include "pico_palette.h"
#include "renderer.h"
#include "rng.h"
#include "shader_storage.h"
#include "texture_storage.h"

#include <SDL2/SDL.h>
#include <iostream>
#include <unordered_map>

namespace blockgame
{
	void Game::Init()
	{
		Sprite grid;
		grid.size = glm::vec2(270.0f, 270.0f);
		grid.texture = &textureStorage.grid;
		grid.shader = &shaderStorage.spriteShader;
		grid.zIndex = -999999;
		gridSpriteHandle = renderer.AddSprite(grid);

		for (size_t i = 0; i < GRID_SIZE.x * GRID_SIZE.y; i++)
		{
			blocks.at(i) = Block::NONE;

			blockSprite.size = glm::vec2(26.0f, 26.0f);
			blockSprite.texture = &textureStorage.block;
			blockSprite.shader = &shaderStorage.spriteShader;
			blockSpriteHandles.at(i) = renderer.AddSprite(blockSprite);

			glm::ivec2 pos = IdToGridPosition(i);

			UpdateBlock(pos, Block::NONE);
		}

		cursorSprite.size = glm::vec2(26.0f, 26.0f);
		cursorSprite.texture = &textureStorage.cursor;
		cursorSprite.shader = &shaderStorage.spriteShader;
		cursorSprite.zIndex = 4096;
		cursorSpriteHandle = renderer.AddSprite(cursorSprite);

		bombSprite.size = glm::vec2(26.0f, 26.0f);
		bombSprite.texture = &textureStorage.bomb;
		bombSprite.shader = &shaderStorage.spriteShader;
		bombSprite.zIndex = 2048;

		blockSpawnTimeRemaining = BLOCK_SPAWN_TIME;

		scoring.score = 0;

		scoreLabel.Create(fontStorage.superstar, "SCORE: 0", ColorToSDLColor(blockgame::PICO_DARK_BLUE),
						  &blockgame::shaderStorage.spriteShader, glm::vec2(37.0f, 24.0f), 999999);

		std::cout << "Game initialized!\n";
	}

	void Game::Tick(const double delta)
	{
		if (isGameOver)
		{
			return;
		}

		// blocks sliding

		for (size_t id = 0; id < blocks.size(); id++)
		{
			glm::vec2 target = GridPositionToRealPosition(IdToGridPosition(id));
			float weight = 1.0f - std::exp(-20.0f * delta); // same feel as the cursor
			blockRealPositions[id].x = lerp(blockRealPositions[id].x, target.x, weight);
			blockRealPositions[id].y = lerp(blockRealPositions[id].y, target.y, weight);

			blockSprite.position = blockRealPositions[id];
			blockSprite.tint = BlockTint(blocks[id]);
			renderer.UpdateSprite(blockSpriteHandles.at(id), blockSprite);
		}

		// block spawn

		blockSpawnTimeRemaining -= delta;

		if (blockSpawnTimeRemaining <= 0.0)
		{
			int i = 0;

			while (i < 99)
			{
				int32_t x = (Random_NextByte() % 8);
				int32_t y = (Random_NextByte() % 8);

				if (GetBlockAtPosition(glm::ivec2{x, y}) == Block::NONE)
				{
					UpdateBlock(glm::ivec2{x, y}, (blockgame::Block)(1 + Random_NextByte() % 3));
					break;
				}
				else
				{
					i++;
				}
			}

			blockSpawnTimeRemaining = BLOCK_SPAWN_TIME;
		}

		// bomb

		if (isBombActive)
		{
			bombTimeLeft -= delta;

			if (bombTimeLeft <= 0.0)
			{
				ExplodeBomb();
			}
		}

		// cursor

		auto cursorTargetPosition = GridPositionToRealPosition(cursorGridPosition);
		float weight = 1.0f - std::exp(-20.0f * delta);
		cursorRealPosition.x = lerp(cursorRealPosition.x, cursorTargetPosition.x, weight);
		cursorRealPosition.y = lerp(cursorRealPosition.y, cursorTargetPosition.y, weight);

		cursorSprite.position = cursorRealPosition;
		renderer.UpdateSprite(cursorSpriteHandle, cursorSprite);

		// score

		scoringLerpTime = std::min(scoringLerpTime + delta * 5.0, 1.0);
		displayedScore = lerpInt64(displayedScore, scoring.score, scoringLerpTime);

		if (displayedScore != lastDisplayedScore)
		{
			lastDisplayedScore = displayedScore;
			scoreLabel.SetText("SCORE: " + FormatScore(displayedScore));
		}

		// game over

		if (potentialGameOver)
		{
			gameOverTimer += delta;

			if (gameOverTimer > GAME_OVER_TIME_LIMIT)
			{
				isGameOver = true;
				std::cout << "Game over!" << "\n";
			}
		}
		else
		{
			return;
		}
	}

	void Game::UpdateBlock(const glm::ivec2 gridPosition, const Block newState)
	{
		auto id = GridPositionToId(gridPosition);

		blocks.at(id) = newState;

		blockSprite.position = GridPositionToRealPosition(gridPosition);

		CheckForGameOverState();
	}

	void Game::CheckForGameOverState()
	{
		potentialGameOver = true;

		for (size_t i = 0; i < GRID_SIZE.x * GRID_SIZE.y; i++)
		{
			if (blocks.at(i) == Block::NONE)
			{
				potentialGameOver = false;
				return;
			}
		}
	}

	glm::vec4 Game::BlockTint(const Block block)
	{
		switch (block)
		{
			case Block::RED:
				return Vec4FromColor(PICO_RED);
			case Block::BLUE:
				return Vec4FromColor(PICO_BLUE);
			case Block::GREEN:
				return Vec4FromColor(PICO_GREEN);
			default:
				return glm::vec4(0.0, 0.0, 0.0, 0.0);
		}
	}

	glm::vec2 Game::GridPositionToRealPosition(const glm::ivec2 gridPosition)
	{
		return glm::vec2(39.0f + gridPosition.x * 23.0f, 41.0f + gridPosition.y * 23.0f);
	}

	size_t Game::GridPositionToId(const glm::ivec2 gridPosition)
	{
		return gridPosition.y * GRID_SIZE.x + gridPosition.x;
	}

	glm::ivec2 Game::IdToGridPosition(const size_t id)
	{
		return glm::ivec2(id % GRID_SIZE.x, id / GRID_SIZE.x);
	}

	Block Game::GetBlockAtPosition(const glm::ivec2 gridPosition)
	{
		if (gridPosition.x < 0 || gridPosition.x >= GRID_SIZE.x || gridPosition.y < 0 || gridPosition.y >= GRID_SIZE.y)
		{
			return Block::NONE;
		}
		return blocks.at(GridPositionToId(gridPosition));
	}

	bool Game::IsValidGridPosition(const glm::ivec2 gridPosition)
	{
		return !(gridPosition.x < 0 || gridPosition.x >= GRID_SIZE.x || gridPosition.y < 0 ||
				 gridPosition.y >= GRID_SIZE.y);
	}

	std::vector<size_t> Game::GetConnectedGroup(const glm::ivec2 startPos)
	{
		std::vector<size_t> group;
		auto startId = GridPositionToId(startPos);
		Block color = blocks.at(startId);

		if (color == Block::NONE)
		{
			return group;
		}

		std::vector<bool> visited(blocks.size(), false);
		std::vector<glm::ivec2> stack;
		stack.push_back(startPos);
		visited[startId] = true;

		static const glm::ivec2 neighbors[4] = {glm::ivec2{1, 0}, glm::ivec2{-1, 0}, glm::ivec2{0, 1},
												glm::ivec2{0, -1}};

		while (!stack.empty())
		{
			glm::ivec2 pos = stack.back();
			stack.pop_back();
			group.push_back(GridPositionToId(pos));

			for (const auto& n : neighbors)
			{
				glm::ivec2 neighborPos = pos + n;

				if (neighborPos.x < 0 || neighborPos.x >= GRID_SIZE.x || neighborPos.y < 0 ||
					neighborPos.y >= GRID_SIZE.y)
				{
					continue;
				}

				auto neighborId = GridPositionToId(neighborPos);

				if (!visited[neighborId] && blocks.at(neighborId) == color)
				{
					visited[neighborId] = true;
					stack.push_back(neighborPos);
				}
			}
		}

		return group;
	}

	void Game::MoveCursor(const glm::ivec2 dir)
	{
		if (!isDragging)
		{
			cursorGridPosition += dir;
			cursorGridPosition.x = std::clamp(cursorGridPosition.x, 0, GRID_SIZE.x - 1);
			cursorGridPosition.y = std::clamp(cursorGridPosition.y, 0, GRID_SIZE.y - 1);
			return;
		}

		auto currentBlock = blocks.at(GridPositionToId(cursorGridPosition));

		if (currentBlock == Block::NONE)
		{
			return;
		}

		std::vector<size_t> movingIds = GetConnectedGroup(cursorGridPosition);
		std::vector<bool> isMoving(blocks.size(), false);

		for (auto id : movingIds)
		{
			isMoving[id] = true;
		}

		size_t i = 0;
		while (i < movingIds.size())
		{
			glm::ivec2 pos = IdToGridPosition(movingIds[i]);
			glm::ivec2 destPos = pos + dir;
			i++;

			if (!IsValidGridPosition(destPos))
			{
				return;
			}

			auto destId = GridPositionToId(destPos);

			if (isMoving[destId])
			{
				continue;
			}

			if (blocks.at(destId) == Block::NONE)
			{
				continue;
			}

			auto pushedGroup = GetConnectedGroup(destPos);

			for (auto pushedId : pushedGroup)
			{
				if (!isMoving[pushedId])
				{
					isMoving[pushedId] = true;
					movingIds.push_back(pushedId);
				}
			}
		}

		std::vector<Block> originalColors;
		std::vector<glm::vec2> originalRealPositions;
		originalColors.reserve(movingIds.size());
		originalRealPositions.reserve(movingIds.size());

		for (auto id : movingIds)
		{
			originalColors.push_back(blocks.at(id));
			originalRealPositions.push_back(blockRealPositions.at(id));
		}

		for (auto id : movingIds)
		{
			// UpdateBlock(IdToGridPosition(id), Block::NONE);
			blocks.at(id) = Block::NONE;
		}

		for (size_t j = 0; j < movingIds.size(); j++)
		{
			glm::ivec2 newPos = IdToGridPosition(movingIds[j]) + dir;
			auto destId = GridPositionToId(newPos);

			blocks.at(destId) = originalColors[j];
			blockRealPositions.at(destId) = originalRealPositions[j];
			// UpdateBlock(newPos, originalColors[j]);
		}

		cursorGridPosition += dir;
	}

	void Game::PlaceBomb()
	{
		if (isBombActive)
		{
			return;
		}

		if (GetBlockAtPosition(cursorGridPosition) != Block::NONE)
		{
			return;
		}

		bombGridPosition = cursorGridPosition;
		bombTimeLeft = BOMB_TIMER;

		isBombActive = true;

		bombSprite.position = GridPositionToRealPosition(bombGridPosition);
		bombSpriteHandle = renderer.AddSprite(bombSprite);
	}

	void Game::ExplodeBomb()
	{
		if (!isBombActive)
		{
			return;
		}

		isBombActive = false;

		std::vector<glm::ivec2> explosionChecks = {
			glm::ivec2(0, 0), glm::ivec2(-1, 0), glm::ivec2(1, 0), glm::ivec2(0, -1), glm::ivec2(0, 1),
		};

		for (auto dir : explosionChecks)
		{
			glm::ivec2 target = bombGridPosition + dir;

			if (!IsValidGridPosition(target))
			{
				continue;
			}

			auto explosionGroup = GetConnectedGroup(target);
			std::unordered_map<Block, uint64_t> comboCounter;

			for (auto id : explosionGroup)
			{
				scoringLerpTime = 0.0;

				Block blockType = blocks.at(id);

				if (blockType != Block::NONE)
				{
					comboCounter[blockType]++;
				}

				UpdateBlock(IdToGridPosition(id), Block::NONE);
			}

			for (const auto& [_, value] : comboCounter)
			{
				scoring.AwardScore(value);
				std::cout << "Score awarded! Combo counter: " << value << "\n";
			}
		}

		renderer.RemoveSprite(bombSpriteHandle);
	}
} // namespace blockgame
