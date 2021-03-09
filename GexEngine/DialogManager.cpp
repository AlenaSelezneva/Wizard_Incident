#include "DialogManager.h"

DialogManager::DialogManager()
{
	/*mainDialogs = std::map<Actor::Type, std::list<DialogNode*> >;
	defaultDialogs = std::map<Actor::Type, DialogNode>();*/
	mainDialogs = {};
	defaultDialogs = {};

	mainDialogs[Actor::Type::Archmage] = std::list<DialogNode*>();
	mainDialogs[Actor::Type::Archmage].push_back(new DialogNode(DialogNode::Type::Message, "Main Dialog"));

	mainDialogs[Actor::Type::Archmage].push_back(new DialogNode(DialogNode::Type::Message, "Second Dialog sdfsd fsd f"));

	defaultDialogs[Actor::Type::Archmage] = DialogNode(DialogNode::Type::Message, "Default Greetings!");
}

DialogNode* DialogManager::getDialog(Actor::Type npc)
{
	if (mainDialogs.count(npc) > 0 && mainDialogs[npc].size() > 0) {
		auto dialog = mainDialogs[npc].front();
		//mainDialogs[npc].pop_front();
		return dialog;
	}
	else if (defaultDialogs.count(npc)) {
		return new DialogNode(defaultDialogs[npc].getType(), defaultDialogs[npc].getText());
	}
	else {
		return new DialogNode(DialogNode::Type::Message, "");
	}
}

void DialogManager::removeFirstDialog(Actor::Type npc)
{
	if (mainDialogs.count(npc) > 0 && mainDialogs[npc].size() > 0) {
		mainDialogs[npc].pop_front();
	}
}
