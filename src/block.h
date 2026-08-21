#pragma once

#include "types.h"

namespace blockgame
{
    enum class Color : uint8
    {
        NONE = 0,
        RED = 1,
        BLUE = 2,
        GREEN = 3,
    };

    struct Block
    {
        Vec2i position;
        Color color;
    };
} // namespace blockgame
