/*
Alena Selezneva
*/

#pragma once

#include <SFML/Window/Event.hpp>
#include "Command.h"
#include "PlayerData.h"
#include <map>

//forward declaration
class CommandQueue;

class Player
{
public:
	enum class Action {
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,

		Interact,
		//ContinueDialog,

		//SpellCast,

		Attack,
		CastShield,
		HealingSpell,
		PowerAttack,

		ShowOrHideJournal,

		ActionCound,
	};

	enum class MissionStatus {
		Running,
		Success,
		Failure
	};

public:
	//Player();
	Player(PlayerData* data);

	void				handleEvent(const sf::Event& event, CommandQueue& commands);
	void				handleRealTimeInput(CommandQueue& commands);

	void				setMissionStatus(MissionStatus status);
	MissionStatus		getMissionStatus() const;

private:
	void				initializeActions();
	void				initializeKeyBindings();
	static bool			isRealtimeAction(Action action);

	MissionStatus		currentMissionStatus;
	PlayerData*			playerData;

	//public:
private:
	std::map<sf::Keyboard::Key, Action>		keyBindings;
	std::map<Action, Command>				actionBindings;

};

