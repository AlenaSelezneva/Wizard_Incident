

#include "State.h"
#include "StateStack.h"


State::Context::Context(
	sf::RenderWindow& window,
	TextureHolder_t& textures,
	FontHolder_t& fonts,
	PlayerData* data,
	Player& player,
	MusicPlayer& music,
	SoundPlayer& sounds)
	: window(&window)
	, textures(&textures)
	, fonts(&fonts)
	, playerData(data)
	, player(&player)
	, music(&music)
	, sounds(&sounds)
{}

State::State(StateStack& stack, Context context)
	: stack(&stack)
	, context(context)
{}

void State::requestStackPush(StateID stateID)
{
	stack->pushState(stateID);
}

void State::requestStackPop()
{
	stack->popState();
}

void State::requestStateClear()
{
	stack->clearStates();
}

State::Context State::getContext()
{
	return context;
}




State::~State() {}
