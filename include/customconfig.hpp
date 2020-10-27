#pragma once
#include "ConfigUtils/shared/ConfigUtils.hpp"

DECLARE_CONFIG(CustomConfig,

    DECLARE_VALUE(MenuHighscore, bool, "Menu Highscore Percentage", true);
    DECLARE_VALUE(LevelEndRank, bool, "Level End Rank Display", true);
    DECLARE_VALUE(AverageCutScore, bool, "Average Cut Score", true);
    DECLARE_VALUE(ScoreDifference, bool, "Score Difference", true);
    DECLARE_VALUE(ScorePercentageDifference, bool, "Score Percentage Difference", true);

    INIT_FUNCTION(
        INIT_VALUE(MenuHighscore);
        INIT_VALUE(LevelEndRank);
        INIT_VALUE(AverageCutScore);
        INIT_VALUE(ScoreDifference);
        INIT_VALUE(ScorePercentageDifference);
    )
)

