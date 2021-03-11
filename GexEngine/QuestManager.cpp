#include "QuestManager.h"
#include "DialogMessage.h"

QuestManager::QuestManager()
{
	currectQuests = {};
	possibleQuests = {};
	completedQuests = {};

	buildBookQuest();
}

DialogNode* QuestManager::getQuestDialog(ObjectWithQuest::Type type)
{
	for (int i = 0; i < currectQuests.size(); ++i) {
		if (currectQuests[i]->getObjectType() == type)
			return currectQuests[i]->getQuestDialog();
	}

	for (int i = 0; i < possibleQuests.size(); ++i) {
		if (possibleQuests[i]->getObjectType() == type)
			return possibleQuests[i]->getQuestDialog();
	}

	return nullptr;
}

void QuestManager::moveToNextStep(ObjectWithQuest::Type type)
{
	for (int i = 0; i < currectQuests.size(); ++i) {
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

	}
}

void QuestManager::buildBookQuest()
{
	auto quest = new Quest(ObjectWithQuest::Type::BookshelfQuest, new DialogMessage("You found a Spellbook"));

	possibleQuests.push_back(quest);

}
