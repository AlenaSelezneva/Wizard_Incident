
#include "ObjectWithQuest.h"
#include "DialogNode.h"

#pragma once
class Quest
{
public:
	Quest();
	Quest(ObjectWithQuest::Type t, DialogNode* dialog);

	ObjectWithQuest::Type	getObjectType() const;
	void					setObjectType(ObjectWithQuest::Type t);

	DialogNode*				getQuestDialog() const;
	void					setQuestDialog(DialogNode* dialog);

	bool					isCompleted() const;
	void					complete();

	Quest*					getNextStep();
	void					setNextStep(Quest* q);

private:
	ObjectWithQuest::Type			type;
	DialogNode*						questDialog;

	Quest*							nextStep;

	bool							isCompleted_;
};

