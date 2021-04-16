#include "DialogManager.h"
#include "DialogMessage.h"
#include "DialogAnswer.h"

DialogManager::DialogManager()
{
	mainDialogs = {};
	defaultDialogs = {};

	startFightDialogs = {};
	endFightDialogs = {};

	buildWelcomeDialog();
	buildFightDialogs();
}

DialogNode* DialogManager::getDialog(Actor::Type npc)
{
	isStartFightDialog_ = false;

	if (mainDialogs.count(npc) > 0 && mainDialogs[npc].size() > 0) {
		auto dialog = mainDialogs[npc].front();
		return dialog;
	}

	if (startFightDialogs.count(npc) > 0) {
		isStartFightDialog_ = true;
		return startFightDialogs[npc];
	}
	
	if (defaultDialogs.count(npc)) {
		return new DialogNode(defaultDialogs[npc].getType(), defaultDialogs[npc].getText());
	}
	
	return new DialogNode(DialogNode::Type::Message, "");
}

DialogNode* DialogManager::getEndFightDialog(Actor::Type npc)
{
	return endFightDialogs[npc];
}

void DialogManager::onCurrentDialogComplete(Actor::Type npc)
{
	if (mainDialogs.count(npc) > 0 && mainDialogs[npc].size() > 0) {
		mainDialogs[npc].pop_front();
	}

	isStartFightDialog_ = false;

	/*if (isStartFightDialog_) {
		isStartFightDialog_ = false;
	}

	if (!isStartFightDialog_ && startFightDialogs.count(npc) > 0) {
		isStartFightDialog_ = true;
	}*/
}

bool DialogManager::isStartFight()
{
	return isStartFightDialog_;
}

void DialogManager::buildWelcomeDialog()
{
	//mainDialogs[Actor::Type::Archmage] = std::list<DialogNode*>();

	defaultDialogs[Actor::Type::Archmage] = DialogMessage("Run along, I am busy...");
	defaultDialogs[Actor::Type::Gazan] = DialogMessage("Morning! Nice day for reading, isn\'t it?");
	defaultDialogs[Actor::Type::Elony] = DialogMessage("I\'m so glad to see you again!");
	defaultDialogs[Actor::Type::MysteriousFigure] = DialogMessage("What? We are supposed to be fighting!");
}

void DialogManager::buildFightDialogs()
{
	DialogNode* fightProposal = new DialogMessage("Do you want to have a training fight with me?");
	fightProposal->attachChild(new DialogAnswer("Yes, let\'s fight", true));
	fightProposal->attachChild(new DialogAnswer("I should go"));

	startFightDialogs[Actor::Type::MysteriousFigure] = fightProposal;

	DialogNode* endFight = new DialogMessage("It was a good fight!");
	endFightDialogs[Actor::Type::MysteriousFigure] = endFight;
}
