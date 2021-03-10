#pragma once
#include <map>
#include <list>

#include "ObjectWithQuest.h"
#include "DialogNode.h"
#include "Quest.h"

class QuestManager
{
public:
	QuestManager();

	DialogNode* getQuestDialog(ObjectWithQuest::Type);
	void		moveToNextStep(ObjectWithQuest::Type);

private:
	void		buildBookQuest();


private:
	std::vector<Quest*> currectQuests;
	std::vector<Quest*> possibleQuests;
	std::vector<Quest*> completedQuests;

	//std::map<InteractableObject::Type, std::list<DialogNode*>> mainInterations;
	//std::map<InteractableObject::Type, DialogNode> defaultInteractions;
};

