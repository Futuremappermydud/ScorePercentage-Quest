


#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

#include "custom-types/shared/register.hpp"
#include "custom-types/shared/types.hpp"

#include "customlogger.hpp"
#include "customconfig.hpp"

#include "UIController.hpp"

#include <iomanip>
#include <sstream>

#include "GlobalNamespace/ResultsViewController.hpp"
#include "GlobalNamespace/StandardLevelDetailView.hpp"
#include "GlobalNamespace/PlatformLeaderboardViewController.hpp"
#include "GlobalNamespace/PlayerLevelStatsData.hpp"
#include "System/Math.hpp"
#include "GlobalNamespace/ScoreFormatter.hpp"
#include "UnityEngine/GameObject.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "GlobalNamespace/LevelCompletionResults.hpp"
#include "GlobalNamespace/LevelStatsView.hpp"
#include "GlobalNamespace/PlayerData.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/IDifficultyBeatmapSet.hpp"
#include "GlobalNamespace/IBeatmapLevel.hpp"
#include "GlobalNamespace/BeatmapData.hpp"
#include "GlobalNamespace/GameplayModifiers.hpp"
#include "questui/shared/QuestUI.hpp"

static ModInfo modInfo;

using namespace GlobalNamespace;
using namespace TMPro;

const Logger& getLogger() {
    static const Logger logger(modInfo);
    return logger;
}

Configuration& getConfig() {
    static Configuration configuration(modInfo);
    return configuration;
}

DEFINE_CONFIG(CustomConfig);

static std::string Round (float val, int precision = 2)
{
	std::stringstream stream;
    stream << std::fixed << std::setprecision(precision) << val;
    std::string Out = stream.str();
	return Out;
}

int currentScore = 0;
double currentPercentage = 0;

static int calculateMaxScore(int blockCount)
{
    int maxScore;
    if(blockCount < 14)
    {
        if (blockCount == 1)
        {
            maxScore = 115;
        }
        else if (blockCount < 5){
            maxScore = (blockCount - 1) * 230 + 115;
        }
        else
        {
            maxScore = (blockCount - 5) * 460 + 1035;
        }
    }
    else
    {
        maxScore = (blockCount - 13) * 920 + 4715;
    }
    return maxScore;
}

static double calculatePercentage(int maxScore, int resultScore)
{
    double resultPercentage = (double)(100 / (double)maxScore * (double)resultScore);
    return resultPercentage;
}

MAKE_HOOK_OFFSETLESS(Menu, void, LevelStatsView* self, IDifficultyBeatmap* difficultyBeatmap, PlayerData* playerData) {
    Menu(self, difficultyBeatmap, playerData);
    if (playerData != nullptr)
    {
        PlayerLevelStatsData* playerLevelStatsData = playerData->GetPlayerLevelStatsData(difficultyBeatmap->get_level()->get_levelID(), difficultyBeatmap->get_difficulty(), difficultyBeatmap->get_parentDifficultyBeatmapSet()->get_beatmapCharacteristic());
        currentScore = playerLevelStatsData->highScore;
        if (playerLevelStatsData->validScore)
        {
            //calculate maximum possilble score
            int currentDifficultyMaxScore = calculateMaxScore(difficultyBeatmap->get_beatmapData()->cuttableNotesType);
            //calculate actual score percentage
            double currentDifficultyPercentageScore = calculatePercentage(currentDifficultyMaxScore, playerLevelStatsData->highScore);
            currentPercentage = currentDifficultyPercentageScore;
            //add percentage to highScoreText if it isn't disabled
            if (getCustomConfig().MenuHighscore.GetValue())
            { 
                std::string highScoreText = std::to_string(playerLevelStatsData->highScore) + " " + "(" + Round(currentDifficultyPercentageScore) + "%)";
                self->highScoreText->SetText(il2cpp_utils::createcsstr(highScoreText));
            }
            return;
        }
    }
    // Set currentScore and currentPercentage to 0, if no playerData exists
    // Does this even do anything!?
    currentScore = 0;
    currentPercentage = 0;
}

MAKE_HOOK_OFFSETLESS(Results, void, ResultsViewController* self) {
    Results(self);
    int maxScore;
    double resultPercentage;
    int resultScore;
    int modifiedScore;
    // Default Rank Text
    std::string rankTextLine1 = to_utf8(csstrtostr(self->rankText->get_text()));
    std::string rankTextLine2 = "";
    // Colors
    std::string colorPositive = "#00B300";
    std::string colorNegative = "#FF0000";
    //Empty for negatives, "+" for positives
    std::string positiveIndicator = "";


    //Only calculate percentage, if map was successfully cleared
    if (self->levelCompletionResults->levelEndStateType == LevelCompletionResults::LevelEndStateType::Cleared)
    {
        modifiedScore = self->levelCompletionResults->modifiedScore;
        maxScore = calculateMaxScore(self->difficultyBeatmap->get_beatmapData()->cuttableNotesType);
        //use modifiedScore with negative multipliers

        GameplayModifiers* mods = self->levelCompletionResults->gameplayModifiers;

        if (mods->get_noFail() || mods->get_enabledObstacleType().NoObstacles || mods->noArrows || mods->noBombs)
        {
            resultScore = modifiedScore;
        }
        //use rawScore without and with positive modifiers to avoid going over 100% without recalculating maxScore
        else
        {
            resultScore = self->levelCompletionResults->rawScore;
        }
        
        resultPercentage = calculatePercentage(maxScore, resultScore);

        //disable wrapping and autosize (unneccessary?)
        self->rankText->set_autoSizeTextContainer(false);
        self->rankText->set_enableWordWrapping(false);


        //Rank Text Changes
        if (getCustomConfig().LevelEndRank.GetValue())
        {
            //Set Percentage to first line
            rankTextLine1 = "<line-height=30%><size=60%>" + Round(resultPercentage) + "<size=45%>%";
            // Add Average Cut Score to 2nd Line if enabled
            if (getCustomConfig().AverageCutScore.GetValue() && !getCustomConfig().ScorePercentageDifference.GetValue())
            {
                int averageCutScore = self->levelCompletionResults->averageCutScore;
                rankTextLine2 = "\n<size=40%>" + std::to_string(averageCutScore) + "<size=30%> / <size=40%>115";

            }
            // Add Percent Difference to 2nd Line if enabled and previous Score exists
            else if (getCustomConfig().ScorePercentageDifference.GetValue() && currentPercentage != 0)
            {
                double percentageDifference = resultPercentage - currentPercentage;
                std::string percentageDifferenceColor;
                //Better or same Score
                if (percentageDifference >= 0)
                {
                    percentageDifferenceColor = colorPositive;
                    positiveIndicator = "+";
                }
                //Worse Score
                else
                {
                    percentageDifferenceColor = colorNegative;
                    positiveIndicator = "";
                }
                rankTextLine2 = "\n<color=" + percentageDifferenceColor + "><size=40%>" + positiveIndicator + Round(percentageDifference) + "<size=30%>%";
            }
            self->newHighScoreText->SetActive(false);
        }//End Preparations for Changes to Rank Text

        self->rankText->SetText(il2cpp_utils::createcsstr(rankTextLine1 + rankTextLine2));


        //Add ScoreDifference Calculation if enabled
        if (getCustomConfig().ScoreDifference.GetValue())
        {
            std::string scoreDifference = "";
            std::string scoreDifferenceColor = "";
            if (currentScore != 0)
            {
                scoreDifference = to_utf8(csstrtostr(ScoreFormatter::Format(modifiedScore - currentScore)));
                //Better Score
                if ((modifiedScore - currentScore) > 0)
                {
                    scoreDifferenceColor = colorPositive;
                    positiveIndicator = "+";
                }
                //Worse Score
                else if ((modifiedScore - currentScore) < 0)
                {
                    scoreDifferenceColor = colorNegative;
                    positiveIndicator = "";
                }

                //Build new ScoreText std::string
                self->scoreText->SetText(il2cpp_utils::createcsstr("<line-height=30%><size=60%>" + to_utf8(csstrtostr(ScoreFormatter::Format(modifiedScore))) + "\n<size=40%><color=" + scoreDifferenceColor + "><size=40%>" + positiveIndicator + scoreDifference));
            }

        }//End ScoreDifference Calculation

    }
}

MAKE_HOOK_OFFSETLESS(Reset, void, PlatformLeaderboardViewController* self, IDifficultyBeatmap* difficultyBeatmap) {
    Reset(self, difficultyBeatmap);
}

extern "C" void setup(ModInfo& info) {
    info.id = "ScorePercentage";
    info.version = VERSION;
    modInfo = info;
    getConfig().Load();
    getCustomConfig().Init(&getConfig());
}



extern "C" void load() {
    il2cpp_functions::Init();

    QuestUI::Init();

    custom_types::Register::RegisterType<ScorePercentage::UIController>();
    QuestUI::Register::RegisterModSettingsViewController<ScorePercentage::UIController*>(modInfo, "Score Percentage Settings");
    INSTALL_HOOK_OFFSETLESS(Results, il2cpp_utils::FindMethod("", "ResultsViewController", "SetDataToUI"));
    INSTALL_HOOK_OFFSETLESS(Menu, il2cpp_utils::FindMethodUnsafe("", "LevelStatsView", "ShowStats", 2));
    INSTALL_HOOK_OFFSETLESS(Reset, il2cpp_utils::FindMethodUnsafe("", "PlatformLeaderboardViewController", "SetData", 1));
}