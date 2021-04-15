#include <map>
#include <vector>

#include "DialogNode.h"
#include "Actor.h"
#include "InteractableObject.h"

#pragma once
class DialogManager
{
public:
	DialogManager();

	DialogNode* getDialog(Actor::Type);
	DialogNode* getEndFightDialog(Actor::Type);
	void		onCurrentDialogComplete(Actor::Type);

	bool		isStartFight();

private:
	void		buildWelcomeDialog();
	void		buildFightDialogs();

private:
	std::map<Actor::Type, std::list<DialogNode*>> mainDialogs;
	std::map<Actor::Type, DialogNode> defaultDialogs;

	std::map<Actor::Type, DialogNode*> startFightDialogs;
	std::map<Actor::Type, DialogNode*> endFightDialogs;

	bool		isStartFightDialog_;

};

