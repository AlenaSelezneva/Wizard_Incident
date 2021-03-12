/*
Alena Selezneva
*/


#include "World.h"
#include "Player.h"
#include "Category.h"
#include "FriendlyNPC.h"
#include "TextNode.h"
#include "Utility.h"
#include "InteractableObject.h"
#include "ObjectWithQuest.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <iostream>


World::World(sf::RenderTarget& outputTarget, const FontHolder_t& fonts, SoundPlayer& sounds, PlayerData* data)
	: target(outputTarget)
	, sceneTexture()
	, worldView(outputTarget.getDefaultView())
	, textures()
	, fonts(fonts)
	, sounds(sounds)
	, playerData(data)
	, sceneGraph()
	, uiGraph()
	, sceneLayers()
	, commandQueue()
	, worldBounds(0.f, 0.f, 1500.f, 1500.f)
	, spawnPosition(worldView.getSize().x / 2.f, worldBounds.height - worldView.getSize().y / 2.f)
{
	sceneTexture.create(target.getSize().x, target.getSize().y);
	loadTextures();

	worldView.setCenter(spawnPosition);

	currentLevel = LevelsTilesSchema::getLevelLayout();
	tileData = initializeTileData();

	buildScene();
}

World::World(sf::RenderTarget& outputTarget, const FontHolder_t& fonts, SoundPlayer& sounds, PlayerData* data, int n)
	: target(outputTarget)
	, sceneTexture()
	, worldView(outputTarget.getDefaultView())
	, textures()
	, fonts(fonts)
	, sounds(sounds)
	, playerData(data)
	, sceneGraph()
	, sceneLayers()
	, commandQueue()
{}

CommandQueue& World::getCommands()
{
	return commandQueue;
}

bool World::hasAlivePlayer() const
{
	return true;
}


void World::update(sf::Time dt) {
	
	// scroll view 
	//worldView.move(0.f, scrollSpeed * dt.asSeconds());

	// reset player velocity
	hero->setVelocity(0.f, 0.f);
	hero->setSpellCasting(false);
	resetNPCsCanTalk();
	playerData->setIntersectsWithPortal(false);
	hintView->setVisible(false);
	hintText->setString("Press ENTER to interact");
	collidingToRedraw = std::list<SceneNode*>();

	//handleCollisions(dt, commandQueue);

	//destroyEntitiesOutsideView();

	//adaptNPCPosition();

	while (!commandQueue.isEmpty()) {
		sceneGraph.onCommand(commandQueue.pop(), dt);
	}
	adaptPlayerVelocity();

	adaptPlayerPositionRelatingBlocks(dt, getCommands());
	handleCollisions(dt, commandQueue);

	updateSounds();
	updateUiElements();

	/*if (!playerData->getCurrentDialog().empty()) {
		return;
	}*/

	//adaptPlyerVelocity();
	//sceneGraph.removeWrecks();

	sceneGraph.update(dt, getCommands());
	adaptPlayerPosition();

	//sceneLayers[UiLevel]->setPosition(worldView.getCenter());

}

void World::updateSounds()
{
	sounds.setListenerPosition(hero->getWorldPoition());
	sounds.removeStoppedSounds();
}

void World::updateUiElements()
{
	questLog->setString(playerData->getQuestInstrunstionDisplay());
	questsView->setVisible(playerData->isShowingJournal());
}

void World::draw() {

	target.setView(worldView);
	for (auto t : walkOverTiles) {
		target.draw(*t);
	}

	target.draw(sceneGraph);

	for (auto t : blockingTiles) {
		target.draw(*t);
	}

	for (auto r : collidingToRedraw) {
		target.draw(*r);
	}

	//if (playerData->isShowingJournal())
	//	target.draw(*questsView);

	//uiGraph.setPosition(worldView.getViewport().left, worldView.getViewport().top);
	
	target.draw(*uiGraph);
}

void World::loadTextures() {
	//textures.load(TextureID::Floor, "Media/Textures/Floor50.png");
	textures.load(TextureID::Floor, "Media/Textures/floor_new.jpg");
	textures.load(TextureID::Wall, "Media/Textures/wall300.png");

	textures.load(TextureID::Hero, "Media/Textures/Hero.png");
	textures.load(TextureID::Archmage, "Media/Textures/Archmage_sheet.png");

	textures.load(TextureID::BookShelf, "Media/Textures/book_shelf.png");

	textures.load(TextureID::QuestJournal, "Media/Textures/QuestJournal.png");
	textures.load(TextureID::HintBackground, "Media/Textures/Hint_Action.png");

	textures.load(TextureID::Portal, "Media/Textures/portal.png");

	textures.load(TextureID::ShieldSpell, "Media/Textures/shield_spell.png");
}

void World::buildScene() {
	
	for (std::size_t i = 0; i < LayerCount; ++i) {
		Category::Type category;
		switch (i) {
		case Floor:
			category = Category::Type::Background;
		case Walls:
			category = Category::Type::Wall;
			break;
		case PlayerLayer:
		case SpellsLayer:
		default:
			category = Category::Type::None;
			break;
		}

		SceneNode::Ptr layer(new SceneNode(category));
		
		sceneLayers[i] = layer.get();

		sceneGraph.attachChild(std::move(layer));
	}

	walkOverTiles = std::vector<SpriteNode*>();
	blockingTiles = std::vector<SpriteNode*>();
	invisibleWallTiles = std::vector<SpriteNode*>();


	// prepare background texture
	/*sf::Texture& texture = textures.get(TextureID::Road);
	texture.setRepeated(true);*/
	sf::Texture& floorTexture = textures.get(TextureID::Floor);
	sf::Texture& wallTexture = textures.get(TextureID::Wall);

	sf::IntRect floorRect(0, 0, (int)tileData[Tile::Type::Floor].width, (int)tileData[Tile::Type::Floor].height);
	sf::IntRect wallRect(0, 0, (int)tileData[Tile::Type::Wall].width, (int)tileData[Tile::Type::Wall].height);

	float tileSize = 50.f;

	for (int i = 0; i < currentLevel.size(); ++i) {
		for (int j = 0; j < currentLevel[0].size(); ++j) {

			//std::unique_ptr<SpriteNode> sprite;
			//SpriteNode sprite;

			SpriteNode* sprite;

			switch (currentLevel[i][j])
			{
			case Tile::Type::Floor:
				//sprite = std::unique_ptr<SpriteNode>(new SpriteNode(floorTexture, floorRect));
				sprite = new SpriteNode(floorTexture, floorRect);
				sprite->setPosition(j * tileSize, i * tileSize);
				walkOverTiles.push_back(sprite);
				break;
			case Tile::Type::Wall:
				sprite = new SpriteNode(wallTexture, wallRect);
				sprite->setPosition(j * tileSize , i * tileSize - tileData[Tile::Wall].height + tileSize);
				blockingTiles.push_back(sprite);
				break;
			case Tile::Type::InvisibleWall:
				sprite = new SpriteNode(wallTexture, wallRect);
				sprite->setPosition(j * tileSize, i * tileSize - tileData[Tile::Wall].height + tileSize);
				invisibleWallTiles.push_back(sprite);
				break;
			default:
				break;
			}

			//sprite->setPosition(j * tileSize + 300.f, i * tileSize);
			//sceneLayers[Floor]->attachChild(std::move(sprite));
		}
	}


	//float viewHeight = worldView.getSize().y;
	sf::IntRect textureRect(worldBounds);
	//textureRect.height += static_cast<int>(viewHeight);

	// background spriteNode
	/*std::unique_ptr<SpriteNode> road(new SpriteNode(texture, textureRect));
	road->setPosition(worldBounds.left / 2, worldBounds.top / 2);
	sceneLayers[Floor]->attachChild(std::move(road));*/



	std::unique_ptr<Actor> hero_(new Actor(Actor::Type::Hero, textures, fonts));
	hero = hero_.get();
	hero->setPosition(spawnPosition);
	hero->setVelocity(0.f, 0.f);
	sceneLayers[PlayerLayer]->attachChild(std::move(hero_));


	std::unique_ptr<FriendlyNPC> archmage(new FriendlyNPC(Actor::Type::Archmage, ObjectWithQuest::Type::Archmage, textures, fonts));
	archmage->setPosition(spawnPosition.x + 200.f, spawnPosition.y - 200.f);
	archmage->setVelocity(0.f, 0.f);
	sceneLayers[PlayerLayer]->attachChild(std::move(archmage));


	std::unique_ptr<FriendlyNPC> gazan(new FriendlyNPC(Actor::Type::Gazan, ObjectWithQuest::Type::Gazan, textures, fonts));
	gazan->setPosition(spawnPosition.x + 500.f, spawnPosition.y - 500.f);
	gazan->setVelocity(0.f, 0.f);
	sceneLayers[PlayerLayer]->attachChild(std::move(gazan));



	std::unique_ptr<InteractableObject> shelf1(new InteractableObject(InteractableObject::Type::BookshelfNotQuest, ObjectWithQuest::Type::BookshelfQuest, textures, fonts));
	shelf1->setPosition(spawnPosition.x - 100.f, spawnPosition.y - 1100.f);
	sceneLayers[PlayerLayer]->attachChild(std::move(shelf1));

	std::unique_ptr<InteractableObject> shelf2(new InteractableObject(InteractableObject::Type::BookshelfNotQuest, ObjectWithQuest::Type::BookshelfNotQuest, textures, fonts));
	shelf2->setPosition(spawnPosition.x + 300.f, spawnPosition.y - 1100.f);
	sceneLayers[PlayerLayer]->attachChild(std::move(shelf2));

	std::unique_ptr<InteractableObject> shelf3(new InteractableObject(InteractableObject::Type::BookshelfNotQuest, ObjectWithQuest::Type::BookshelfNotQuest, textures, fonts));
	shelf3->setPosition(spawnPosition.x + 700.f, spawnPosition.y - 1100.f);
	sceneLayers[PlayerLayer]->attachChild(std::move(shelf3));



	std::unique_ptr<SpriteNode> portal_(new SpriteNode(textures.get(TextureID::Portal)));
	portal_->setPosition(hero->getPosition().x - 600.f, hero->getPosition().y - 800.f);
	portal = portal_.get();
	sceneLayers[PlayerLayer]->attachChild(std::move(portal_));

	/*addEnemies();*/
	buildQuestView();
	buildHintView();
}

void World::buildQuestView()
{
	float margin = 20.f;

	uiGraph = new SceneNode(Category::None);

	std::unique_ptr<UiNode> qJournalBackground(new UiNode(textures.get(TextureID::QuestJournal)));
	//qJournalBackground->setPosition(worldView.getViewport().left + 20.f, worldView.getViewport().top + 200.f);
	qJournalBackground->setPosition(margin, margin);
	questsView = qJournalBackground.get();

	std::unique_ptr<TextNode> questHeaderNode(new TextNode(fonts, "", 30));
	questHeaderNode->setString("Quests\n");
	questHeaderNode->setTextColor(sf::Color(125, 120, 186));

	questHeaderNode->setPosition(qJournalBackground.get()->getBoundingRect().width / 2, 80.f);

	std::unique_ptr<TextNode> questTextNode(new TextNode(fonts, "", 16));
	questTextNode->setString("Some Quest: Do this");
	questTextNode->setTextColor(sf::Color(125, 120, 186));
	questTextNode->setPosition(qJournalBackground.get()->getBoundingRect().width / 2, 130.f);
	questLog = questTextNode.get();

	//questHeaderNode.get()->attachChild(std::move(questTextNode));

	qJournalBackground.get()->attachChild(std::move(questHeaderNode));

	qJournalBackground.get()->attachChild(std::move(questTextNode));

	uiGraph->attachChild(std::move(qJournalBackground));
}

void World::buildHintView()
{
	float margin = 20.f;

	std::unique_ptr<UiNode> hintBackground(new UiNode(textures.get(TextureID::HintBackground)));
	hintBackground->setPosition(worldView.getSize().x - hintBackground->getBoundingRect().width - margin,
		worldView.getSize().y - hintBackground->getBoundingRect().height - margin);

	hintView = hintBackground.get();

	std::unique_ptr<TextNode> hint(new TextNode(fonts, "", 20));
	hint->setString("Press ENTER to interact");
	//questHeaderNode->setTextColor(sf::Color(125, 120, 186));

	hint->setPosition(hintBackground.get()->getBoundingRect().width / 2, hintBackground.get()->getBoundingRect().height / 2);
	hintText = hint.get();

	hintBackground->attachChild(std::move(hint));

	uiGraph->attachChild(std::move(hintBackground));
}


void World::handleCollisions(sf::Time dt, CommandQueue& commands)
{

	//handleCollisionsWithTiles();

	// get all colliding pairs
	std::set<SceneNode::Pair> collisionPairs;
	sceneGraph.checkSceneCollision(sceneGraph, collisionPairs);

	if (hero->getBoundingRect().intersects(portal->getBoundingRect())) {
		hintView->setVisible(true);
		playerData->setIntersectsWithPortal(true);
	}

	for (auto pair : collisionPairs) {
		/*if (matchesCategories(pair, Category::Hero, Category::QuestObject) && playerData->hasPendingQuest((static_cast<ObjectWithQuest&>(*pair.second)).getQuestObjectType())) {

		}*/
		bool goOnQuest = false;

		if (matchesCategories(pair, Category::Hero, Category::QuestObject)) {
			auto& hero = static_cast<Actor&>(*pair.first);

			if ((*pair.second).getCategory() == Category::TalkingNPC) {
				auto& obj = static_cast<FriendlyNPC&>(*pair.second);

				if (playerData->hasPendingQuest(obj.getQuestObjectType())) {
					goOnQuest = true;
					playerData->setCurrentQuestDialog(obj.getQuestObjectType());
					hintView->setVisible(true);
				}

				adaptHeroPositionRelatingEntity(static_cast<Entity*>(pair.second), dt, commands);

				/*if (hero.getBaseTileRect().intersects(obj.getBaseTileRect())) {
					adaptPosition(&hero, &obj);
				}

				if (hero.getPosition().y >= obj.getPosition().y) {
					collidingToRedraw.push_back(&hero);
				}*/
			}
			else {
				auto& obj = static_cast<InteractableObject&>(*pair.second);

				if (playerData->hasPendingQuest(obj.getQuestObjectType())) {
					goOnQuest = true;
					playerData->setCurrentQuestDialog(obj.getQuestObjectType());
					hintView->setVisible(true);
				}

				adaptHeroPositionRelatingEntity(static_cast<Entity*>(pair.second), dt, commands);

				/*if (hero.getBaseTileRect().intersects(obj.getBaseTileRect())) {
					adaptPosition(&hero, &obj);
				}

				if (hero.getPosition().y >= obj.getPosition().y) {
					collidingToRedraw.push_back(&hero);
				}*/
			}
		}

		if (!goOnQuest && matchesCategories(pair, Category::Hero, Category::TalkingNPC)) {
			auto& npc = static_cast<FriendlyNPC&>(*pair.second);
			npc.setCanTalkToHero(true);

			playerData->setCurrentDialog(npc.getType());

			auto& hero = static_cast<Actor&>(*pair.first);

			adaptHeroPositionRelatingEntity(static_cast<Entity*>(pair.second), dt, commands);

			/*if (hero.getBaseTileRect().intersects(npc.getBaseTileRect())) {
				adaptPosition(&hero, &npc);
			}

			if (hero.getPosition().y >= npc.getPosition().y) {
				collidingToRedraw.push_back(&hero);
			}*/

			hintText->setString("Press ENTER to talk");
			hintView->setVisible(true);
		}
		
		//else if (matchesCategories(pair, Category::Hero, Category::InteractableObject)) {
		//	auto& interactableObject = static_cast<InteractableObject&>(*pair.second);
		//	//playerData->setCurrentDialog(npc.getDialog());
		//	//playerData->setCurrentDialog(interactableObject.getType());

		//	auto& hero = static_cast<Actor&>(*pair.first);


		//	if (hero.getBaseTileRect().intersects(interactableObject.getBaseTileRect())) {
		//		adaptPosition(&hero, &interactableObject);
		//	}

		//	if (hero.getBaseTileRect().top + hero.getBaseTileRect().height >= 
		//				interactableObject.getBaseTileRect().top + interactableObject.getBaseTileRect().height) {
		//		collidingToRedraw.push_back(&hero);
		//	}
		//}
		
				
	}

	/*
	for (auto pair : collisionPairs) {
		if (matchesCategories(pair, Category::Hero, Category::Zombie)) {
			auto& hero = static_cast<Actor&>(*pair.first);
			auto& zombie = static_cast<Actor&>(*pair.second);

			if (hero.getState() == Actor::State::Attack)
				zombie.damage(1);
			else
				hero.damage(1);

		}

		if (matchesCategories(pair, Category::Actor, Category::Actor)) {
			auto& first = static_cast<Actor&>(*pair.first);
			auto& second = static_cast<Actor&>(*pair.second);

			if (first.getPosition().x < second.getPosition().x) {
				first.move(sf::Vector2f(-1.f, 0.f));
				second.move(sf::Vector2f(1.f, 0.f));
			}
			else {
				first.move(sf::Vector2f(1.f, 0.f));
				second.move(sf::Vector2f(-1.f, 0.f));
			}

			if (first.getPosition().y < second.getPosition().y) {
				first.move(sf::Vector2f(0.f, -1.f));
				second.move(sf::Vector2f(0.f, 1.f));
			}
			else {
				first.move(sf::Vector2f(0.f, 1.f));
				second.move(sf::Vector2f(0.f, -1.f));
			}

		}
	}*/
}

bool World::matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
	unsigned int category1 = colliders.first->getCategory();
	unsigned int category2 = colliders.second->getCategory();

	if (type1 & category1 && type2 & category2) {
		return true;
	}
	else if (type2 & category1 && type1 & category2) {
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else {
		return false;
	}
}

//void World::destroyEntitiesOutsideView()
//{
//	Command command;
//	command.category = Category::NPC;
//	command.action = derivedAction<Entity>([this](Entity& e, sf::Time t) {
//		if (!getBattlefieldBounds().intersects(e.getBoundingRect())) {
//			e.destroy();
//		}
//		});
//	commandQueue.push(command);
//}

void World::adaptPlayerVelocity()
{
	sf::Vector2f velocity = hero->getVelocity();

	// If moving diagonally, normalize the velocity
	if (velocity.x != 0.f && velocity.y != 0.f)
		hero->setVelocity(velocity / std::sqrt(2.f));
}

void World::adaptPlayerPosition()
{
	const float borderDistance = 100.f;
	const float scrollSpeed = 5.f;

	sf::FloatRect viewBounds(worldView.getCenter() - worldView.getSize() / 2.f,
		worldView.getSize());

	// worldView
	if (hero->getPosition().x <= viewBounds.left + borderDistance)
		worldView.move(-scrollSpeed, 0);
	if (hero->getPosition().x + hero->getBoundingRect().width >= viewBounds.left + viewBounds.width - borderDistance)
		worldView.move(scrollSpeed, 0);
	if (hero->getPosition().y <= viewBounds.top + borderDistance)
		worldView.move(0, -scrollSpeed);
	if (hero->getPosition().y + hero->getBoundingRect().height >= viewBounds.top + viewBounds.height - borderDistance)
		worldView.move(0, scrollSpeed);

	uiGraph->setPosition(viewBounds.left, viewBounds.top);
}

void World::adaptPlayerPositionRelatingBlocks(sf::Time dt, CommandQueue& commands)
{
	size_t deltaTime = 20;

	auto heroCopy = new Actor(Actor::Type::Hero, textures, fonts);
	heroCopy->setPosition(hero->getPosition());
	heroCopy->setVelocity(hero->getVelocity());

	heroCopy->update(sf::milliseconds(dt.asMilliseconds() + deltaTime), commands);

	for (auto t : blockingTiles) {
		if (hero->getBoundingRect().intersects(t->getBoundingRect()) &&
				hero->getBoundingRect().top + hero->getBoundingRect().height >= t->getBoundingRect().top + t->getBoundingRect().height)
			collidingToRedraw.push_back(hero);

		if (heroCopy->getBaseTileRect().intersects(t->getBaseTileRect())) {
			if (hero->getBaseTileRect().intersects(t->getBaseTileRect())) {
				//hero->accelerate(-1.75 * hero->getVelocity().x, -1.75 * hero->getVelocity().y);
				adaptPosition(hero, t);
			}
			else {
				hero->setVelocity(0, 0);
			}
			break;
		}
	}

	for (auto t : invisibleWallTiles) {
		if (hero->getBoundingRect().intersects(t->getBoundingRect()) &&
			hero->getBoundingRect().top + hero->getBoundingRect().height >= t->getBoundingRect().top + t->getBoundingRect().height)
			collidingToRedraw.push_back(hero);

		if (heroCopy->getBaseTileRect().intersects(t->getBaseTileRect())) {
			if (hero->getBaseTileRect().intersects(t->getBaseTileRect())) {
				//hero->accelerate(-1.75 * hero->getVelocity().x, -1.75 * hero->getVelocity().y);
				adaptPosition(hero, t);
			}
			else {
				hero->setVelocity(0, 0);
			}
			break;
		}
	}
}

void World::adaptPosition(Entity* ent1, SceneNode* sceneNode)
{
	float speed = 2.f;

	if (ent1->getPosition().x < sceneNode->getPosition().x) {
		/*hero.move(sf::Vector2f(-1.f, 0.f));*/
		ent1->move(sf::Vector2f(-speed, 0.f));
	}
	else {
		ent1->move(sf::Vector2f(speed, 0.f));
	}

	if (ent1->getPosition().y < sceneNode->getPosition().y) {
		ent1->move(sf::Vector2f(0.f, -speed));
	}
	else {
		ent1->move(sf::Vector2f(0.f, speed));
	}
}

void World::adaptHeroPositionRelatingEntity(Entity* entity, sf::Time dt, CommandQueue& commands)
{
	auto copy = new Actor(Actor::Type::Hero, textures, fonts);
	copy->setPosition(hero->getPosition());
	copy->setVelocity(hero->getVelocity());
	/*copy->update(dt, commands);*/

	size_t deltaTime = 20;
	copy->update(sf::milliseconds(dt.asMilliseconds() + deltaTime), commands);

	SceneNode* obj;

	if (entity->getCategory() == Category::TalkingNPC) {
		obj = static_cast<FriendlyNPC*>(entity);
	}
	else {
		obj = static_cast<InteractableObject*>(entity);
	}


	if (hero->getBoundingRect().intersects(obj->getBoundingRect()) &&
		hero->getBoundingRect().top + hero->getBoundingRect().height >= obj->getBoundingRect().top + obj->getBoundingRect().height)
		collidingToRedraw.push_back(hero);
	else
		collidingToRedraw.push_back(obj);

	if (copy->getBaseTileRect().intersects(obj->getBaseTileRect())) {
		if (hero->getBaseTileRect().intersects(obj->getBaseTileRect())) {
			hero->accelerate(-1.75 * hero->getVelocity().x, -1.75 * hero->getVelocity().y);
			adaptPosition(hero, obj);
		}
		else {
			hero->setVelocity(0, 0);
		}
	}
}

void World::resetNPCsCanTalk()
{
	playerData->setCurrentDialog(Actor::Type::None);
}

sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(worldView.getCenter() - worldView.getSize() / 2.f, worldView.getSize());
}

//sf::FloatRect World::getBattlefieldBounds() const
//{
//	auto bounds = getViewBounds();
//	bounds.top -= 100.f;
//	bounds.height += 100.f;
//
//	return bounds;
//}
