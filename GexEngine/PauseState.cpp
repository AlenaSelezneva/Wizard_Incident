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
	, heroAttributes()
{
	sf::Font& font = context.fonts->get(FontID::Main);
	sf::Vector2f viewSize = context.window->getView().getSize();

	pausedText.setFont(font);
	pausedText.setString("Game Paused");
	pausedText.setCharacterSize(70);
	centerOrigin(pausedText);
	pausedText.setPosition(0.35f * viewSize.x, 0.4f * viewSize.y);


	textures = new TextureHolder_t();
	textures->load(TextureID::AttributesBackground, "Media/Textures/attributesView.png");
	textures->load(TextureID::InstrucitonsBackground, "Media/Textures/instructionsView.png");

	float margin = 20.f;

	instructions = new UiNode(textures->get(TextureID::InstrucitonsBackground));
	instructions->setPosition(0.67f * viewSize.x, 0.42f * viewSize.y);
	instructions->setVisible(true);

	std::unique_ptr<TextNode> instr(new TextNode(*context.fonts, "", 26));
	instr->setString("W or Up\t\t\tgo forward\nS or Down\t\tgo backward\nA or Left\t\t\tgo left\nD or Right\t\t\tgo Right\nJ\t\t\t\t\t\t\tQuests\n\nSpace\t\t\t\tAttack\nH\t\t\t\t\t\t\tHeal\nP\t\t\t\t\t\t\tPower Attack\nR.Shift\t\t\t\tShield");
	instr->setTextColor(sf::Color(125, 120, 186));

	instr->setPosition(instructions->getBoundingRect().width / 2, instructions->getBoundingRect().height / 2);
	instructions->attachChild(std::move(instr));



	heroAttributes = new UiNode(textures->get(TextureID::AttributesBackground));
	heroAttributes->setPosition(margin, margin);
	heroAttributes->setVisible(true);

	std::unique_ptr<TextNode> stats(new TextNode(*context.fonts, "", 26));
	stats->setString(context.playerData->getHeroAttributesString());
	stats->setTextColor(sf::Color(125, 120, 186));

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
	window.draw(*instructions);
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
		context.music->setPaused(false);
		requestStackPop();
	}
	if (event.key.code == sf::Keyboard::BackSpace)
	{
		requestStateClear();
	}
	return false;
}


