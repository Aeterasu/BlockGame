#pragma once

#include "grid.h"
#include "sprite.h"

namespace blockgame
{
    struct Game
    {
        std::vector<Sprite> blockSprites;
        Texture blockTexture;

        Grid grid;
        Sprite gridSprite;
        Texture gridTexture;

        Shader spriteShader;
        void init();
    };
} // namespace blockgame
