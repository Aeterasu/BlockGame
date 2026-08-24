#include "game.h"

#include "renderer.h"
#include "texture_storage.h"

#include <cstdint>
#include <stdio.h>

namespace blockgame
{
    void Game::init()
    {
        gridSprite.size = glm::vec2(270.0f, 270.0f);

        gridTexture = texture_storage.grid;
        gridSprite.texture = &gridTexture;

        spriteShader = LoadShader("assets/shaders/sprite.vertex.glsl", "assets/shaders/sprite.fragment.glsl");
        gridSprite.shader = &spriteShader;

        const std::uint8_t BLOCKS_COUNT = 64;

        blockSprites.reserve(BLOCKS_COUNT);
        blockTexture = texture_storage.block;
        renderer.AddSprite(gridSprite);

        for (int i = 0; i < BLOCKS_COUNT; i++)
        {
            Sprite sprite;
            sprite.position = glm::vec2(38.0f + (i % 8) * 23.0f, 39.0f + (i / 8) * 23.0f);
            sprite.size = glm::vec2(26.0f, 26.0f);

            sprite.texture = &blockTexture;

            sprite.shader = &spriteShader;

            blockSprites.push_back(sprite);
        }

        for (auto& s : blockSprites)
        {
            renderer.AddSprite(s);
        }

        fprintf(stderr, "Game initialized!");
    }
} // namespace blockgame
