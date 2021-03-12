#include "FightingState.h"

FightingState::FightingState(StateStack& stack, Context context)
	: GameState(stack, context)
	, battleScene(*context.window, *context.fonts, *context.sounds, context.playerData)
	, player(*context.player)
{
	//context.music->play(MusicID::MissionTheme);
}

void FightingState::draw()
{
	battleScene.draw();
}

bool FightingState::update(sf::Time dt)
{
	battleScene.update(dt);

	CommandQueue& commands = battleScene.getCommands();
	player.handleRealTimeInput(commands);

	return false;
}

bool FightingState::handleEvent(const sf::Event& event)
{
	CommandQueue& commands = battleScene.getCommands();
	player.handleEvent(event, commands);

	// Escape pressed, trigger the pause screen
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(StateID::Pause);

	//if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter &&
	//	context.playerData->getCurrentDialog() != nullptr) {
	//	requestStackPush(StateID::Dialog);
	//	//context.playerData->setCurrentDialog("");
	//}

	return false;
}
