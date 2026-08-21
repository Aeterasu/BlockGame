#include <SDL2/SDL.h>
#include <SDL_video.h>

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#include <emscripten.h>
#else
#include <glad/gl.h>
#endif

#include "game.h"

#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>

namespace
{
    SDL_Window* window = nullptr;
    SDL_GLContext glContext = nullptr;
    GLuint shaderProgram = 0;
    GLuint vao = 0;

    bool running = true;

    blockgame::Game game;

    std::string LoadFileAsString(const std::string& path)
    {
        std::ifstream file(path);
        if (!file)
        {
            std::fprintf(stderr, "Failed to open file: %s\n", path.c_str());
            return "";
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

#ifdef __EMSCRIPTEN__
    constexpr const char* kShaderHeader = "#version 300 es\nprecision mediump float;\n";
#else
    constexpr const char* kShaderHeader = "#version 330 core\n";
#endif

    GLuint CompileShader(GLenum type, const std::string& path)
    {
        std::string body = LoadFileAsString(path);
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

    bool InitGL()
    {
#ifdef __EMSCRIPTEN__
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        window = SDL_CreateWindow("blockgame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 270, 270,
                                  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

        if (window == nullptr)
        {
            std::fprintf(stderr, "Game window could not be created, aborting...");
            return false;
        }

        glContext = SDL_GL_CreateContext(window);
        if (!glContext)
        {
            std::fprintf(stderr, "SDL_GL_CreateContext failed: %s\n", SDL_GetError());
            return false;
        }

#ifndef __EMSCRIPTEN__
        if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress))
        {
            std::fprintf(stderr, "Failed to load GL via GLAD\n");
            return false;
        }
#endif

        std::fprintf(stderr, "GL Version: %s\n", glGetString(GL_VERSION));
        std::fprintf(stderr, "GL Renderer: %s\n", glGetString(GL_RENDERER));

        GLuint vs = CompileShader(GL_VERTEX_SHADER, "assets/shaders/triangle.vertex.glsl");
        GLuint fs = CompileShader(GL_FRAGMENT_SHADER, "assets/shaders/triangle.fragment.glsl");

        if (!vs || !fs)
        {
            return false;
        }

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vs);
        glAttachShader(shaderProgram, fs);
        glLinkProgram(shaderProgram);

        GLint linked = 0;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);

        if (!linked)
        {
            char log[512];
            glGetProgramInfoLog(shaderProgram, sizeof(log), nullptr, log);
            std::fprintf(stderr, "Shader link error: %s\n", log);
            return false;
        }

        glDeleteShader(vs);
        glDeleteShader(fs);

        float vertices[] = {
            0.0f, 0.5f, 1.0f, 0.0f, 0.0f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        };

        GLuint vbo = 0;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        return true;
    }

    bool InitGame()
    {

        game.init();

        return true;
    }

    void Tick()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        glClearColor(0.11444f, 0.09531f, 0.07819f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(window);

#ifdef __EMSCRIPTEN__
        if (!running)
        {
            emscripten_cancel_main_loop();
        }
#endif
    }
} // namespace

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
    }

    if (!InitGL())
    {
        SDL_Quit();
        return -1;
    }

    InitGame();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(Tick, 0, 1);
#else
    while (running)
    {
        Tick();
    }
#endif

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
