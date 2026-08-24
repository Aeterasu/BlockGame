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
} // namespace blockgame
