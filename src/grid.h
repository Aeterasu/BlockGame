#pragma once

#include "block.h"

#include <glm/glm.hpp>
#include <vector>

namespace blockgame
{
    struct Grid
    {
        static constexpr glm::ivec2 GRID_SIZE = glm::ivec2{8, 8};

        std::vector<Block> blocks;

        Block& get_at(glm::ivec2 position)
        {
            return blocks[position.y * GRID_SIZE.y + position.x];
        }
    };
} // namespace blockgame
