#include "game.h"

#include "color.h"
#include "lerp.h"
#include "pico_palette.h"
#include "renderer.h"
#include "rng.h"
#include "shader_storage.h"
#include "texture_storage.h"

#include <SDL2/SDL.h>

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
			blocks.push_back(Block::NONE);

			blockSprite.size = glm::vec2(26.0f, 26.0f);
			blockSprite.texture = &textureStorage.block;
			blockSprite.shader = &shaderStorage.spriteShader;
			blockSpriteHandles.push_back(renderer.AddSprite(blockSprite));

			glm::ivec2 pos = IdToGridPosition(i);

			UpdateBlock(pos, Block::NONE);
		}

		cursorSprite.size = glm::vec2(26.0f, 26.0f);
		cursorSprite.texture = &textureStorage.cursor;
		cursorSprite.shader = &shaderStorage.spriteShader;
		cursorSprite.zIndex = 4096;
		cursorSpriteHandle = renderer.AddSprite(cursorSprite);

		blockSpawnTimeRemaining = BLOCK_SPAWN_TIME;

		std::fprintf(stderr, "Game initialized!");
	}

	void Game::Tick(const double delta)
	{
		blockSpawnTimeRemaining -= delta;

		if (blockSpawnTimeRemaining <= 0.0)
		{
			int32_t x = (Random_NextByte() % 8);
			int32_t y = (Random_NextByte() % 8);
			UpdateBlock(glm::ivec2{x, y}, (blockgame::Block)(1 + Random_NextByte() % 3));

			blockSpawnTimeRemaining = BLOCK_SPAWN_TIME;
		}

		// cursor

		auto cursorTargetPosition = GridPositionToRealPosition(cursorGridPosition);
		float weight = 1.0f - std::exp(-20.0f * delta);
		cursorRealPosition.x = lerp(cursorRealPosition.x, cursorTargetPosition.x, weight);
		cursorRealPosition.y = lerp(cursorRealPosition.y, cursorTargetPosition.y, weight);

		cursorSprite.position = cursorRealPosition;
		renderer.UpdateSprite(cursorSpriteHandle, cursorSprite);
	}

	void Game::UpdateBlock(const glm::ivec2 gridPosition, const Block newState)
	{
		auto id = GridPositionToId(gridPosition);

		blocks.at(id) = newState;

		blockSprite.position = GridPositionToRealPosition(gridPosition);

		switch (newState)
		{
			case Block::NONE:
				blockSprite.tint = glm::vec4(0.0, 0.0, 0.0, 0.0);
				break;
			case Block::RED:
				blockSprite.tint = Vec4FromColor(PICO_RED);
				break;
			case Block::BLUE:
				blockSprite.tint = Vec4FromColor(PICO_BLUE);
				break;
			case Block::GREEN:
				blockSprite.tint = Vec4FromColor(PICO_GREEN);
				break;
			default:
				blockSprite.tint = glm::vec4(0.0, 0.0, 0.0, 0.0);
		}

		renderer.UpdateSprite(blockSpriteHandles.at(id), blockSprite);
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

			if (destPos.x < 0 || destPos.x >= GRID_SIZE.x || destPos.y < 0 || destPos.y >= GRID_SIZE.y)
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
		originalColors.reserve(movingIds.size());

		for (auto id : movingIds)
		{
			originalColors.push_back(blocks.at(id));
		}

		for (auto id : movingIds)
		{
			UpdateBlock(IdToGridPosition(id), Block::NONE);
		}

		for (size_t j = 0; j < movingIds.size(); j++)
		{
			glm::ivec2 newPos = IdToGridPosition(movingIds[j]) + dir;
			UpdateBlock(newPos, originalColors[j]);
		}

		cursorGridPosition += dir;
	}
} // namespace blockgame
