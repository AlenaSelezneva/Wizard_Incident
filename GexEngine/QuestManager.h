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
	void		onCurrentQuestDialogComplete();

private:
	void		buildBookQuest();
	Quest*		getQuestById(int id);

	void		moveToCompleted(Quest* quest);


private:
	std::vector<Quest*> currectQuests;
	std::vector<Quest*> possibleQuests;
	std::vector<Quest*> completedQuests;

	size_t				newId;

	int					lastRetrievedQuestId;

	//std::map<InteractableObject::Type, std::list<DialogNode*>> mainInterations;
	//std::map<InteractableObject::Type, DialogNode> defaultInteractions;
};

