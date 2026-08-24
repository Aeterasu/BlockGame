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

    extern TextureStorage texture_storage;

    void InitTextureStorage();
} // namespace blockgame
