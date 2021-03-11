#include "ObjectWithQuest.h"
#include "DialogNode.h"

#pragma once
class QuestNode
{
public:
	QuestNode();
	QuestNode(ObjectWithQuest::Type t, DialogNode* dialog, bool isRepeat);

	ObjectWithQuest::Type	getObjectType() const;
	void					setObjectType(ObjectWithQuest::Type t);
				
	DialogNode*				getQuestDialog() const;
	void					setQuestDialog(DialogNode* dialog);

	bool					isRepeated() const;
	void					setRepeated(bool b);

private:
	ObjectWithQuest::Type			type;
	DialogNode*						questDialog;

	bool							isRepeated_;
};

