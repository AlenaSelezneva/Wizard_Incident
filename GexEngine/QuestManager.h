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
	void		onCurrentQuestDialogComplete(bool agree = true);

	std::string	getAllQuestInstructions() const;

private:
	void		buildBookQuest();
	Quest*		getQuestById(int id);

	void		moveToCompleted(Quest* quest);
	void		removeQuestFromArray(std::vector<Quest*>* arr, int questId);


private:
	std::vector<Quest*> currectQuests;
	std::vector<Quest*> possibleQuests;
	std::vector<Quest*> completedQuests;
	std::vector<Quest*> refusedQuests;

	size_t				newId;

	int					lastRetrievedQuestId;

	//std::map<InteractableObject::Type, std::list<DialogNode*>> mainInterations;
	//std::map<InteractableObject::Type, DialogNode> defaultInteractions;
};

