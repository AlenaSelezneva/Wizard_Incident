/*
Alena Selezneva
*/

#include "GameState.h"
#include "MusicPlayer.h"

#include <SFML/Graphics/RenderWindow.hpp>

GameState::GameState(StateStack& stack, Context context)
	: State(stack, context)
	, world(*context.window, *context.fonts, *context.sounds, *context.music, context.playerData)
	, player(*context.player)
{
	//context.music->play(MusicID::MissionTheme);
}

void GameState::draw()
{
	world.draw();
}

bool GameState::update(sf::Time dt)
{
	world.update(dt);

	CommandQueue& commands = world.getCommands();
	player.handleRealTimeInput(commands);

	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	CommandQueue& commands = world.getCommands();
	player.handleEvent(event, commands);

	// Escape pressed, trigger the pause screen
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(StateID::Pause);

	if (context.playerData->isRequiredTalking()) {
		requestStackPush(StateID::Dialog);
	}

	/*if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
		world.startFight(Actor::Type::Archmage);*/

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter &&
			context.playerData->isIntersectsWithPortal()) {
		//requestStackPop();
		//requestStackPush(StateID::Battle);
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter &&
				context.playerData->getCurrentDialog() != nullptr ) {
		requestStackPush(StateID::Dialog);
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::B) {
		requestStackPop();
		requestStackPush(StateID::Battle);
	}

	return true;
}
