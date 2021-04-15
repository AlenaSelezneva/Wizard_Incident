#include <string>

#include "DialogNode.h"
#include "DialogManager.h"
#include "QuestManager.h"
#include "Attribute.h"

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

	bool isInFightState() const;
	void seInFightState(bool b);

	size_t					getHeroAttribute(Attribute a);
	std::string				getHeroAttributesString();

	void					increaseHeroAttribute(Attribute a);

private:
	DialogManager*				dialogManager;
	QuestManager*				questManager;

	Actor::Type					currentTalkingNPC;
	ObjectWithQuest::Type		currentQuestObject;

	DialogNode*					currentDialog;

	bool						isShowingJournal_;

	bool						isIntersectsWithPortal_;

	bool						isInFightState_;

	std::map<Attribute, size_t> heroStats;

};

