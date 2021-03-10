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
	currentTalkingNPC = npc;
}

void PlayerData::onCurrentDialogComplete()
{
	dialogManager->removeFirstDialog(currentTalkingNPC);
}

void PlayerData::setCurrentQuestDialog(ObjectWithQuest::Type obj)
{
	if (obj == ObjectWithQuest::Type::None) {
		currentDialog = nullptr;
		return;
	}


	currentDialog = questManager->getQuestDialog(obj);
	currentQuestObject = obj;
}

void PlayerData::onCurrectQuestNextStep()
{
	questManager->moveToNextStep(currentQuestObject);
}

bool PlayerData::hasPendingQuest(ObjectWithQuest::Type obj)
{
	return questManager->getQuestDialog(obj) != nullptr;
}
