#include "texture_storage.h"

namespace blockgame
{
    TextureStorage textureStorage;

    void InitTextureStorage()
    {
        textureStorage.grid = LoadTexture("assets/textures/texture_grid.png");
        textureStorage.border = LoadTexture("assets/textures/texture_border.png");
        textureStorage.block = LoadTexture("assets/textures/texture_block.png");
    }
} // namespace blockgame
