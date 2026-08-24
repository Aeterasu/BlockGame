#pragma once

#include "shader.h"
#include "texture.h"

#include <glm/glm.hpp>

namespace blockgame
{
    using SpriteHandle = std::uint32_t;

    struct Sprite
    {
        glm::vec2 position{0.0f, 0.0f};
        glm::vec2 size{32.0f, 32.0f};
        float rotation = 0.0f;
        glm::vec4 tint{1.0f, 1.0f, 1.0f, 1.0f};

        std::int32_t zIndex = 0;

        const Texture* texture = nullptr;
        const Shader* shader = nullptr;
    };

    void DrawSprite(const Sprite& sprite, const glm::mat4& projection, GLuint quadVao);
} // namespace blockgame
