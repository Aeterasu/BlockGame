#include "game.h"

#include "renderer.h"
#include "shader_storage.h"
#include "texture_storage.h"

namespace blockgame
{
    void Game::init()
    {
        Sprite g;
        g.size = glm::vec2(270.0f, 270.0f);
        g.texture = &textureStorage.grid;
        g.shader = &shaderStorage.spriteShader;
        gridSprite = renderer.AddSprite(g);

        std::fprintf(stderr, "Game initialized!");
    }
} // namespace blockgame
