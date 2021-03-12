#include <string>

#include "DialogNode.h"
#include "DialogManager.h"
#include "QuestManager.h"

#pragma once
class PlayerData
{
public:
	PlayerData();

	DialogNode*				getCurrentDialog() const;

	void					setCurrentDialog(Actor::Type npc);
	void					onCurrentDialogComplete();

	void					setCurrentQuestDialog(ObjectWithQuest::Type obj);
	void					onCurrectQuestDialogComplete();

	bool					hasPendingQuest(ObjectWithQuest::Type obj);

	bool					isShowingJournal();
	void					setShowingJournal(bool b);
	void					flipShowingJournal();

	std::string				getQuestInstrunstionDisplay() const;

	bool					isIntersectsWithPortal() const;
	void					setIntersectsWithPortal(bool b);


private:
	DialogManager*				dialogManager;
	QuestManager*				questManager;

	Actor::Type					currentTalkingNPC;
	ObjectWithQuest::Type		currentQuestObject;

	DialogNode*					currentDialog;

	bool						isShowingJournal_;

	bool						isIntersectsWithPortal_;

};

