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
	mainDialogs[Actor::Type::Archmage] = std::list<DialogNode*>();

	DialogNode* weldomeSpeach = new DialogMessage(
		"Welcome to the Wizard Tower, School of Magic. You are one of the few who is able to \nenter here and who has the potential to master the power of Magic and use it in your \nown interests.");

	mainDialogs[Actor::Type::Archmage].push_back(weldomeSpeach);

	weldomeSpeach->attachChild(new DialogMessage(
		"We, older mages of this Tower, will teach you to use the Magic for the sake of goodness. \nDark Magic is ruthless, and it is as dangerous to you as it is to those around."));

	weldomeSpeach = weldomeSpeach->getChildren()->at(0);

	weldomeSpeach->attachChild(new DialogAnswer("I am so glad to be here! Can\'t \nwait to get to learn all of this.", Attribute::Intelligence));
	weldomeSpeach->attachChild(new DialogAnswer("Thank you, it is a great honor to \nget here. I need to restrain the \nMagic so that it only brings good to everyone. ", Attribute::Wisdom));
	weldomeSpeach->attachChild(new DialogAnswer("You are lucky to get such a talented \nstudent as I am. I will master my \npowers quickly.", Attribute::Arrogance));

	DialogNode* nextStep = new DialogMessage(
		"You have a long way ahead, my pointy-eared friend. Magic requires a lot of effort, \npatience, and a bit of artfulness. This place was created to help the \nlike of you to develop your potential. The magisters will teach you \na variety of different spells. You will learn quicker when cooperating \nwith other students.");

	weldomeSpeach->getChildren()->at(0)->attachChild(nextStep);
	weldomeSpeach->getChildren()->at(1)->attachChild(nextStep);
	weldomeSpeach->getChildren()->at(2)->attachChild(nextStep);

	weldomeSpeach = nextStep;

	weldomeSpeach->attachChild(new DialogAnswer("I will be a diligent student. I will practice and \nabsorb all the knowledge I can get! ", Attribute::Intelligence));
	weldomeSpeach->attachChild(new DialogAnswer("I do hope to find people here who will help me \ndevelop and improve. ", Attribute::Wisdom));
	weldomeSpeach->attachChild(new DialogAnswer("Thanks, but I feel a talent in me. You will \nhave no problems with me being here.", Attribute::Arrogance));

	nextStep = new DialogMessage(
		"Anyways, now you will learn your first spell. Every wizard needs to know how to \ndefend themselves, and you can never know where the danger may be. Even within the \nwalls of this school, you may need to be able to stand up for yourself.");

	weldomeSpeach->getChildren()->at(0)->attachChild(nextStep);
	weldomeSpeach->getChildren()->at(1)->attachChild(nextStep);
	weldomeSpeach->getChildren()->at(2)->attachChild(nextStep);

	weldomeSpeach = nextStep;

	weldomeSpeach->attachChild(new DialogMessage(
		"I will teach you to cast a Shield Spell. It will also serve as a test for your \nabilities, as we can\'t teach serious Magic to a person unable to cast \nthis simple spell."));

	weldomeSpeach = weldomeSpeach->getChildren()->at(0);

	weldomeSpeach->attachChild(new DialogAnswer("Can\'t wait to learn the Shield Spell! I am ready to \nbe tested and start my way to knowledge.", Attribute::Intelligence));
	weldomeSpeach->attachChild(new DialogAnswer("This sounds reasonable. Who knows where they might \nencounter something dangerous? \nIt is better to be prepared.", Attribute::Wisdom));
	weldomeSpeach->attachChild(new DialogAnswer("Will I really have to prove my abilities to you? \nCan\'t you see my obvious talent?", Attribute::Arrogance));

	auto difOptiong = weldomeSpeach->getChildren()->at(2);
	difOptiong->attachChild(new DialogMessage( "I am sorry, but it is absolutely necessary."));
	difOptiong = difOptiong->getChildren()->at(0);
	difOptiong->attachChild(new DialogAnswer("Alright, the sooner we start, the sooner it is done."));

	nextStep = new DialogMessage("Let\'s start");

	weldomeSpeach->getChildren()->at(0)->attachChild(nextStep);
	weldomeSpeach->getChildren()->at(1)->attachChild(nextStep);
	difOptiong->getChildren()->at(0)->attachChild(nextStep);

	defaultDialogs[Actor::Type::Archmage] = DialogMessage("Run along, I am busy...");
	defaultDialogs[Actor::Type::Gazan] = DialogMessage("Morning! Nice day for reading, isn\'t it?");
}

void DialogManager::buildFightDialogs()
{
	DialogNode* fightProposal = new DialogMessage("Do you want to have a training fight with me?");
	fightProposal->attachChild(new DialogAnswer("Yes, let\'s fight", true));
	fightProposal->attachChild(new DialogAnswer("Some other time"));

	startFightDialogs[Actor::Type::MysteriousFigure] = fightProposal;

	DialogNode* endFight = new DialogMessage("It was a good fight!");
	endFightDialogs[Actor::Type::MysteriousFigure] = endFight;
}
