#pragma once

#include "grid.h"
#include "sprite.h"

namespace blockgame
{
    struct Game
    {
        Grid grid;
        Sprite gridSprite;
        Sprite blockSprite;
        Texture gridTexture;
        Texture blockTexture;
        Shader spriteShader;
        Shader blockShader;

        void init();
    };
} // namespace blockgame
