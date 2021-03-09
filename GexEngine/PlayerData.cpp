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

void PlayerData::setCurrentDialog(DialogNode* d)
{
	currentDialog = d;
}

void PlayerData::setCurrentDialog(Actor::Type npc)
{
	currentDialog = dialogManager->getDialog(npc);
}
