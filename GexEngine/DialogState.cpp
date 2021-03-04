#include "DialogState.h"
#include "ResourceHolder.h"
#include "Utility.h"
#include "PlayerData.h"
#include "SpriteNode.h"
#include "TextNode.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>



DialogState::DialogState(StateStack& stack, Context context)
	: State(stack, context)
	, options()
	, optionIndex(0)
	, textures()
{
	loadTextures();

	currentMessage = sf::Text();
	currentMessage.setFont(context.fonts->get(FontID::Main));
	currentMessage.setString(context.playerData->getCurrentDialog());

	/*sf::Text playOption;
	playOption.setFont(context.fonts->get(FontID::Main));
	playOption.setString("Play");
	centerOrigin(playOption);
	playOption.setPosition(context.window->getView().getSize() / 2.f);
	options.push_back(playOption);

	sf::Text exitOption;
	exitOption.setFont(context.fonts->get(FontID::Main));
	exitOption.setString("Exit");
	centerOrigin(exitOption);
	exitOption.setPosition(playOption.getPosition() + sf::Vector2f(0.f, 30.f));
	options.push_back(exitOption);*/

	//updateDialogOptionText();

	//context.music->play(MusicID::MenuTheme);
}

void DialogState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	// somewhat transparent, "dim" the window
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 200));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);

	//window.draw(currentMessage);

	drawMainMessage(&window);

	for (const auto& text : options) {
		window.draw(text);
	}
}

bool DialogState::update(sf::Time dt)
{
	return false;
}

bool DialogState::handleEvent(const sf::Event& event)
{
	if (event.type != sf::Event::KeyPressed) {
		return false;
	}

	if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Enter) {
		/*if (optionIndex == Play) {
			requestStackPop();
			requestStackPush(StateID::Game);
		}
		else if (optionIndex == Exit) {
			requestStackPop();

		}*/
		context.playerData->setCurrentDialog("");
		requestStackPop();

	}
	else if (event.key.code == sf::Keyboard::Up) {
		/*if (optionIndex > 0)
			optionIndex--
		else
			optionIndex = options.size() - 1;*/
		++optionIndex;
		optionIndex %= options.size();
		updateChosenDialogOption();
	}
	else if (event.key.code == sf::Keyboard::Down) {
		/*if (optionIndex < option.size() - 1)
			optionIndex++
		else
			optionIndex = 0;*/
		--optionIndex;
		optionIndex += options.size();
		optionIndex %= options.size();
		updateChosenDialogOption();
	}

	return false;
}

void DialogState::updateChosenDialogOption()
{
	if (options.empty()) {
		return;
	}
	for (auto& text : options) {
		text.setFillColor(sf::Color::White);
	}
	options[optionIndex].setFillColor(sf::Color::Red);
}

void DialogState::drawMainMessage(sf::RenderWindow* window)
{

	std::unique_ptr<SpriteNode> dialog(new SpriteNode(textures.get(TextureID::DialogMain)));

	std::unique_ptr<TextNode> text(new TextNode(*context.fonts, ""));
	text->setString(currentMessage.getString());
	text->setPosition(150.f, 100.f);

	dialog.get()->attachChild(std::move(text));

	//centerOrigin(*text);


	dialog.get()->setPosition(window->getView().getCenter().x - dialog.get()->getBoundingRect().width / 2,
		window->getView().getCenter().y - dialog.get()->getBoundingRect().height / 2);

	window->draw(*dialog);	
}

void DialogState::drawMessageAndOptions(sf::RenderWindow* window)
{

}

void DialogState::loadTextures()
{
	textures.load(TextureID::DialogMain, "Media/Textures/Dialog_Main_2.png");
	textures.load(TextureID::DialogOption, "Media/Textures/Dialog_Hero_Option_2.png");
	textures.load(TextureID::DialogOptionChosen, "Media/Textures/Dialog_Hero_Option_Chosen.png");
	/*context.textures->load(TextureID::DialogMain, "Media/Textures/Dialog_Main_2.png");
	context.textures->load(TextureID::DialogOption, "Media/Textures/Dialog_Hero_Option_2.png");
	context.textures->load(TextureID::DialogOptionChosen, "Media/Textures/Dialog_Hero_Option_Chosen.png");*/
}