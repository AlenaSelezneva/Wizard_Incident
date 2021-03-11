
#include "QuestNode.h"
#include "DialogNode.h"
#include "ObjectWithQuest.h"

#include <vector>

#pragma once
class Quest
{
public:
	Quest(size_t newId);

	void			addQuestNodeBunch(std::vector<QuestNode> nodes);
	DialogNode*		getQuestDialog(ObjectWithQuest::Type obj);

	void			moveToNextQuestStep();
	bool			isCompleted() const;

	size_t			getId() const;

	void			onDialogShowed();

private:
	size_t								id;

	std::vector<std::vector<QuestNode > >	questNodes;
	size_t								questNodeIndex;
	bool								isCompleted_;

	int									lastRestrievedNodeIndex;

};

