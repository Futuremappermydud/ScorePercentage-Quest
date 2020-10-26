#pragma once
#include "CustomConfigValue.hpp"

struct Config_t {
    CustomBoolConfigValue MenuHighscore = CustomBoolConfigValue("Menu Highscore Percentage", true);
    CustomBoolConfigValue LevelEndRank = CustomBoolConfigValue("Level End Rank Display", true);
    CustomBoolConfigValue AverageCutScore = CustomBoolConfigValue("Average Cut Score", true);
    CustomBoolConfigValue ScoreDifference = CustomBoolConfigValue("Score Difference", true);
    CustomBoolConfigValue ScorePercentageDifference = CustomBoolConfigValue("Score Percentage Difference", true);
};

extern Config_t Config;