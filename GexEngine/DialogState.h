#include "State.h"
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>

#pragma once
class DialogState : public State
{
public:
	DialogState(StateStack& stack, Context context);

	virtual void		draw() override;
	virtual bool		update(sf::Time dt) override;
	virtual bool		handleEvent(const sf::Event& event) override;

	void				updateChosenDialogOption();

	void				drawMainMessage(sf::RenderWindow* window);
	void				drawMessageAndOptions(sf::RenderWindow* window);

private:
	void				loadTextures();

private:
	enum OptionNames {
		Intelligence, 
		Wisdom, 
		Arrogance
	};

private:
	TextureHolder_t			textures;
	sf::Text				currentMessage;
	std::vector<sf::Text>	options;
	std::size_t				optionIndex;

};
