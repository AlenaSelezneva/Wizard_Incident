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
		
	currentDialog = dialogManager->getDialog(npc);

	currentQuestObject = ObjectWithQuest::Type::None;
	currentTalkingNPC = npc;
}

void PlayerData::onCurrentDialogComplete()
{
	if (currentQuestObject != ObjectWithQuest::Type::None && questManager->getQuestDialog(currentQuestObject) != nullptr)
		onCurrectQuestDialogComplete();
	else
		dialogManager->onCurrentDialogComplete(currentTalkingNPC);
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

void PlayerData::onCurrectQuestDialogComplete()
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

void PlayerData::seInFightState(bool b)
{
	isInFightState_ = b;
}

size_t PlayerData::getHeroAttribute(Attribute a)
{
	if (heroStats.count(a) > 0)
		return heroStats[a];
	return 0;
}

std::string PlayerData::getHeroAttributesString()
{
	std::string res = "Int " + std::to_string(heroStats[Attribute::Intelligence]) +
		"\nWis " + std::to_string(heroStats[Attribute::Wisdom]) +
		"\nArr " + std::to_string(heroStats[Attribute::Arrogance]);

	return res;
}
