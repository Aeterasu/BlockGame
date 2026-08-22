#pragma once

#include "grid.h"
#include "sprite.h"

namespace blockgame
{
    struct Game
    {
        Grid grid;
        Sprite gridSprite;
        Texture gridTexture;
        Shader spriteShader;

        void init();
    };
} // namespace blockgame
