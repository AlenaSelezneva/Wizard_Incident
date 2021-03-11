#include "QuestNode.h"


QuestNode::QuestNode()
	: type(ObjectWithQuest::Type::None)
	, questDialog()
	, isRepeated_(false)
{
}

QuestNode::QuestNode(ObjectWithQuest::Type t, DialogNode* dialog, bool isRepeat)
	: type(t)
	, questDialog(dialog)
	, isRepeated_(isRepeat)
{
}

ObjectWithQuest::Type QuestNode::getObjectType() const
{
	return type;
}

void QuestNode::setObjectType(ObjectWithQuest::Type t)
{
	type = t;
}

DialogNode* QuestNode::getQuestDialog() const
{
	return questDialog;
}

void QuestNode::setQuestDialog(DialogNode* dialog)
{
	questDialog = dialog;
}

bool QuestNode::isRepeated() const
{
	return isRepeated_;
}

void QuestNode::setRepeated(bool b)
{
	isRepeated_ = b;
}
