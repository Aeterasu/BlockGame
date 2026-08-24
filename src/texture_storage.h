#pragma once

#include "texture.h"

namespace blockgame
{
    struct TextureStorage
    {
        Texture grid;
        Texture border;
        Texture block;
    };

    extern TextureStorage textureStorage;

    void InitTextureStorage();
} // namespace blockgame
