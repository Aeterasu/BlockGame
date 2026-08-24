#include "game.h"

#include "color.h"
#include "pico_palette.h"
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

        Sprite border;
        border.size = glm::vec2(270.0f, 270.0f);
        border.texture = &textureStorage.border;
        border.shader = &shaderStorage.spriteShader;
        border.zIndex = 999;
        Color grey = PICO_GREY;
        border.tint = glm::vec4(grey.r, grey.g, grey.b, 1.0);
        renderer.AddSprite(border);

        std::fprintf(stderr, "Game initialized!");
    }
} // namespace blockgame
