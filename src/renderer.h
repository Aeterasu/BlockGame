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
        std::vector<SpriteHandle> handles;

        SpriteHandle nextHandle = 0;

        GLuint quadVao = 0;
        glm::mat4 projection{1.0f};

        SpriteHandle AddSprite(const Sprite& sprite);
        void RemoveSprite(SpriteHandle handle);
        void UpdateSprite(SpriteHandle handle, const Sprite& sprite);

        void InitQuad();

        bool InitGL();

        void DrawFrame();

        void DeleteGL();
    };

    extern Renderer renderer;
} // namespace blockgame
