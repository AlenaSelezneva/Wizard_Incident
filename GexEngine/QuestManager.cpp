#include "QuestManager.h"
#include "DialogMessage.h"
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

	/*for (int i = 0; i < currectQuests.size(); ++i) {
		if (currectQuests[i]->getObjectType() == type) {
			currectQuests[i]->complete();
			auto nextStep = currectQuests[i]->getNextStep();

			if (nextStep == nullptr) {
				// Work on this later!!

				//completedQuests.push_back();
				currectQuests[i] = nullptr;
			}
			else {
				currectQuests[i] = nextStep;
			}
		}
	}

	for (int i = 0; i < possibleQuests.size(); ++i) {
		if (possibleQuests[i]->getObjectType() == type) {
			possibleQuests[i]->complete();

			auto nextStep = possibleQuests[i]->getNextStep();

			if (nextStep != nullptr) {
				currectQuests.push_back(nextStep);
			}
			else {
				completedQuests.push_back(possibleQuests[i]);
			}
			//possibleQuests[i] = nullptr;
			possibleQuests.erase(possibleQuests.begin() + i, possibleQuests.begin() + i + 1);
		}

	}*/
}

void QuestManager::buildBookQuest()
{
	auto questNode = QuestNode(ObjectWithQuest::Type::BookshelfQuest, new DialogMessage("You found a Spellbook"), false);
	auto questNode2 = QuestNode(ObjectWithQuest::Type::BookshelfNotQuest, new DialogMessage("Not here"), true);

	std::vector<QuestNode> nodes{ questNode, questNode2 };

	auto quest = new Quest(++newId);
	quest->addQuestNodeBunch(nodes);

	possibleQuests.push_back(quest);
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