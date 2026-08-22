#include "shader.h"

#include "file.h"

#include <glm/gtc/type_ptr.hpp>

#ifdef __EMSCRIPTEN__
constexpr const char* kShaderHeader = "#version 300 es\nprecision mediump float;\n";
#else
constexpr const char* kShaderHeader = "#version 330 core\n";
#endif

namespace blockgame
{
    Shader LoadShader(const std::string& vertPath, const std::string& fragPath)
    {
        GLuint vs = CompileShader(GL_VERTEX_SHADER, vertPath);
        GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragPath);

        if (!vs || !fs)
        {
            if (vs)
            {
                glDeleteShader(vs);
            }

            if (fs)
            {
                glDeleteShader(fs);
            }

            return Shader{};
        }

        GLuint program = glCreateProgram();
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);

        GLint linked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &linked);

        if (!linked)
        {
            char log[512];
            glGetProgramInfoLog(program, sizeof(log), nullptr, log);
            std::fprintf(stderr, "Shader link error (%s / %s): %s\n", vertPath.c_str(), fragPath.c_str(), log);
            glDeleteShader(vs);
            glDeleteShader(fs);
            glDeleteProgram(program);
            return Shader{};
        }

        glDeleteShader(vs);
        glDeleteShader(fs);

        Shader shader;
        shader.id = program;
        return shader;
    }

    GLuint CompileShader(GLenum type, const std::string& path)
    {
        std::string body = blockgame::LoadFileAsString(path);
        if (body.empty())
        {
            return 0;
        }

        std::string source = std::string(kShaderHeader) + body;
        const char* src = source.c_str();

        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        GLint success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            char log[512];
            glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
            std::fprintf(stderr, "Shader compile error: %s\n", log);
            return 0;
        }

        return shader;
    }

    void SetUniformMat4(GLuint program, const char* name, const glm::mat4& value)
    {
        glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, glm::value_ptr(value));
    }

    void SetUniformInt(GLuint program, const char* name, int value)
    {
        glUniform1i(glGetUniformLocation(program, name), value);
    }

    void SetUniformVec4(GLuint program, const char* name, const glm::vec4& value)
    {
        glUniform4fv(glGetUniformLocation(program, name), 1, glm::value_ptr(value));
    }
} // namespace blockgame
