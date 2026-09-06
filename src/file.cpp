#include "file.h"

#include "log.h"

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
			log_err("Failed to open file: ", path);
			return "";
		}
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}
} // namespace blockgame
