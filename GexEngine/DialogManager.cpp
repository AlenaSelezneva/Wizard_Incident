#include "DialogManager.h"
#include "DialogMessage.h"
#include "DialogAnswer.h"

DialogManager::DialogManager()
{
	/*mainDialogs = std::map<Actor::Type, std::list<DialogNode*> >;
	defaultDialogs = std::map<Actor::Type, DialogNode>();*/
	mainDialogs = {};
	defaultDialogs = {};

	//mainInterations = {};
	//defaultInteractions = {};

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

	DialogNode* weldomeSpeach = new DialogMessage(
		"Welcome to the Wizard Tower, School of Magic. You are one of the few who is able to \nenter here and who has the potential to master the power of Magic and use it in your \nown interests.");

	mainDialogs[Actor::Type::Archmage].push_back(weldomeSpeach);

	weldomeSpeach->attachChild(new DialogMessage(
		"We, older mages of this Tower, will teach you to use the Magic for the sake of goodness. \nDark Magic is ruthless, and it is as dangerous to you as it is to those around."));

	weldomeSpeach = weldomeSpeach->getChildren()->at(0);

	weldomeSpeach->attachChild(new DialogAnswer("I am so glad to be here! Can’t \nwait to get to learn all of this."));
	weldomeSpeach->attachChild(new DialogAnswer("Thank you, it is a great honor to \nget here. I need to \nrestrain the Magic so \nthat it only brings good to everyone. "));
	weldomeSpeach->attachChild(new DialogAnswer("You are lucky to get such a talented \nstudent as I am. I will master my \npowers quickly."));

	DialogNode* nextStep = new DialogMessage(
		"You have a long way ahead, my pointy-eared friend. Magic requires a lot of effort, patience, \nand a bit of artfulness. This place was created to help the like of you to develop your potential. \nThe magisters will teach you a variety of different spells. You will learn quicker when cooperating \nwith other students.");

	weldomeSpeach->getChildren()->at(0)->attachChild(nextStep);
	weldomeSpeach->getChildren()->at(1)->attachChild(nextStep);
	weldomeSpeach->getChildren()->at(2)->attachChild(nextStep);

	weldomeSpeach = nextStep;

	weldomeSpeach->attachChild(new DialogAnswer("I will be a diligent student. I will \npractice and absorb all the knowledge I \ncan get! "));
	weldomeSpeach->attachChild(new DialogAnswer("I do hope to find people here who will \nhelp me develop and improve. "));
	weldomeSpeach->attachChild(new DialogAnswer("Thanks, but I feel a talent in me. You \nwill have no problems with \nme being here."));


	/*mainDialogs[Actor::Type::Archmage] = std::list<DialogNode*>();
	mainDialogs[Actor::Type::Archmage].push_back(new DialogMessage(
		"Welcome to the Wizard Tower, School of Magic. You are one of the few who is able to enter here and who has the potential to master the power of Magic and use it in your own interests."));



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

	mainDialogs[Actor::Type::Archmage].push_back(secondDialog);*/

	defaultDialogs[Actor::Type::Archmage] = DialogMessage("Run along, I am busy...");
}
