#include "texture_storage.h"

namespace blockgame
{
    TextureStorage texture_storage;

    void InitTextureStorage()
    {
        texture_storage.grid = LoadTexture("assets/textures/texture_grid.png");
        texture_storage.border = LoadTexture("assets/textures/texture_border.png");
        texture_storage.block = LoadTexture("assets/textures/texture_block.png");
    }
} // namespace blockgame
