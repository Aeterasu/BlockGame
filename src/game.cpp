#include "game.h"

#include "gl_compatibility.h"
#include "renderer.h"

#include <stdio.h>

namespace blockgame
{
    void Game::init()
    {
        gridSprite.size = glm::vec2(270.0f, 270.0f);

        gridTexture = LoadTexture("assets/textures/texture_grid.png");
        gridSprite.texture = &gridTexture;

        spriteShader = LoadShader("assets/shaders/sprite.vertex.glsl", "assets/shaders/sprite.fragment.glsl");
        gridSprite.shader = &spriteShader;

        blockSprite.position = glm::vec2(135.0f, 135.0f);
        blockSprite.size = glm::vec2(26.0f, 26.0f);

        blockTexture = LoadTexture("assets/textures/texture_block.png");
        blockSprite.texture = &blockTexture;

        blockSprite.shader = &spriteShader;

        renderer.AddSprite(gridSprite);
        renderer.AddSprite(blockSprite);

        fprintf(stderr, "Game initialized!");
    }
} // namespace blockgame
