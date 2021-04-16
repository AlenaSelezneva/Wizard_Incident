
#pragma once

#include "State.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class UiNode;

class PauseState :
	public State
{
public:
	PauseState(StateStack& stack, Context context);

	virtual void		draw() override;
	virtual bool		update(sf::Time dt) override;
	virtual bool		handleEvent(const sf::Event& event) override;

private:
	TextureHolder_t*		textures;
	sf::Sprite			backgroundSprite;
	sf::Text			pausedText;

	UiNode*				heroAttributes;
	UiNode*				instructions;
};

