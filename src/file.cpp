#include "file.h"

#include <fstream>
#include <sstream>
#include <string>

namespace blockgame
{
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
} // namespace blockgame
