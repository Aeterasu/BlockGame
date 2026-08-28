#pragma once

#include <cstdint>
#include <iostream>

namespace blockgame
{
	struct Scoring
	{
		std::uint64_t score = 0;

		const std::uint64_t BASE_REWARD = 100;

		inline std::uint64_t GetComboMultiplier(uint64_t comboCount)
		{
			if (comboCount <= 1)
			{
				return 1;
			}

			if (comboCount <= 3)
			{
				return 8;
			}

			if (comboCount <= 8)
			{
				return 32;
			}

			if (comboCount <= 16)
			{
				return 64;
			}

			if (comboCount > 16)
			{
				return 128;
			}

			return 1;
		}

		inline void AwardScore(uint64_t combo)
		{
			score += combo * BASE_REWARD * GetComboMultiplier(combo);
			std::cout << "multiplier: " << GetComboMultiplier(combo) << "\n";
		}
	};
} // namespace blockgame
