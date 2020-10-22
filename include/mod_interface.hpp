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
#include "../extern/codegen/include/GlobalNamespace/LevelStatsView.hpp"
#include "../extern/codegen/include/GlobalNamespace/PlayerData.hpp"
#include "../extern/codegen/include/GlobalNamespace/IDifficultyBeatmap.hpp"
#include "../extern/codegen/include/GlobalNamespace/IDifficultyBeatmapSet.hpp"
#include "../extern/codegen/include/GlobalNamespace/IBeatmapLevel.hpp"
#include "../extern/codegen/include/GlobalNamespace/BeatmapData.hpp"
#include "../extern/codegen/include/GlobalNamespace/GameplayModifiers.hpp"
#include "../extern/questui/shared/BeatSaberUI.hpp"
#include "../extern/questui/shared/QuestUI.hpp"

Configuration& getConfig();
static const Logger& getLogger();