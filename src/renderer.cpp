#include "renderer.h"

#include "sprite.h"

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace blockgame
{
    Renderer renderer;
    bool Renderer::InitGL()
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

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        InitQuad();

        projection = glm::ortho(0.0f, 270.0f, 270.0f, 0.0f, -1.0f, 1.0f);

        return true;
    }

    void Renderer::InitQuad()
    {
        // clang-format off
				float vertices[] = 
				{
						0.0f, 1.0f, 0.0f, 1.0f,
						1.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 0.0f,

						0.0f, 1.0f, 0.0f, 1.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						1.0f, 0.0f, 1.0f, 0.0f,
				};
        // clang-format on

        GLuint vbo = 0;
        glGenVertexArrays(1, &quadVao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(quadVao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }
    void Renderer::AddSprite(const Sprite& sprite)
    {
        activeSprites.push_back(sprite);
    }

    void Renderer::RemoveSprite(const Sprite& sprite)
    {
        auto it = std::find_if(activeSprites.begin(), activeSprites.end(),
                               [&sprite](const Sprite& current) { return &current == &sprite; });

        if (it != activeSprites.end())
        {
            activeSprites.erase(it);
        }
    }

    void Renderer::DrawFrame()
    {
        glClearColor(0.11444f, 0.09531f, 0.07819f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (Sprite& sprite : activeSprites)
        {
            blockgame::DrawSprite(sprite, projection, quadVao);
        }
    }

    void Renderer::DeleteGL()
    {
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
    }
} // namespace blockgame
