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
		layout->set_spacing(1.3f);

		ContentSizeFitter* Fitter = layout->get_gameObject()->AddComponent<ContentSizeFitter*>();
		Fitter->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);
		Fitter->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);
		Transform* transform = layout->get_transform();

		VerticalLayoutGroup* layout2 = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(transform);
		layout2->get_gameObject()->AddComponent<QuestUI::Backgroundable*>()->ApplyBackground(il2cpp_utils::createcsstr("round-rect-panel"));
		layout2->set_spacing(1.2f);
		layout2->set_padding(UnityEngine::RectOffset::New_ctor(3, 3, 2, 2));

		ContentSizeFitter* contentSizeFitter = layout2->get_gameObject()->AddComponent<ContentSizeFitter*>();
		contentSizeFitter->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);
		contentSizeFitter->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);
		Transform* transform2 = layout2->get_transform();
		AddConfigValueToggle(transform2, getCustomConfig().MenuHighscore);
		AddConfigValueToggle(transform2, getCustomConfig().LevelEndRank);
		AddConfigValueToggle(transform2, getCustomConfig().AverageCutScore);
		AddConfigValueToggle(transform2, getCustomConfig().ScoreDifference);
		AddConfigValueToggle(transform2, getCustomConfig().ScorePercentageDifference);

        VerticalLayoutGroup* layout3 = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(transform);
		layout3->get_gameObject()->AddComponent<QuestUI::Backgroundable*>()->ApplyBackground(il2cpp_utils::createcsstr("round-rect-panel"));
		layout3->set_spacing(2);
		layout3->set_padding(UnityEngine::RectOffset::New_ctor(3, 3, 2, 2));

		ContentSizeFitter* contentSizeFitter2 = layout3->get_gameObject()->AddComponent<ContentSizeFitter*>();
		contentSizeFitter2->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);
		contentSizeFitter2->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);
		Transform* transform3 = layout3->get_transform();
        QuestUI::BeatSaberUI::CreateText(transform3, "<b>Config Info</b>\n1. <i>Settings are applied on change.</i>\n2. <i>When toggling Menu Highscore, clicking the same song will not show the\nchanges until another song is pressed.</i>\n3. <i>Average Cut Score and Score Percentage Difference are <u>exclusive</u> of each other.\nSo if you enable both, <u>percentage will take priority</u>.</i>\n", false);
	}
}