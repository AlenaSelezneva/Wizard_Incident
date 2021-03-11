#include "Quest.h"

Quest::Quest(size_t newId)
	: id(newId)
{
	questNodes = { {} };
	questNodeIndex = 0;
	isCompleted_ = false;

	lastRestrievedNodeIndex = -1;
}


void Quest::addQuestNodeBunch(std::vector<QuestNode> nodes)
{
	if (questNodes[0].size() == 0)
		questNodes[0] = nodes;
	else
		questNodes.push_back(nodes);
}

DialogNode* Quest::getQuestDialog(ObjectWithQuest::Type obj)
{
	lastRestrievedNodeIndex = -1;

	if (isCompleted_ || questNodes.size() == 0 || questNodes[questNodeIndex].size() == 0) 
		return nullptr;	

	for (int i = 0; i < questNodes[questNodeIndex].size(); ++i) {
		if (questNodes[questNodeIndex][i].getObjectType() == obj) {
			lastRestrievedNodeIndex = i;
			return questNodes[questNodeIndex][i].getQuestDialog();
		}
	}
	
	return nullptr;
}

void Quest::moveToNextQuestStep()
{
	questNodeIndex++;
	if (questNodeIndex >= questNodes.size())
		isCompleted_ = true;
}

bool Quest::isCompleted() const
{
	return isCompleted_;
}

size_t Quest::getId() const
{
	return id;
}

void Quest::onDialogShowed()
{
	if (!questNodes[questNodeIndex][lastRestrievedNodeIndex].isRepeated()) {
		moveToNextQuestStep();
	}
}
