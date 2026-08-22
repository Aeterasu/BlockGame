#include "game.h"

#include <stdio.h>

namespace blockgame
{
    void Game::init()
    {
        gridSprite.position = glm::vec2(0.0f, 1.0f);
        gridSprite.size = glm::vec2(270.0f, 270.0f);
        gridSprite.rotation = 0.0f;
        gridSprite.tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

        gridTexture = LoadTexture("assets/textures/texture_grid.png");
        gridSprite.texture = &gridTexture;

        spriteShader = LoadShader("assets/shaders/sprite.vertex.glsl", "assets/shaders/sprite.fragment.glsl");
        gridSprite.shader = &spriteShader;
        fprintf(stderr, "Game initialized!");
    }
} // namespace blockgame
