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

CommandQueue& World::getCommands()
{
	return commandQueue;
}

bool World::hasAlivePlayer() const
{
	return true;
}

bool World::hasPlayerReachedEnd() const
{
	return false;
}


void World::update(sf::Time dt) {
	
	// scroll view 
	//worldView.move(0.f, scrollSpeed * dt.asSeconds());

	// reset player velocity
	hero->setVelocity(0.f, 0.f);
	hero->setSpellCasting(false);
	resetNPCsCanTalk();
	collidingToRedraw = std::list<SceneNode*>();

	handleCollisions();

	//destroyEntitiesOutsideView();

	//adaptNPCPosition();

	while (!commandQueue.isEmpty()) {
		sceneGraph.onCommand(commandQueue.pop(), dt);
	}

	updateSounds();

	/*if (!playerData->getCurrentDialog().empty()) {
		return;
	}*/

	//adaptPlyerVelocity();
	//sceneGraph.removeWrecks();

	adaptPlayerPositionRelatingBlocks(dt, getCommands());

	sceneGraph.update(dt, getCommands());
	adaptPlayerPosition();
}

void World::updateSounds()
{
	sounds.setListenerPosition(hero->getWorldPoition());
	sounds.removeStoppedSounds();
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

	//if (!playerData->getCurrentDialog().empty()) {

	//	std::unique_ptr<SpriteNode> dialog(new SpriteNode(textures.get(TextureID::DialogMain)));

	//	std::unique_ptr<TextNode> text(new TextNode(fonts, ""));
	//	text->setString(playerData->getCurrentDialog());
	//	text->setPosition(150.f, 60.f);

	//	dialog.get()->attachChild(std::move(text));

	//	//centerOrigin(*text);

	//	dialog.get()->setPosition(	worldView.getCenter().x - dialog.get()->getBoundingRect().width / 2,
	//								worldView.getCenter().y - dialog.get()->getBoundingRect().height / 2 );

	//	target.draw(*dialog);
	//}
}

void World::loadTextures() {
	//textures.load(TextureID::Hero2, "Media/Textures/Hero2.png");
	//textures.load(TextureID::Road, "Media/Textures/Road.png");
	//textures.load(TextureID::Eagle, "Media/Textures/Eagle.png");
	//textures.load(TextureID::Raptor, "Media/Textures/Raptor.png");
	//textures.load(TextureID::Wall, "Media/Textures/wall2.png");
	textures.load(TextureID::Floor, "Media/Textures/Floor50.png");
	textures.load(TextureID::Wall, "Media/Textures/wall300.png");

	textures.load(TextureID::Hero, "Media/Textures/Archmage_sheet.png");

	textures.load(TextureID::BookShelf, "Media/Textures/book_shelf.png");

	/*textures.load(TextureID::DialogMain, "Media/Textures/Dialog_Main_2.png");
	textures.load(TextureID::DialogOption, "Media/Textures/Dialog_Hero_Option_2.png");
	textures.load(TextureID::DialogOptionChosen, "Media/Textures/Dialog_Hero_Option_Chosen.png");*/

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
			category = Category::Type::None;
			break;
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




	std::unique_ptr<InteractableObject> shelf1(new InteractableObject(InteractableObject::Type::BookshelfNotQuest, ObjectWithQuest::Type::BookshelfQuest, textures, fonts));
	shelf1->setPosition(spawnPosition.x - 100.f, spawnPosition.y - 1100.f);
	sceneLayers[PlayerLayer]->attachChild(std::move(shelf1));

	std::unique_ptr<InteractableObject> shelf2(new InteractableObject(InteractableObject::Type::BookshelfNotQuest, ObjectWithQuest::Type::BookshelfNotQuest, textures, fonts));
	shelf2->setPosition(spawnPosition.x + 150.f, spawnPosition.y - 1100.f);
	sceneLayers[PlayerLayer]->attachChild(std::move(shelf2));

	std::unique_ptr<InteractableObject> shelf3(new InteractableObject(InteractableObject::Type::BookshelfNotQuest, ObjectWithQuest::Type::BookshelfNotQuest, textures, fonts));
	shelf3->setPosition(spawnPosition.x + 400.f, spawnPosition.y - 1100.f);
	sceneLayers[PlayerLayer]->attachChild(std::move(shelf3));



	// add player aircraft
	/*std::unique_ptr<Actor> leader(new Actor(Actor::Type::Hero2, textures, fonts));
	playerAircraft = leader.get();
	playerAircraft->setPosition(spawnPosition);
	playerAircraft->setVelocity(80.f, scrollSpeed);
	sceneLayers[UpperAir]->attachChild(std::move(leader));

	addEnemies();*/
}

void World::addEnemies()
{
	// Add enemies to the spawn point container

}

void World::addEnemy(Actor::Type type, float relX, float relY)
{
	// relative to spawn position
	SpawnPoint spawn(type, spawnPosition.x + relX, spawnPosition.y - relY);
}


void World::handleCollisions()
{

	//handleCollisionsWithTiles();

	// get all colliding pairs
	std::set<SceneNode::Pair> collisionPairs;
	sceneGraph.checkSceneCollision(sceneGraph, collisionPairs);

	for (auto pair : collisionPairs) {
		if (matchesCategories(pair, Category::Hero, Category::TalkingNPC)) {
			auto& npc = static_cast<FriendlyNPC&>(*pair.second);
			npc.setCanTalkToHero(true);

			//playerData->setCurrentDialog(npc.getDialog());
			playerData->setCurrentDialog(npc.getType());

			auto& hero = static_cast<Actor&>(*pair.first);

			if (hero.getBaseTileRect().intersects(npc.getBaseTileRect())) {
				adaptPosition(&hero, &npc);

				//if (hero.getPosition().x < npc.getPosition().x) {
				//	/*hero.move(sf::Vector2f(-1.f, 0.f));*/
				//	hero.move(sf::Vector2f(-speed, 0.f));
				//}
				//else {
				//	hero.move(sf::Vector2f(speed, 0.f));
				//}

				//if (hero.getPosition().y < npc.getPosition().y) {
				//	hero.move(sf::Vector2f(0.f, -speed));
				//}
				//else {
				//	hero.move(sf::Vector2f(0.f, speed));
				//}
			}

			if (hero.getPosition().y >= npc.getPosition().y) {
				collidingToRedraw.push_back(&hero);
			}
		}
		else if (matchesCategories(pair, Category::Hero, Category::InteractableObject)) {
			auto& interactableObject = static_cast<InteractableObject&>(*pair.second);
			//playerData->setCurrentDialog(npc.getDialog());
			//playerData->setCurrentDialog(interactableObject.getType());

			auto& hero = static_cast<Actor&>(*pair.first);


			if (hero.getBaseTileRect().intersects(interactableObject.getBaseTileRect())) {
				adaptPosition(&hero, &interactableObject);
			}

			if (hero.getBaseTileRect().top + hero.getBaseTileRect().height >= 
						interactableObject.getBaseTileRect().top + interactableObject.getBaseTileRect().height) {
				collidingToRedraw.push_back(&hero);
			}
		}
		
				
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

void World::destroyEntitiesOutsideView()
{
	Command command;
	command.category = Category::NPC;
	command.action = derivedAction<Entity>([this](Entity& e, sf::Time t) {
		if (!getBattlefieldBounds().intersects(e.getBoundingRect())) {
			e.destroy();
		}
		});
	commandQueue.push(command);
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
}

void World::adaptPlayerPositionRelatingBlocks(sf::Time dt, CommandQueue& commands)
{
	auto heroCopy = new Actor(Actor::Type::Hero, textures, fonts);
	heroCopy->setPosition(hero->getPosition());
	heroCopy->setVelocity(hero->getVelocity());
	heroCopy->update(dt, commands);

	for (auto t : blockingTiles) {
		if (hero->getBoundingRect().intersects(t->getBoundingRect()) &&
				hero->getBoundingRect().top + hero->getBoundingRect().height >= t->getBoundingRect().top + t->getBoundingRect().height)
			collidingToRedraw.push_back(hero);

		if (heroCopy->getBaseTileRect().intersects(t->getBaseTileRect())) {
			if (hero->getBaseTileRect().intersects(t->getBaseTileRect())) {
				hero->accelerate(-1.75 * hero->getVelocity().x, -1.75 * hero->getVelocity().y);
			}
			else {
				hero->setVelocity(0, 0);
			}
			break;
		}
	}
}

void World::adaptPosition(Entity* ent1, Entity* ent2)
{
	float speed = 2.f;

	if (ent1->getPosition().x < ent2->getPosition().x) {
		/*hero.move(sf::Vector2f(-1.f, 0.f));*/
		ent1->move(sf::Vector2f(-speed, 0.f));
	}
	else {
		ent1->move(sf::Vector2f(speed, 0.f));
	}

	if (ent1->getPosition().y < ent2->getPosition().y) {
		ent1->move(sf::Vector2f(0.f, -speed));
	}
	else {
		ent1->move(sf::Vector2f(0.f, speed));
	}
}


void World::adaptNPCPosition()
{
	// worldView
/*
	sf::FloatRect viewBounds(worldView.getCenter() - worldView.getSize() / 2.f,
		worldView.getSize());
	const float borderDistance = 40.f;

	Command adaptPosition;
	adaptPosition.category = Category::Zombie;

	adaptPosition.action = derivedAction<Actor>([this](Actor& enemy, sf::Time dt) {
		sf::FloatRect viewBounds(worldView.getCenter() - worldView.getSize() / 2.f,
			worldView.getSize());
		const float borderDistance = 40.f;

		sf::Vector2f position = enemy.getPosition();

		position.x = std::max(position.x, viewBounds.left + borderDistance);
		position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
		position.y = std::max(position.y, viewBounds.top + borderDistance);
		position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);

		enemy.setPosition(position);
		});

	commandQueue.push(adaptPosition);*/

}

void World::resetNPCsCanTalk()
{
	playerData->setCurrentDialog(Actor::Type::None);

	/*Command resetCanTalk;
	resetCanTalk.category = Category::TalkingNPC;

	resetCanTalk.action = derivedAction<FriendlyNPC>([this](FriendlyNPC& npc, sf::Time dt) {
		npc.setCanTalkToHero(false);
		});

	commandQueue.push(resetCanTalk);*/
}

sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(worldView.getCenter() - worldView.getSize() / 2.f, worldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
	auto bounds = getViewBounds();
	bounds.top -= 100.f;
	bounds.height += 100.f;

	return bounds;
}
