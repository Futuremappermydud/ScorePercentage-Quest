#include "../include/mod_interface.hpp"

// Any specific types you would want here

#include <unordered_set>  // specific types that you might want here

// Any beatsaber-hook specific includes here

#include "../extern/beatsaber-hook/shared/utils/utils.h"
#include "../extern/beatsaber-hook/shared/utils/logging.hpp"
// For displaying modloader information (ex: Modloader.getInfo().name)
#include "../extern/modloader/shared/modloader.hpp"
// For using il2cpp_utils:: methods
#include "../extern/beatsaber-hook/shared/utils/il2cpp-utils.hpp" 
// For using il2cpp_functions:: methods
#include "../extern/beatsaber-hook/shared/utils/il2cpp-functions.hpp"
// For using commonly used types (such as Vector2, Vector3, Color, Scene, etc.)
#include "../extern/beatsaber-hook/shared/utils/typedefs.h"
// For using configuration
#include "../extern/beatsaber-hook/shared/config/config-utils.hpp"
#include "../extern/codegen/include/GlobalNamespace/ResultsViewController.hpp"
#include "../extern/codegen/include/GlobalNamespace/StandardLevelDetailView.hpp"
#include "../extern/codegen/include/GlobalNamespace/PlayerLevelStatsData.hpp"
#include "../extern/codegen/include/System/Math.hpp"
#include "../extern/codegen/include/GlobalNamespace/ScoreFormatter.hpp"
#include "../extern/codegen/include/UnityEngine/GameObject.hpp"
#include "../extern/codegen/include/TMPro/TextMeshProUGUI.hpp"
#include "../extern/codegen/include/GlobalNamespace/LevelCompletionResults.hpp"
#include "../extern/codegen/include/GlobalNamespace/PlayerData.hpp"
#include "../extern/codegen/include/GlobalNamespace/IDifficultyBeatmap.hpp"
#include "../extern/codegen/include/GlobalNamespace/IDifficultyBeatmapSet.hpp"
#include "../extern/codegen/include/GlobalNamespace/IBeatmapLevel.hpp"
#include "../extern/codegen/include/GlobalNamespace/BeatmapData.hpp"
#include "../extern/codegen/include/GlobalNamespace/GameplayModifiers.hpp"

static ModInfo modInfo;

#include <iomanip>
#include <sstream>

using namespace GlobalNamespace;
using namespace TMPro;

static const Logger& getLogger() {
    static const Logger logger(modInfo);
    return logger;
}

Configuration& getConfig() {
  static Configuration configuration(modInfo);
  return configuration;
}

static struct Config_t {
    bool EnableMenuHighscore = true;
    bool EnableLevelEndRank = true;
    bool EnableAvarageCutScore = true;
    bool EnableScoreDifference = true;
    bool EnableScorePercentageDifference = true;
} Config;

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

MAKE_HOOK_OFFSETLESS(Menu, void, StandardLevelDetailView* self) {
    Menu(self);
    if (self->playerStatsContainer)
    {
        if (self->showPlayerStats && self->playerData != nullptr)
         {
                    self->playerStatsContainer->SetActive(true);
                    PlayerLevelStatsData* playerLevelStatsData = self->playerData->GetPlayerLevelStatsData(self->level->get_levelID(), self->selectedDifficultyBeatmap->get_difficulty(), self->selectedDifficultyBeatmap->get_parentDifficultyBeatmapSet()->get_beatmapCharacteristic());
                    currentScore = playerLevelStatsData->highScore;
                    if (playerLevelStatsData->validScore)
                    {
                        //calculate maximum possilble score
                        int currentDifficultyMaxScore = calculateMaxScore(self->selectedDifficultyBeatmap->get_beatmapData()->notesCount);
                        //calculate actual score percentage
                        double currentDifficultyPercentageScore = calculatePercentage(currentDifficultyMaxScore, playerLevelStatsData->highScore);
                        currentPercentage = currentDifficultyPercentageScore;
                        //add percentage to highScoreText if it isn't disabled
                        if (getConfig().config["Enable Menu Highscore Percentage"].GetBool())
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
                
                // self->playerStatsContainer.SetActive(false);
            }
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
        maxScore = calculateMaxScore(self->difficultyBeatmap->get_beatmapData()->notesCount);
        //use modifiedScore with negative multipliers

        GameplayModifiers* mods = self->levelCompletionResults->gameplayModifiers;

        if (mods->get_noFail() || mods->get_noObstacles() || mods->noArrows || mods->noBombs)
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
        if (getConfig().config["Enable Level End Rank Display"].GetBool())
        {
            //Set Percentage to first line
            rankTextLine1 = "<line-height=30%><size=60%>" + Round(resultPercentage) + "<size=45%>%";
            // Add Average Cut Score to 2nd Line if enabled
            if (getConfig().config["Enable Avarage Cut Score"].GetBool() && !getConfig().config["Enable Score Percentage Difference"].GetBool())
            {
                int averageCutScore = self->levelCompletionResults->averageCutScore;
                rankTextLine2 = "\n<size=40%>" + std::to_string(averageCutScore) + "<size=30%> / <size=0%>115";

            }
            // Add Percent Difference to 2nd Line if enabled and previous Score exists
            else if (getConfig().config["Enable Score Percentage Difference"].GetBool() && currentPercentage != 0)
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
        if (getConfig().config["Enable Score Difference"].GetBool())
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

void Reset() {
    getConfig().config.RemoveAllMembers();
    getConfig().config.SetObject();
    rapidjson::Document::AllocatorType& allocator = getConfig().config.GetAllocator();
    getConfig().config.AddMember("Enable Avarage Cut Score", Config.EnableAvarageCutScore, allocator);
    getConfig().config.AddMember("Enable Level End Rank Display", Config.EnableLevelEndRank, allocator);
    getConfig().config.AddMember("Enable Menu Highscore Percentage", Config.EnableMenuHighscore, allocator);
    getConfig().config.AddMember("Enable Score Difference", Config.EnableScoreDifference, allocator);
    getConfig().config.AddMember("Enable Score Percentage Difference", Config.EnableScorePercentageDifference, allocator);
    getConfig().Write();
}   




extern "C" void setup(ModInfo& info) {
    info.id = "ScorePercentage";
    info.version = "0.1.0";
    modInfo = info;
    
    getConfig().Load();
    if (!getConfig().config.HasMember("Enable Avarage Cut Score")) {
		Reset();
	}
}
extern "C" void load() {
    il2cpp_functions::Init();
    INSTALL_HOOK_OFFSETLESS(Results, il2cpp_utils::FindMethod("", "ResultsViewController", "SetDataToUI"));
    INSTALL_HOOK_OFFSETLESS(Menu, il2cpp_utils::FindMethod("", "StandardLevelDetailView", "RefreshContent"));
}