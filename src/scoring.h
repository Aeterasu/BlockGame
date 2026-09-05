#pragma once

#include "text.h"

#include <cstdint>

namespace blockgame
{
	struct Scoring
	{
		std::uint64_t score = 0;

		const std::uint64_t DRAIN_AMOUNT = 25;

		bool isDraining = false;

		const std::uint64_t BASE_REWARD = 100;

		uint64_t lastDisplayedScore = 0;
		uint64_t displayedScore = 0;
		double scoringLerpTime = 0.0;

		Label* scoreLabel;

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

		void AwardScore(const uint64_t combo);

		void Tick(const double delta);
		void TickTurn();

		void DrainScore();
	};
} // namespace blockgame
