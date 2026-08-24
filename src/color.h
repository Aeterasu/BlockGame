#pragma once

#include <algorithm>
#include <glm/glm.hpp>
#include <string>

namespace blockgame
{
    struct Color
    {
        float r = 1.0f;
        float g = 1.0f;
        float b = 1.0f;
        float a = 1.0f;

        // clang-format off
        constexpr Color(float r_ = 1.0f, float g_ = 1.0f, float b_ = 1.0f, float a_ = 1.0f) :
						r(std::clamp(r_, 0.0f, 1.0f)),
						g(std::clamp(g_, 0.0f, 1.0f)),
						b(std::clamp(b_, 0.0f, 1.0f)),
						a(std::clamp(a_, 0.0f, 1.0f))
        {}
        // clang-format on
    };

    Color ColorFromHex(const std::string& hex);

    glm::vec4 Vec4FromColor(const Color color);
    Color ColorFromVec4(const glm::vec4 vec);
} // namespace blockgame
