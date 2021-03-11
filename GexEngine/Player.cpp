

#include "Player.h"
#include <algorithm>
#include "CommandQueue.h"
#include "Actor.h"
#include "FriendlyNPC.h"
#include "State.h"
#include <iostream>

Player::Player(PlayerData* data)
	: currentMissionStatus(MissionStatus::Running)
	, playerData(data)
{

	initializeKeyBindings();
	initializeActions();

	for (auto& pair : actionBindings) {
		if (pair.first == Action::Interact) {
			pair.second.category = Category::TalkingNPC;
		}
		else if (pair.first == Action::ShowOrHideJournal) {
			pair.second.category = Category::Hero;
		}
		/*else if (pair.first == Action::ContinueDialog) {
			pair.second.category = Category::Hero;
		} */
		else {
			pair.second.category = Category::Hero;
		}
	}
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		// Check if pressed key appears in key binding, trigger command if so
		auto found = keyBindings.find(event.key.code);
		if (found != keyBindings.end() && !isRealtimeAction(found->second))
			commands.push(actionBindings[found->second]);
	}
}

void Player::handleRealTimeInput(CommandQueue& commands)
{
	for (auto pair : keyBindings) {

		// If key is pressed, lookup action and trigger corresponding command
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
			commands.push(actionBindings[pair.second]);
	}

}

void Player::setMissionStatus(MissionStatus status)
{
	currentMissionStatus = status;
}

Player::MissionStatus Player::getMissionStatus() const
{
	return currentMissionStatus;
}

void Player::initializeKeyBindings() {
	keyBindings[sf::Keyboard::Left] = Action::MoveLeft;
	keyBindings[sf::Keyboard::Right] = Action::MoveRight;
	keyBindings[sf::Keyboard::Up] = Action::MoveUp;
	keyBindings[sf::Keyboard::Down] = Action::MoveDown;

	keyBindings[sf::Keyboard::A] = Action::MoveLeft;
	keyBindings[sf::Keyboard::D] = Action::MoveRight;
	keyBindings[sf::Keyboard::W] = Action::MoveUp;
	keyBindings[sf::Keyboard::S] = Action::MoveDown;

	keyBindings[sf::Keyboard::J] = Action::ShowOrHideJournal;

	keyBindings[sf::Keyboard::Space] = Action::SpellCast;

	//keyBindings[sf::Keyboard::Enter] = Action::Interact;
	//keyBindings[sf::Keyboard::Space] = Action::ContinueDialog;
}


void Player::initializeActions()
{
	const float playerSpeed = 100.f;
	
	actionBindings[Action::MoveLeft].action = derivedAction<Actor>(
		[playerSpeed](Actor& a, sf::Time dt) {
			a.accelerate(sf::Vector2f(-playerSpeed, 0.f));
		});

	actionBindings[Action::MoveRight].action = derivedAction<Actor>(
		[playerSpeed](Actor& a, sf::Time dt) {
			a.accelerate(sf::Vector2f(playerSpeed, 0.f));
		});

	actionBindings[Action::MoveUp].action = derivedAction<Actor>(
		[playerSpeed](Actor& a, sf::Time dt) {
			a.accelerate(sf::Vector2f(0.f, -playerSpeed));
		});

	actionBindings[Action::MoveDown].action = derivedAction<Actor>(
		[playerSpeed](Actor& a, sf::Time dt) {
			a.accelerate(sf::Vector2f(0.f, playerSpeed));
		});

	actionBindings[Action::SpellCast].action = derivedAction<Actor>(
		[playerSpeed](Actor& a, sf::Time dt) {
			a.setSpellCasting(true);
		});

	actionBindings[Action::ShowOrHideJournal].action = derivedAction<SceneNode>(
		[this](SceneNode& node, sf::Time dt) {
			playerData->flipShowingJournal();
		});

	/*actionBindings[Action::Interact].action = derivedAction<FriendlyNPC>(
		[this](FriendlyNPC& npc, sf::Time dt) {
			if (npc.canTalkToHero()) {
				//currentDialog = npc.getDialog();
				playerData->setCurrentDialog(npc.getDialog());
				std::cout << playerData->getCurrentDialog() << "\n";
			}
		});*/

	/*actionBindings[Action::ContinueDialog].action = derivedAction<Actor>(
		[this](Actor& hero, sf::Time dt) {
			std::cout << "NOT  " << playerData->getCurrentDialog() << "\n";
			playerData->setCurrentDialog("");
		});*/


	/*actionBindings[Action::Fire].action = derivedAction<Actor>(
		[](Actor& a, sf::Time dt) {
			a.attack();
		});*/
}

bool Player::isRealtimeAction(Action action)
{
	switch (action)
	{
	case Action::MoveLeft:
	case Action::MoveRight:
	case Action::MoveDown:
	case Action::MoveUp:
	case Action::SpellCast:
		return true;
	case Action::Interact:
		return false;
	default:
		return false;
	}
}
