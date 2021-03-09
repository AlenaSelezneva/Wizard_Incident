#include "DialogManager.h"

DialogManager::DialogManager()
{
	/*mainDialogs = std::map<Actor::Type, std::list<DialogNode*> >;
	defaultDialogs = std::map<Actor::Type, DialogNode>();*/
	mainDialogs = {};
	defaultDialogs = {};
}

DialogNode* DialogManager::getDialog(Actor::Type npc)
{
	if (mainDialogs.count(npc) && mainDialogs[npc].size() > 0) {
		auto dialog = mainDialogs[npc].front();
		mainDialogs[npc].pop_front();
		return dialog;
	}
	else if (defaultDialogs.count(npc)) {
		return new DialogNode(defaultDialogs[npc].getType(), defaultDialogs[npc].getText());
	}
	else {
		return new DialogNode(DialogNode::Type::Message, "");
	}
}
