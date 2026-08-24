#pragma once

#include "shader.h"

namespace blockgame
{
    struct ShaderStorage
    {
        Shader spriteShader;
    };

    extern ShaderStorage shaderStorage;

    void InitShaderStorage();
} // namespace blockgame
