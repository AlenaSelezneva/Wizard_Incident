#include "QuestManager.h"
#include "DialogMessage.h"
#include "DialogAnswer.h"
#include "QuestNode.h"

#include <algorithm>

QuestManager::QuestManager()
{
	currectQuests = {};
	possibleQuests = {};
	completedQuests = {};

	newId = 0;
	lastRetrievedQuestId = -1;

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

void QuestManager::onCurrentQuestDialogComplete()
{
	Quest* quest = getQuestById(lastRetrievedQuestId);

	if (quest == nullptr)
		return;

	quest->onDialogShowed();

	if (quest->isCompleted())
		moveToCompleted(quest);
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

	std::remove_if(currectQuests.begin(), currectQuests.end(), [quest](Quest* q) {
		return q->getId() == quest->getId();
		});

	std::remove_if(possibleQuests.begin(), possibleQuests.end(), [quest](Quest* q) {
		return q->getId() == quest->getId();
	});
}

void QuestManager::buildBookQuest()
{
	auto bookQuest = new Quest(++newId);

	DialogNode* gazanRequest = new DialogMessage( "I am looking for the Monstrous Bestiary, could you help me find it please?");
	gazanRequest->attachChild(new DialogAnswer("I will see what I can do."));

	auto initialNode = QuestNode(ObjectWithQuest::Type::Gazan, gazanRequest, false);

	bookQuest->addQuestNodeBunch({ initialNode });

	auto questNodeBook = QuestNode(ObjectWithQuest::Type::BookshelfQuest, new DialogMessage("You found the Monstrous Bestiary"), false);
	auto questNodeBook2 = QuestNode(ObjectWithQuest::Type::BookshelfNotQuest, new DialogMessage("The book you are looking for is not here"), true);
	auto gazanWaitingNode = QuestNode(ObjectWithQuest::Type::Gazan, new DialogMessage("Have you found it yet?"), true);

	std::vector<QuestNode> nodes{ questNodeBook, questNodeBook2, gazanWaitingNode };

	bookQuest->addQuestNodeBunch(nodes);

	gazanRequest = new DialogMessage("Great, that is the one!");
	gazanRequest->attachChild(new DialogMessage("Could you deliver it to the Archmage now?"));
	gazanRequest->getChildren()->at(0)->attachChild(new DialogAnswer("Alright, I will do that."));

	/*auto sentToArchmage = QuestNode(ObjectWithQuest::Type::Gazan, gazanRequest, false);*/

	bookQuest->addQuestNodeBunch({ QuestNode(ObjectWithQuest::Type::Gazan, gazanRequest, false) });

	bookQuest->addQuestNodeBunch({ QuestNode(ObjectWithQuest::Type::Archmage, new DialogMessage("Thank you, I have been waiting for this book for half a day now."), false) });

	possibleQuests.push_back(bookQuest);
}