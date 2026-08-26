#include "game.h"

#include "color.h"
#include "pico_palette.h"
#include "renderer.h"
#include "rng.h"
#include "shader_storage.h"
#include "texture_storage.h"

#include <iostream>

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

        blockSpawnTimeRemaining = BLOCK_SPAWN_TIME;

        std::fprintf(stderr, "Game initialized!");
    }

    void Game::Tick(const double delta)
    {
        blockSpawnTimeRemaining -= delta;

        if (blockSpawnTimeRemaining <= 0.0)
        {
            std::cout << "MEOW" << "\n";
            int32_t x = (Random_NextByte() % 8);
            int32_t y = (Random_NextByte() % 8);
            UpdateBlock(glm::ivec2{x, y}, (blockgame::Block)(1 + Random_NextByte() % 3));

            blockSpawnTimeRemaining = BLOCK_SPAWN_TIME;
        }
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
        return glm::vec2(41.0f + gridPosition.x * 23.0f, 41.0f + gridPosition.y * 23.0f);
    }

    size_t Game::GridPositionToId(const glm::ivec2 gridPosition)
    {
        return gridPosition.y * GRID_SIZE.x + gridPosition.x;
    }

    glm::ivec2 Game::IdToGridPosition(const size_t id)
    {
        return glm::ivec2(id % GRID_SIZE.x, id / GRID_SIZE.x);
    }
} // namespace blockgame
