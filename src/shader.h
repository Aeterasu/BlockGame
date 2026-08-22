#pragma once

#include <glm/glm.hpp>
#include <string>

#ifdef __EMSCRIPTEN__
    #include <GLES3/gl3.h>
#else
    #include <glad/gl.h>
#endif

namespace blockgame
{
    struct Shader
    {
        GLuint id = 0;
    };

    Shader LoadShader(const std::string& vertPath, const std::string& fragPath);
    GLuint CompileShader(GLenum type, const std::string& path);

    void SetUniformMat4(GLuint program, const char* name, const glm::mat4& value);
    void SetUniformInt(GLuint program, const char* name, int value);
    void SetUniformVec4(GLuint program, const char* name, const glm::vec4& value);
} // namespace blockgame
