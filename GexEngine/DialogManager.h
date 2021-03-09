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


private:
	std::map<Actor::Type, std::list<DialogNode*>> mainDialogs;
	std::map<Actor::Type, DialogNode> defaultDialogs;

};

