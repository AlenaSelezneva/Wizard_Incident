/*
Alena Selezneva
*/

#include "PauseState.h"
#include "Utility.h"
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "MusicPlayer.h"
#include "TextNode.h"
#include "PlayerData.h"
#include "UiNode.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


PauseState::PauseState(StateStack& stack, Context context)
	: State(stack, context)
	, textures()
	, backgroundSprite()
	, pausedText()
	, instructionText()
	, heroAttributes()
{
	sf::Font& font = context.fonts->get(FontID::Main);
	sf::Vector2f viewSize = context.window->getView().getSize();

	pausedText.setFont(font);
	pausedText.setString("Game Paused");
	pausedText.setCharacterSize(70);
	centerOrigin(pausedText);
	pausedText.setPosition(0.5f * viewSize.x, 0.4f * viewSize.y);

	instructionText.setFont(font);
	instructionText.setString("(Press Backspace to return to the main menu)");
	centerOrigin(instructionText);
	instructionText.setPosition(0.5f * viewSize.x, 0.6f * viewSize.y);

	float margin = 20.f;

	textures = new TextureHolder_t();
	textures->load(TextureID::HintBackground, "Media/Textures/Hint_Action.png");

	heroAttributes = new UiNode(textures->get(TextureID::HintBackground));
	heroAttributes->setPosition(margin, margin);
	heroAttributes->setVisible(true);
	//heroAttributes = statsBackground;

	std::unique_ptr<TextNode> stats(new TextNode(*context.fonts, "", 20));
	stats->setString(context.playerData->getHeroAttributesString());
	//stats->setTextColor(sf::Color(125, 120, 186));

	stats->setPosition(heroAttributes->getBoundingRect().width / 2, heroAttributes->getBoundingRect().height / 2);
	heroAttributes->attachChild(std::move(stats));

	context.music->setPaused(true);
}

void PauseState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	// somewhat transparent, "dim" the window
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(pausedText);
	window.draw(instructionText);
	window.draw(*heroAttributes);
}

bool PauseState::update(sf::Time dt)
{
	return false;
}

bool PauseState::handleEvent(const sf::Event& event)
{
	if (event.type != sf::Event::KeyPressed)
		return false;
	if (event.key.code == sf::Keyboard::Escape)
	{
		// Escape pressed, remove itself to return to the game
		context.music->setPaused(false);
		requestStackPop();
	}
	if (event.key.code == sf::Keyboard::BackSpace)
	{
		// Escape pressed, remove itself to return to the game
		requestStateClear();
		//requestStackPush(StateID::Menu);
	}
	return false;
}


