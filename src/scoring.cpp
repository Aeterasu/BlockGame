#include "scoring.h"

#include "lerp.h"

#include <algorithm>
#include <iostream>

namespace blockgame
{
	void Scoring::AwardScore(const uint64_t combo)
	{
		score += combo * BASE_REWARD * GetComboMultiplier(combo);

		scoringLerpTime = 0.0;

		isDraining = false;

		std::cout << "multiplier: " << GetComboMultiplier(combo) << "\n";
	}

	void Scoring::Tick(const double delta)
	{
		// label

		scoringLerpTime = std::min(scoringLerpTime + delta * 5.0, 1.0);
		displayedScore = lerpInt64(displayedScore, score, scoringLerpTime);

		if (displayedScore != lastDisplayedScore)
		{
			lastDisplayedScore = displayedScore;

			if (scoreLabel)
			{
				scoreLabel->SetText("SCORE: " + FormatScore(displayedScore));
			}
		}
	}

	void Scoring::TickTurn()
	{
		DrainScore();
	}

	void Scoring::DrainScore()
	{
		score -= std::min(score, DRAIN_AMOUNT);

		scoringLerpTime = 0.0;
	}
} // namespace blockgame
