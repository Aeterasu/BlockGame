#include "game.h"

#include <stdio.h>

namespace blockgame
{
    void Game::init()
    {
        gridSprite.position = glm::vec2(0.0f, 0.0f);
        gridSprite.size = glm::vec2(270.0f, 270.0f);
        gridSprite.rotation = 0.0f;
        gridSprite.tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

        gridTexture = LoadTexture("assets/textures/texture_grid.png");
        gridSprite.texture = &gridTexture;

        spriteShader = LoadShader("assets/shaders/sprite.vertex.glsl", "assets/shaders/sprite.fragment.glsl");
        gridSprite.shader = &spriteShader;

        blockSprite.position = glm::vec2(135.0f, 135.0f);
        blockSprite.size = glm::vec2(26.0f, 26.0f);
        blockSprite.rotation = 0.0f;
        blockSprite.tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

        blockTexture = LoadTexture("assets/textures/texture_block.png");
        blockSprite.texture = &blockTexture;

        blockSprite.shader = &spriteShader;

        fprintf(stderr, "Game initialized!");
    }
} // namespace blockgame
