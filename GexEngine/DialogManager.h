#include <map>
#include <vector>

#include "DialogNode.h"
#include "Actor.h"

#pragma once
class DialogManager
{
public:
	DialogManager();

	DialogNode* getDialog(Actor::Type);
	void		removeFirstDialog(Actor::Type);

private:
	void		buildWelcomeDialog();

private:
	std::map<Actor::Type, std::list<DialogNode*>> mainDialogs;
	std::map<Actor::Type, DialogNode> defaultDialogs;

};

