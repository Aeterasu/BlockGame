#pragma once

#include <SDL_image.h>
#include <glad/gl.h>
#include <string>

namespace blockgame
{
    struct Texture
    {
        GLuint id = 0;
        int width = 0;
        int height = 0;
    };

    Texture LoadTexture(const std::string& path);
} // namespace blockgame
