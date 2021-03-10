#include "DialogManager.h"
#include "DialogMessage.h"
#include "DialogAnswer.h"

DialogManager::DialogManager()
{
	/*mainDialogs = std::map<Actor::Type, std::list<DialogNode*> >;
	defaultDialogs = std::map<Actor::Type, DialogNode>();*/
	mainDialogs = {};
	defaultDialogs = {};

	buildWelcomeDialog();
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

void DialogManager::buildWelcomeDialog()
{
	mainDialogs[Actor::Type::Archmage] = std::list<DialogNode*>();
	mainDialogs[Actor::Type::Archmage].push_back(new DialogMessage("Main Dialog"));

	mainDialogs[Actor::Type::Archmage].front()->attachChild(new DialogAnswer("Ansswer 1"));
	mainDialogs[Actor::Type::Archmage].front()->attachChild(new DialogAnswer("Ansswer 2"));
	mainDialogs[Actor::Type::Archmage].front()->attachChild(new DialogAnswer("Ansswer 3"));

	auto secondPartOfDialog = new DialogMessage("Continue Talking...");

	mainDialogs[Actor::Type::Archmage].front()->getChildren()->at(0)->attachChild(secondPartOfDialog);
	mainDialogs[Actor::Type::Archmage].front()->getChildren()->at(1)->attachChild(secondPartOfDialog);
	mainDialogs[Actor::Type::Archmage].front()->getChildren()->at(2)->attachChild(secondPartOfDialog);



	auto secondDialog = new DialogMessage("Second Dialog sdfsd fsd f");
	secondDialog->attachChild(new DialogMessage("Second Dialog Continue Talking"));
	secondDialog->getChildren()->at(0)->attachChild(new DialogMessage("Second Dialog Final Message"));

	mainDialogs[Actor::Type::Archmage].push_back(secondDialog);

	defaultDialogs[Actor::Type::Archmage] = DialogMessage("Default Greetings!");
}
