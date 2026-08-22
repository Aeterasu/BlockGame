#pragma once

#include <cstdint>
#include <glm/glm.hpp>

namespace blockgame
{
    enum class BlockType : std::int8_t
    {
        NONE = 0,
        RED = 1,
        BLUE = 2,
        GREEN = 3,
    };

    struct Block
    {
        glm::ivec2 position;
        BlockType type;
    };
} // namespace blockgame
