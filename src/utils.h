#pragma once

#include <array>
#include <charconv>
#include <cstdint>
#include <string>

inline std::string DoubleToString(double value, uint32_t precision)
{
	std::array<char, 512> buffer{};

	auto result =
		std::to_chars(buffer.data(), buffer.data() + buffer.size(), value, std::chars_format::fixed, precision);

	return std::string(buffer.data(), result.ptr);
}
