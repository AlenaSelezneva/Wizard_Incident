#include <string>

#include "DialogNode.h"
#include "DialogManager.h"

#pragma once
class PlayerData
{
public:
	PlayerData();

	DialogNode*				getCurrentDialog() const;
	void					setCurrentDialog(Actor::Type npc);
	void					onCurrentDialogComplete();

private:
	DialogNode*					currentDialog;
	Actor::Type					currentTalkingNPC;
	DialogManager*				dialogManager;

};

