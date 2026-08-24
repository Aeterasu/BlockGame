#pragma once

#include "sprite.h"

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <vector>

namespace blockgame
{
    struct Renderer
    {
        SDL_Window* window = nullptr;
        SDL_GLContext glContext = nullptr;
        std::vector<Sprite> activeSprites;

        GLuint quadVao = 0;
        glm::mat4 projection{1.0f};

        void AddSprite(const Sprite& sprite);
        void RemoveSprite(const Sprite& sprite);

        void InitQuad();

        bool InitGL();

        void DrawFrame();

        void DeleteGL();
    };

    extern Renderer renderer;
} // namespace blockgame
