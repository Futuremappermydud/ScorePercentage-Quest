#include "UIController.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "UnityEngine/RectOffset.hpp"

#include "customconfig.hpp"

#include "questui/shared/BeatSaberUI.hpp"

#define AddConfigToggle(parent, value) CreateToggle(parent, value.GetName(), il2cpp_utils::MakeAction<UnityEngine::Events::UnityAction_1<bool>>(il2cpp_functions::class_get_type(classof(UnityEngine::Events::UnityAction_1<bool>*)), (void*)nullptr, +[](bool toggle) { value.SetValue(toggle); }))->GetComponent<UI::Toggle*>()->set_isOn(value.GetValue());

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
		AddConfigToggle(transform, Config.MenuHighscore);
		AddConfigToggle(transform, Config.LevelEndRank);
		AddConfigToggle(transform, Config.AverageCutScore);
		AddConfigToggle(transform, Config.ScoreDifference);
		AddConfigToggle(transform, Config.ScorePercentageDifference);
	}
}