#pragma once

#include "grid.h"
#include "sprite.h"

namespace blockgame
{
    struct Game
    {
        Grid grid;
        SpriteHandle gridSprite;

        void init();
    };
} // namespace blockgame
