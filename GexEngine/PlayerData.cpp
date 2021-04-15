#include "PlayerData.h"


PlayerData::PlayerData()
	: heroStats()
{
	dialogManager = new DialogManager();
	questManager = new QuestManager();

	currentQuestObject = ObjectWithQuest::Type::None;
	currentTalkingNPC = Actor::Type::None;

	currentDialog = nullptr;

	heroStats[Attribute::Intelligence] = 1;
	heroStats[Attribute::Wisdom] = 1;
	heroStats[Attribute::Arrogance] = 1;
}

DialogNode* PlayerData::getCurrentDialog() const
{
	return currentDialog;
}

void PlayerData::setCurrentDialog(Actor::Type npc)
{
	if (npc == Actor::Type::None) {
		currentDialog = nullptr;
		return;
	}

	if (isRequiredTalking_)
		return;
		
	currentDialog = dialogManager->getDialog(npc);

	currentQuestObject = ObjectWithQuest::Type::None;
	currentTalkingNPC = npc;
}

void PlayerData::onCurrentDialogComplete(bool agree)
{
	if (currentQuestObject != ObjectWithQuest::Type::None && questManager->getQuestDialog(currentQuestObject) != nullptr)
		onCurrectQuestDialogComplete(agree);
	else {

		if (dialogManager->isStartFight() && agree)
			isInFightState_ = true;

		dialogManager->onCurrentDialogComplete(currentTalkingNPC);
	}

	currentDialog = nullptr;

	isRequiredTalking_ = false;
	isProposingFight_ = false;
	isProposingQuest_ = false;
}

void PlayerData::setCurrentQuestDialog(ObjectWithQuest::Type obj)
{
	if (obj == ObjectWithQuest::Type::None) {
		currentDialog = nullptr;
		return;
	}

	currentDialog = questManager->getQuestDialog(obj);
	currentQuestObject = obj;
	currentTalkingNPC = Actor::Type::None;
}

void PlayerData::onCurrectQuestDialogComplete(bool agree)
{
	questManager->onCurrentQuestDialogComplete();
}

bool PlayerData::hasPendingQuest(ObjectWithQuest::Type obj)
{
	return questManager->getQuestDialog(obj) != nullptr;
}

bool PlayerData::isShowingJournal()
{
	return isShowingJournal_;
}

void PlayerData::setShowingJournal(bool b)
{
	isShowingJournal_ = b;
}

void PlayerData::flipShowingJournal()
{
	isShowingJournal_ = !isShowingJournal_;
}

std::string PlayerData::getQuestInstrunstionDisplay() const
{
	return questManager->getAllQuestInstructions();
}

bool PlayerData::isIntersectsWithPortal() const
{
	return isIntersectsWithPortal_;
}

void PlayerData::setIntersectsWithPortal(bool b)
{
	isIntersectsWithPortal_ = b;
}

bool PlayerData::isInFightState() const
{
	return isInFightState_;
}

Actor::Type PlayerData::getCurrentActor() const
{
	return currentTalkingNPC;
}

//void PlayerData::setInFightState(bool b)
//{
//	isInFightState_ = b;
//}

void PlayerData::startFight()
{
	isInFightState_ = true;
}

void PlayerData::finishFight(Actor::Type t)
{
	isInFightState_ = false;
	//isRequiredTalking_ = true;
	currentDialog = dialogManager->getEndFightDialog(t);
}

bool PlayerData::isRequiredTalking()
{
	return isRequiredTalking_;
}

size_t PlayerData::getHeroAttribute(Attribute a)
{
	if (heroStats.count(a) > 0)
		return heroStats[a];
	return 0;
}

std::string PlayerData::getHeroAttributesString()
{
	std::string res = "Int: " + std::to_string(heroStats[Attribute::Intelligence]) +
		"\nWis: " + std::to_string(heroStats[Attribute::Wisdom]) +
		"\nArr: " + std::to_string(heroStats[Attribute::Arrogance]);

	return res;
}

void PlayerData::increaseHeroAttribute(Attribute a)
{
	if (a != Attribute::None)
		heroStats[a]++;
}
