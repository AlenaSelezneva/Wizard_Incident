#include "GameState.h"
#include "BattleScene.h"

#pragma once
class FightingState : public GameState
{
public:
	FightingState(StateStack& stack, Context context);

	virtual void		draw() override;
	virtual bool		update(sf::Time dt) override;
	virtual bool		handleEvent(const sf::Event& event) override;

private:
	BattleScene			battleScene;
	Player&				player;

};

