// Any specific types you would want here

#include <unordered_set>  // specific types that you might want here

// Any beatsaber-hook specific includes here

#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/logging.hpp"
// For displaying modloader information (ex: Modloader.getInfo().name)
#include "modloader/shared/modloader.hpp"
// For using il2cpp_utils:: methods
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp" 
// For using il2cpp_functions:: methods
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
// For using commonly used types (such as Vector2, Vector3, Color, Scene, etc.)
#include "beatsaber-hook/shared/utils/typedefs.h"
// For using configuration
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "GlobalNamespace/ResultsViewController.hpp"
#include "GlobalNamespace/StandardLevelDetailView.hpp"
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
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

Configuration& getConfig();
const Logger& getLogger();