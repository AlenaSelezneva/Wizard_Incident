//#include "FightingState.h"
//
//FightingState::FightingState(StateStack& stack, Context context)
//	: GameState(stack, context)
//	, battleScene(*context.window, *context.fonts, *context.sounds, context.playerData)
//{
//	battlePlayer = BattlePlayer(context.playerData);
//	//context.music->play(MusicID::MissionTheme);
//}
//
//void FightingState::draw()
//{
//	battleScene.draw();
//}
//
//bool FightingState::update(sf::Time dt)
//{
//	battleScene.update(dt);
//
//	CommandQueue& commands = battleScene.getCommands();
//	battlePlayer.handleRealTimeInput(commands);
//
//	return false;
//}
//
//bool FightingState::handleEvent(const sf::Event& event)
//{
//	CommandQueue& commands = battleScene.getCommands();
//	battlePlayer.handleEvent(event, commands);
//	//player.handleEvent(event, commands);
//
//	// Escape pressed, trigger the pause screen
//	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
//		requestStackPush(StateID::Pause);
//
//	return false;
//}
