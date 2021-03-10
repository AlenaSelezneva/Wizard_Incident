#include "PlayerData.h"


PlayerData::PlayerData()
	//: dialogManager()
	//, currentDialog()
{
	dialogManager = new DialogManager();
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
