#include "BattlePlayer.h"

BattlePlayer::BattlePlayer()
	: currentMissionStatus(MissionStatus::Running)
{
	initializeKeyBindings();
	initializeActions();

	for (auto& pair : actionBindings) {
		pair.second.category = Category::Hero;
	}
}

BattlePlayer::BattlePlayer(PlayerData* data)
	: currentMissionStatus(MissionStatus::Running)
	, playerData(data)
{
	initializeKeyBindings();
	initializeActions();

	for (auto& pair : actionBindings) {
		pair.second.category = Category::Hero;
	}
}

void BattlePlayer::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		// Check if pressed key appears in key binding, trigger command if so
		auto found = keyBindings.find(event.key.code);
		if (found != keyBindings.end() && !isRealtimeAction(found->second))
			commands.push(actionBindings[found->second]);
	}
}

void BattlePlayer::handleRealTimeInput(CommandQueue& commands)
{
	for (auto pair : keyBindings) {

		// If key is pressed, lookup action and trigger corresponding command
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
			commands.push(actionBindings[pair.second]);
	}

}

void BattlePlayer::setMissionStatus(MissionStatus status)
{
	currentMissionStatus = status;
}

BattlePlayer::MissionStatus BattlePlayer::getMissionStatus() const
{
	return currentMissionStatus;
}

void BattlePlayer::initializeKeyBindings() {
	keyBindings[sf::Keyboard::Left] = Action::MoveLeft;
	keyBindings[sf::Keyboard::Right] = Action::MoveRight;
	keyBindings[sf::Keyboard::Up] = Action::MoveUp;
	keyBindings[sf::Keyboard::Down] = Action::MoveDown;

	keyBindings[sf::Keyboard::A] = Action::MoveLeft;
	keyBindings[sf::Keyboard::D] = Action::MoveRight;
	keyBindings[sf::Keyboard::W] = Action::MoveUp;
	keyBindings[sf::Keyboard::S] = Action::MoveDown;

	keyBindings[sf::Keyboard::Space] = Action::Attack;

	keyBindings[sf::Keyboard::RShift] = Action::CastShield;

	//keyBindings[sf::Keyboard::Enter] = Action::Interact;
	//keyBindings[sf::Keyboard::Space] = Action::ContinueDialog;
}


void BattlePlayer::initializeActions()
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



	actionBindings[Action::Attack].action = derivedAction<Actor>(
		[playerSpeed](Actor& a, sf::Time dt) {
			a.attack();
		});

	actionBindings[Action::CastShield].action = derivedAction<Actor>(
		[](Actor& a, sf::Time dt) {
			a.setCastingShield(true);
		});
}

bool BattlePlayer::isRealtimeAction(Action action)
{
	switch (action)
	{
	case Action::MoveLeft:
	case Action::MoveRight:
	case Action::MoveDown:
	case Action::MoveUp:
	case Action::CastShield:

		return true;
	case Action::Attack:

		return false;
	default:
		return false;
	}
}
