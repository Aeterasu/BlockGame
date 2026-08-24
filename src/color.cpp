#include "color.h"

#include <cstdint>
#include <string>

namespace blockgame
{
    Color ColorFromHex(const std::string& hex)
    {
        uint32_t value = std::stoul(hex.substr(hex[0] == '#' ? 1 : 0), nullptr, 16);

        return Color(((value >> 16) & 0xFF) / 255.0f, ((value >> 8) & 0xFF) / 255.0f, (value & 0xFF) / 255.0f, 1.0f);
    }

    glm::vec4 Vec4FromColor(const Color color)
    {
        return glm::vec4{color.r, color.g, color.b, color.a};
    }

    Color ColorFromVec4(const glm::vec4 vec)
    {
        return Color(vec.x, vec.y, vec.z, vec.w);
    }
} // namespace blockgame
