#include "shader_storage.h"

#include "shader.h"

namespace blockgame
{
    ShaderStorage shaderStorage;

    void InitShaderStorage()
    {
        shaderStorage.spriteShader =
            LoadShader("assets/shaders/sprite.vertex.glsl", "assets/shaders/sprite.fragment.glsl");
    }
} // namespace blockgame
