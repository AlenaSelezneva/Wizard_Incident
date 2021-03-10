#include "QuestManager.h"

QuestManager::QuestManager()
{
	currectQuests = {};
	possibleQuests = {};
	completedQuests = {};
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

			currectQuests.push_back(nextStep);
			possibleQuests[i] = nullptr;
		}

	}
}

void QuestManager::buildBookQuest()
{


}
