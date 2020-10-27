#include "UIController.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/Vector3.hpp"

#include "customconfig.hpp"

#include "questui/shared/BeatSaberUI.hpp"

using namespace QuestUI::BeatSaberUI;
using namespace UnityEngine::UI;
using namespace UnityEngine;

DEFINE_CLASS(ScorePercentage::UIController);

void ScorePercentage::UIController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling){
	if(firstActivation)
	{
		VerticalLayoutGroup* layout = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_rectTransform());
		layout->get_gameObject()->AddComponent<QuestUI::Backgroundable*>()->ApplyBackground(il2cpp_utils::createcsstr("round-rect-panel"));
		layout->set_spacing(3.5f);
		layout->set_padding(UnityEngine::RectOffset::New_ctor(3, 3, 2, 2));

		ContentSizeFitter* contentSizeFitter = layout->get_gameObject()->AddComponent<ContentSizeFitter*>();
		contentSizeFitter->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);
		contentSizeFitter->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);
		Transform* transform = layout->get_transform();
		AddConfigValueToggle(transform, getCustomConfig().MenuHighscore);
		AddConfigValueToggle(transform, getCustomConfig().LevelEndRank);
		AddConfigValueToggle(transform, getCustomConfig().AverageCutScore);
		AddConfigValueToggle(transform, getCustomConfig().ScoreDifference);
		AddConfigValueToggle(transform, getCustomConfig().ScorePercentageDifference);
	}
}