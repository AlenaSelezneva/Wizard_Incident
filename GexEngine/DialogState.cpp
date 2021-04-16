#include "DialogState.h"
#include "ResourceHolder.h"
#include "Utility.h"
#include "PlayerData.h"
#include "SpriteNode.h"
#include "TextNode.h"
#include "DialogMessage.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>



DialogState::DialogState(StateStack& stack, Context context)
	: State(stack, context)
	, optionIndex(0)
	, textures()
	, playerData(context.playerData)
	, optionNodes()
	, dialogView()
{
	loadTextures();

	currentDialog = context.playerData->getCurrentDialog();

	currentMessage = sf::Text();
	currentMessage.setFont(context.fonts->get(FontID::Main));
	currentMessage.setString(currentDialog->getText());

	buildView();

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

	window.draw(*dialogView);
}

bool DialogState::update(sf::Time dt)
{
	return false;
}

void DialogState::updateChosenDialogOption()
{
	if (currentDialog->getChildren() == nullptr || currentDialog->getChildren()->size() == 0) {
		return;
	}

	for (int i = 0; i < optionNodes.size(); ++i) {
		optionNodes[i]->setTexture((i == optionIndex ? textures.get(TextureID::DialogOptionChosen) : textures.get(TextureID::DialogOption)));
	}
}

void DialogState::moveToNextDialogMessage(int childIndex)
{
	if (currentDialog->getAttribute() != Attribute::None) {
		playerData->increaseHeroAttribute(currentDialog->getAttribute());
	}

	if (currentDialog->getChildren() > 0) {
		currentDialog = currentDialog->getChildren()->at(childIndex);
	}
}

void DialogState::loadTextures()
{
	textures.load(TextureID::DialogMain, "Media/Textures/Dialog_Main_2.png");
	textures.load(TextureID::DialogOption, "Media/Textures/Dialog_Hero_Option_2.png");
	textures.load(TextureID::DialogOptionChosen, "Media/Textures/Dialog_Hero_Option_Chosen.png");
	textures.load(TextureID::DialogMainHero, "Media/Textures/Dialog_Hero_Option_Main.png");
	/*context.textures->load(TextureID::DialogMain, "Media/Textures/Dialog_Main_2.png");
	context.textures->load(TextureID::DialogOption, "Media/Textures/Dialog_Hero_Option_2.png");
	context.textures->load(TextureID::DialogOptionChosen, "Media/Textures/Dialog_Hero_Option_Chosen.png");*/
}

void DialogState::buildView()
{
	if (currentDialog == nullptr)
		return;

	dialogView = new SceneNode();

	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	if ((static_cast<DialogMessage*> (currentDialog))->isAnswerRequired()) {
		buildMessageWithOptions(&window);
	}
	else {
		buildMessage(&window);
	}
}

void DialogState::buildMessage(sf::RenderWindow* window)
{
	std::unique_ptr<SpriteNode> dialog(new SpriteNode(
		currentDialog->getType() == DialogNode::Type::Message ?
		textures.get(TextureID::DialogMain) : textures.get(TextureID::DialogMainHero)));

	optionNodes.clear();

	std::unique_ptr<TextNode> text(new TextNode(*context.fonts, "", CHARACTER_SIZE_FOR_MESSAGE));
	text->setString(currentDialog->getText());
	/*text->setPosition(150.f, 100.f);*/
	text->setPosition(dialog.get()->getBoundingRect().width / 2, dialog.get()->getBoundingRect().height / 2);

	dialog.get()->attachChild(std::move(text));

	dialog.get()->setPosition(window->getView().getCenter().x - dialog.get()->getBoundingRect().width / 2,
		window->getView().getCenter().y - dialog.get()->getBoundingRect().height / 2);

	dialogView->attachChild(std::move(dialog));
}

void DialogState::buildMessageWithOptions(sf::RenderWindow* window)
{
	optionNodes.clear();
	optionIndex = 0;

	float optionWidth = 250.f;
	float optionHeight = 90.f;
	float verticalSpacing = 15.f;
	float horizontalSpacing = 10.f;

	std::unique_ptr<SpriteNode> dialog(new SpriteNode(
		currentDialog->getType() == DialogNode::Type::Message ? 
			textures.get(TextureID::DialogMain) : textures.get(TextureID::DialogMainHero)));

	std::unique_ptr<TextNode> text(new TextNode(*context.fonts, "", CHARACTER_SIZE_FOR_MESSAGE));
	text->setString(currentDialog->getText());
	//text->setPosition(150.f, 100.f);
	text->setPosition(dialog.get()->getBoundingRect().width / 2, dialog.get()->getBoundingRect().height / 2);


	dialog.get()->attachChild(std::move(text));

	dialog.get()->setPosition(window->getView().getCenter().x - dialog.get()->getBoundingRect().width / 2 - optionWidth / 2,
		window->getView().getCenter().y - dialog.get()->getBoundingRect().height / 2);

	//centerOrigin(*text);

	for (int i = 0; i < currentDialog->getChildren()->size(); ++i) {

		std::unique_ptr<SpriteNode> option(new SpriteNode(i == optionIndex ? textures.get(TextureID::DialogOptionChosen) : textures.get(TextureID::DialogOption)));

		std::unique_ptr<TextNode> optionText(new TextNode(*context.fonts, "", CHARACTER_SIZE_FOR_OPTION));
		optionText.get()->setString(currentDialog->getChildren()->at(i)->getText());
		//optionText.get()->setString(options[i]);
		//optionText->setPosition(90.f, 50.f);
		optionText->setPosition(option.get()->getBoundingRect().width / 2, option.get()->getBoundingRect().height / 2);


		option->attachChild(std::move(optionText));
		option->setPosition(dialog.get()->getBoundingRect().width + horizontalSpacing, i * (optionHeight + verticalSpacing));

		optionNodes.push_back(option.get());

		dialog.get()->attachChild(std::move(option));
	}

	dialogView->attachChild(std::move(dialog));
}

bool DialogState::handleEvent(const sf::Event& event)
{
	if (event.type != sf::Event::KeyPressed) {
		return false;
	}

	if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Enter) {

		if (currentDialog->getChildren()->size() > 1) {
			moveToNextDialogMessage(optionIndex);
			//moveToNextDialogMessage();
			buildView();
		}
		else 
			if (currentDialog->getChildren()->size() > 0) {
			moveToNextDialogMessage();
			buildView();
		}
		else {
			context.playerData->onCurrentDialogComplete( currentDialog->isAgreeing());
			requestStackPop();
		}

	}
	else if (event.key.code == sf::Keyboard::Up) {
		--optionIndex;
		optionIndex += currentDialog->getChildren()->size();
		optionIndex %= (currentDialog->getChildren()->size() > 0 ? currentDialog->getChildren()->size() : 1);
		updateChosenDialogOption();
	}
	else if (event.key.code == sf::Keyboard::Down) {
		++optionIndex;
		optionIndex %= (currentDialog->getChildren()->size() > 0 ? currentDialog->getChildren()->size() : 1);
		updateChosenDialogOption();
	}

	return false;
}