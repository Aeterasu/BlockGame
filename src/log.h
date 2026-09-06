#pragma once

#include <iostream>

namespace blockgame
{
	template <typename... Args>
	void log(Args&&... args)
	{
		(std::cout << ... << args) << '\n';
	}

	template <typename... Args>
	void log_err(Args&&... args)
	{
		(std::cerr << ... << args) << '\n';
	}
} // namespace blockgame

using blockgame::log;
using blockgame::log_err;
