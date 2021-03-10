#include "Quest.h"

Quest::Quest()
	: type(ObjectWithQuest::Type::None)
	, questDialog()
	, isCompleted_(false)
{
}

Quest::Quest(ObjectWithQuest::Type t, DialogNode* dialog)
	: type(t)
	, questDialog(dialog)
	, isCompleted_(false)
{
}

ObjectWithQuest::Type Quest::getObjectType() const
{
	return type;
}

void Quest::setObjectType(ObjectWithQuest::Type t)
{
	type = t;
}

DialogNode* Quest::getQuestDialog() const
{
	return questDialog;
}

void Quest::setQuestDialog(DialogNode* dialog)
{
	questDialog = dialog;
}

bool Quest::isCompleted() const
{
	return isCompleted_;
}

void Quest::complete()
{
	isCompleted_ = true;
}

Quest* Quest::getNextStep()
{
	return nextStep;
}

void Quest::setNextStep(Quest* q)
{
	nextStep = q;
}

