#include "PlayerData.h"


PlayerData::PlayerData()
	//: dialogManager()
	//, currentDialog()
{
	dialogManager = new DialogManager();
	questManager = new QuestManager();

	currentQuestObject = ObjectWithQuest::Type::None;
	currentTalkingNPC = Actor::Type::None;

	currentDialog = nullptr;
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
