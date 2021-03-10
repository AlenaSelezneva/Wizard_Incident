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
	void		removeFirstDialog(Actor::Type);

private:
	void		buildWelcomeDialog();

private:
	std::map<Actor::Type, std::list<DialogNode*>> mainDialogs;
	std::map<Actor::Type, DialogNode> defaultDialogs;

	std::map<InteractableObject::Type, std::list<DialogNode*>> mainInterations;
	std::map<InteractableObject::Type, DialogNode> defaultInteractions;

};

