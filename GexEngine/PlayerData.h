#include <string>

#include "DialogNode.h"
#include "DialogManager.h"

#pragma once
class PlayerData
{
public:
	PlayerData();

	/*std::string				getCurrentDialog() const;
	void					setCurrentDialog(std::string s);*/
	DialogNode*				getCurrentDialog() const;
	void					setCurrentDialog(DialogNode* s);
	void					setCurrentDialog(Actor::Type npc);

private:
	//std::string				currentDialog;
	DialogNode*					currentDialog;
	DialogManager*				dialogManager;

};

