#pragma once

#include <cstdint>

namespace blockgame
{
	enum class DifficultyRank : int8_t
	{
		LEVEL_1,
		LEVEL_2,
		LEVEL_3,
		LEVEL_4,
		LEVEL_5,
	};

	struct Difficulty
	{
		DifficultyRank rank = DifficultyRank::LEVEL_1;

		void IncreaseRank();
		void DecreaseRank();
		void ResetRank();
		void SetRank(const DifficultyRank rank_);
	};
} // namespace blockgame
