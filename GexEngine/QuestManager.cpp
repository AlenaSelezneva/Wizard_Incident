#include "QuestManager.h"
#include "DialogMessage.h"
#include "DialogAnswer.h"
#include "QuestNode.h"
#include "SoundPlayer.h"

#include <algorithm>
#include "CommandQueue.h"
#include "SoundNode.h"

QuestManager::QuestManager(SoundPlayer* sounds)
	:sounds(sounds)
{
	currectQuests = {};
	possibleQuests = {};
	completedQuests = {};
	refusedQuests = {};

	newId = 0;
	lastRetrievedQuestId = -1;

	buildWelcomeMessageQuest();
	buildBookQuest();
}

DialogNode* QuestManager::getQuestDialog(ObjectWithQuest::Type type)
{
	for (int i = 0; i < currectQuests.size(); ++i) {
		if (currectQuests[i]->getQuestDialog(type) != nullptr) {
			lastRetrievedQuestId = currectQuests[i]->getId();
			return currectQuests[i]->getQuestDialog(type);
		}	
	}

	for (int i = 0; i < possibleQuests.size(); ++i) {
		if (possibleQuests[i]->getQuestDialog(type) != nullptr) {
			lastRetrievedQuestId = possibleQuests[i]->getId();
			return possibleQuests[i]->getQuestDialog(type);
		}	
	}

	return nullptr;
}

void QuestManager::onCurrentQuestDialogComplete(bool agree)
{
	Quest* quest = getQuestById(lastRetrievedQuestId);

	if (quest == nullptr)
		return;

	quest->onDialogShowed();
	

	if (lastRetrievedQuestId == 1) {
		buildMeetingQuests();
	}

	/*if (!agree) {
		refusedQuests.push_back(quest);
		removeQuestFromArray(&currectQuests, quest->getId());
		removeQuestFromArray(&possibleQuests, quest->getId());
	}*/

	if (quest->isCompleted()) {
		moveToCompleted(quest);
		playQuestSounds(EffectID::QuestCompleted);
	}
	else {
		int cnt = std::count_if(currectQuests.begin(), currectQuests.end(), [quest](Quest* q) {
				return q->getId() == quest->getId();
			});

		if (cnt == 0) {
			currectQuests.push_back(quest);
			removeQuestFromArray(&possibleQuests, quest->getId());
			playQuestSounds(EffectID::QuestAccepted);
		}
	}
}

std::string QuestManager::getAllQuestInstructions() const
{
	std::string res = "";
	for (auto quest : currectQuests) {
		res += quest->getQuestDisplayLine() + "\n\n";
	}

	return res;
}

Quest* QuestManager::getQuestById(int id)
{
	for (int i = 0; i < currectQuests.size(); ++i) {
		if (currectQuests[i]->getId() == id)
			return currectQuests[i];
	}

	for (int i = 0; i < possibleQuests.size(); ++i) {
		if (possibleQuests[i]->getId() == id)
			return possibleQuests[i];
	}

	for (int i = 0; i < completedQuests.size(); ++i) {
		if (completedQuests[i]->getId() == id)
			return completedQuests[i];
	}

	return nullptr;
}

void QuestManager::moveToCompleted(Quest* quest)
{
	completedQuests.push_back(quest);

	removeQuestFromArray(&currectQuests, quest->getId());

	removeQuestFromArray(&possibleQuests, quest->getId());
}

void QuestManager::removeQuestFromArray(std::vector<Quest*>* arr, int questId)
{
	for (int i = 0; i < arr->size(); ++i) {
		if (questId == arr->at(i)->getId()) {
			arr->erase(arr->begin() + i);
		}
	}
}

void QuestManager::buildBookQuest()
{
	auto bookQuest = new Quest(++newId, "Monstrous Bestiary");

	DialogNode* gazanRequest = new DialogMessage( "I am looking for the Monstrous Bestiary, could you help me find it please?");
	gazanRequest->attachChild(new DialogAnswer("I will see what I can do.", true));
	//gazanRequest->attachChild(new DialogAnswer("Do I look like I have \ntime for this?", false));

	auto initialNode = QuestNode(ObjectWithQuest::Type::Gazan, gazanRequest, false);

	bookQuest->addQuestNodeBunch({ initialNode }, "");

	auto questNodeBook = QuestNode(ObjectWithQuest::Type::BookshelfQuest, new DialogMessage("You found the Monstrous Bestiary"), false);
	auto questNodeBook2 = QuestNode(ObjectWithQuest::Type::BookshelfNotQuest, new DialogMessage("The book you are looking for is not here"), true);
	auto gazanWaitingNode = QuestNode(ObjectWithQuest::Type::Gazan, new DialogMessage("Have you found it yet?"), true);

	std::vector<QuestNode> nodes{ questNodeBook, questNodeBook2, gazanWaitingNode };

	bookQuest->addQuestNodeBunch(nodes, "Find the Monstrous Bestiary");

	gazanRequest = new DialogMessage("Great, that is the one!");
	gazanRequest->attachChild(new DialogMessage("Could you deliver it to the Archmage now?"));
	gazanRequest->getChildren()->at(0)->attachChild(new DialogAnswer("Alright, I will do that."));

	/*auto sentToArchmage = QuestNode(ObjectWithQuest::Type::Gazan, gazanRequest, false);*/

	bookQuest->addQuestNodeBunch({ QuestNode(ObjectWithQuest::Type::Gazan, gazanRequest, false) }, "Bring the book to Gazan");

	bookQuest->addQuestNodeBunch({ QuestNode(ObjectWithQuest::Type::Archmage, new DialogMessage("Thank you, I have been waiting for this book for half a day now."), false) }, "Deliver Monstrous Bestiary \n\t\t\tto the Archmage");

	possibleQuests.push_back(bookQuest);
}

void QuestManager::buildMeetingQuests()
{
	auto meetElonyQuest = new Quest(++newId, "Meet Elony");
	auto elonyNode = QuestNode(ObjectWithQuest::Type::Elony, new DialogMessage("I a\'m so glad we could meet!"), false);
	meetElonyQuest->addQuestNodeBunch({ elonyNode }, "find and talk \n\t\t\t\tto Elony");

	auto meetGazanQuest = new Quest(++newId, "Meet Gazan");
	auto gazanNode = QuestNode(ObjectWithQuest::Type::Gazan, new DialogMessage("A newcomer, eh?"), false);
	meetGazanQuest->addQuestNodeBunch({ gazanNode }, "find and talk \n\t\t\t\tto Gazan");

	currectQuests.push_back(meetElonyQuest);
	currectQuests.push_back(meetGazanQuest);
}

void QuestManager::buildWelcomeMessageQuest()
{
	auto welcomeQuest = new Quest(++newId, "Welcome to Tower");
	

	DialogNode* weldomeSpeach = new DialogMessage(
		"Welcome to the Wizard Tower, School of Magic. You are one of the few who is able to \nenter here and who has the potential to master the power of Magic and use it in your \nown interests.");
	auto w = weldomeSpeach;
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
	difOptiong->attachChild(new DialogMessage("I am sorry, but it is absolutely necessary."));
	difOptiong = difOptiong->getChildren()->at(0);
	difOptiong->attachChild(new DialogAnswer("Alright, the sooner we start, the sooner it is done."));

	nextStep = new DialogMessage("Let\'s start");

	weldomeSpeach->getChildren()->at(0)->attachChild(nextStep);
	weldomeSpeach->getChildren()->at(1)->attachChild(nextStep);
	difOptiong->getChildren()->at(0)->attachChild(nextStep);


	auto welcomeTale = QuestNode(ObjectWithQuest::Type::Archmage, w, false);
	welcomeQuest->addQuestNodeBunch({ welcomeTale }, "Talk to Archmage");

	auto exploreNode = QuestNode(ObjectWithQuest::Type::Dgery, new DialogMessage(""), false);
	welcomeQuest->addQuestNodeBunch({ exploreNode }, "Explore the Tower");

	currectQuests.push_back(welcomeQuest);
}

void QuestManager::playQuestSounds(EffectID effect)
{
	sounds->play(effect);
}
