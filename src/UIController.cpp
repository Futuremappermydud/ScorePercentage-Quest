#include "../include/UI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/RectOffset.hpp"
using namespace QuestUI::BeatSaberUI;
using namespace UnityEngine::UI;
using namespace UnityEngine;

DEFINE_CLASS(ScorePercentage::UIController);

enum UITypes
{
	EnableAverageCutScore,
	EnableLevelEndRank,
	EnableMenuHighscore,
	EnableScoreDifference,
	EnableScorePercentageDifference,
};

void SetConfigValue(UITypes Type, bool Value)
{
	switch (Type)
	{
	case EnableAverageCutScore:
		getConfig().config["Enable Average Cut Score"].SetBool(Value);
		break;
	case EnableLevelEndRank:
		getConfig().config["Enable Level End Rank Display"].SetBool(Value);
		break;
	case EnableMenuHighscore:
		getConfig().config["Enable Menu Highscore Percentage"].SetBool(Value);
		break;
	case EnableScoreDifference:
		getConfig().config["Enable Score Difference"].SetBool(Value);
		break;
	case EnableScorePercentageDifference:
		getConfig().config["Enable Score Percentage Difference"].SetBool(Value);
		break;
	default:
		break;
	}
	getConfig().Write();
}

bool GetConfigValue(UITypes Type)
{
	//LoadConfig();
	switch (Type)
	{
	case EnableLevelEndRank:
		return getConfig().config["Enable Level End Rank Display"].GetBool();
		break;
	case EnableMenuHighscore:
		return getConfig().config["Enable Menu Highscore Percentage"].GetBool();
		break;
	case EnableScoreDifference:
		return getConfig().config["Enable Score Difference"].GetBool();
		break;
	case EnableScorePercentageDifference:
		return getConfig().config["Enable Score Percentage Difference"].GetBool();
		break;
	case EnableAverageCutScore:
		return getConfig().config["Enable Average Cut Score"].GetBool();
		break;
	default:
		break;
	}
	
}

std::string GetText(UITypes Type)
{
	switch (Type)
	{
	case EnableAverageCutScore:
		return "Enable Average Cut Score";
		break;
	case EnableMenuHighscore:
		return "Enable Menu Highscore Percentage";
		break;
	case EnableScorePercentageDifference:
		return "Enable Score Percentage Difference";
		break;
	case EnableScoreDifference:
		return "Enable Score Difference";
		break;
	case EnableLevelEndRank:
		return "Enable Level End Rank Override";
		break;
	default:
		return "Unknow poggers";
		break;
	}
}

void SetAverage(ScorePercentage::UIController* vc, bool value)
{
	SetConfigValue(UITypes::EnableAverageCutScore, value);
}
void SetMenu(ScorePercentage::UIController* vc, bool value)
{
	SetConfigValue(UITypes::EnableMenuHighscore, value);
}
void SetLevelEndRank(ScorePercentage::UIController* vc, bool value)
{
	SetConfigValue(UITypes::EnableLevelEndRank, value);
}
void SetScoreDifference(ScorePercentage::UIController* vc, bool value)
{
	SetConfigValue(UITypes::EnableScoreDifference, value);
}
void SetScorePercentageDifference(ScorePercentage::UIController* vc, bool value)
{
	SetConfigValue(UITypes::EnableScorePercentageDifference, value);
}

void test()
{

}

Button* CreateUIButtonLazy_2(Transform* parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Events::UnityAction* onClick, std::string buttonText){
        Button* button = CreateUIButton(parent, "PlayButton", onClick, buttonText, nullptr);
        ((RectTransform*)button->get_transform())->set_anchoredPosition(anchoredPosition);
        return button;
    }

void ScorePercentage::UIController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling){
	if(firstActivation)
	{
		VerticalLayoutGroup* layout1 = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_rectTransform());
		layout1->set_spacing(3.5f);

		QuestUI::Backgroundable* bg = layout1->get_gameObject()->AddComponent<QuestUI::Backgroundable*>();

		bg->ApplyBackground(il2cpp_utils::createcsstr("round-rect-panel"));

		UnityEngine::Color bgColor = bg->background->get_color();
		bgColor.a = 0.5;

		bg->background->set_color(bgColor);

		layout1->set_padding(UnityEngine::RectOffset::New_ctor(3, 3, 2, 2));

		ContentSizeFitter* layout1Fitter = layout1->get_gameObject()->AddComponent<ContentSizeFitter*>();
		layout1Fitter->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);
		layout1Fitter->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);

		auto Average = CreateToggle(layout1->get_transform(), GetText(UITypes::EnableAverageCutScore), il2cpp_utils::MakeAction<UnityEngine::Events::UnityAction_1<bool>>(il2cpp_functions::class_get_type(classof(UnityEngine::Events::UnityAction_1<bool>*)), this, +[](ScorePercentage::UIController* view, bool toggle) { SetAverage(view, toggle); }));
		Average->GetComponent<UI::Toggle*>()->set_isOn(GetConfigValue(UITypes::EnableAverageCutScore));

	}
}	