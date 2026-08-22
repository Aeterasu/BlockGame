#pragma once

#include <SDL2/SDL_image.h>
#ifdef __EMSCRIPTEN__
    #include <GLES3/gl3.h>
#else
    #include <glad/gl.h>
#endif

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
