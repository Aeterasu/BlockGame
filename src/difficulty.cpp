#include "difficulty.h"

#include <algorithm>

namespace blockgame
{
	void Difficulty::IncreaseRank()
	{
		rank = static_cast<DifficultyRank>(
			std::min<int8_t>(static_cast<int8_t>(rank) + 1, static_cast<int8_t>(DifficultyRank::LEVEL_5)));
	}

	void Difficulty::DecreaseRank()
	{
		rank = static_cast<DifficultyRank>(
			std::max<int8_t>(static_cast<int8_t>(rank) - 1, static_cast<int8_t>(DifficultyRank::LEVEL_1)));
	}

	void Difficulty::ResetRank()
	{
		rank = DifficultyRank::LEVEL_1;
	}

	void Difficulty::SetRank(const DifficultyRank rank_)
	{
		rank = rank_;
	}
} // namespace blockgame
