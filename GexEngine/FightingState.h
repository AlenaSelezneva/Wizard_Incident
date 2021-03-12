#include "GameState.h"

#pragma once
class FightingState : GameState
{
public:
	FightingState(StateStack& stack, Context context);

	virtual void		draw() override;
	virtual bool		update(sf::Time dt) override;
	virtual bool		handleEvent(const sf::Event& event) override;

private:
	World				world;
	Player& player;

};

