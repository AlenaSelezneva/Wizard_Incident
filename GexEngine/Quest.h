
#include "QuestNode.h"
#include "DialogNode.h"
#include "ObjectWithQuest.h"

#include <vector>

#pragma once
class Quest
{
public:
	Quest(size_t newId, std::string name);

	void			addQuestNodeBunch(std::vector<QuestNode> nodes, std::string instruction);
	DialogNode*		getQuestDialog(ObjectWithQuest::Type obj);

	void			moveToNextQuestStep();
	bool			isCompleted() const;

	size_t			getId() const;

	void			onDialogShowed();

	std::string		getQuestDisplayLine() const;

private:
	size_t								id;

	std::string							questName;

	std::vector<std::vector<QuestNode > >	questNodes;
	std::vector<std::string>				instructions;
	size_t								questNodeIndex;
	bool								isCompleted_;

	int									lastRestrievedNodeIndex;

};

