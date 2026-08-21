#pragma once

#include <cstdint>

namespace blockgame
{

    using int8 = int8_t;
    using int16 = int16_t;
    using int32 = int32_t;
    using int64 = int64_t;

    using uint8 = uint8_t;
    using uint16 = uint16_t;
    using uint32 = uint32_t;
    using uint64 = uint64_t;

    using float32 = float;
    using float64 = double;

    struct Vec2i
    {
        int x, y;

        constexpr Vec2i() : x(0), y(0)
        {
        }
        constexpr Vec2i(int x_, int y_) : x(x_), y(y_)
        {
        }
    };
} // namespace blockgame
