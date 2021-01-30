/*
Alena Selezneva
*/


#include "World.h"
#include "Player.h"
#include "Category.h"
#include "SoundNode.h"

#include <SFML/Graphics/RenderTarget.hpp>


World::World(sf::RenderTarget& outputTarget, const FontHolder_t& fonts, SoundPlayer& sounds)
	: target(outputTarget)
	, sceneTexture()
	, worldView(outputTarget.getDefaultView())
	, textures()
	, fonts(fonts)
	, sounds(sounds)
	, sceneGraph()
	, sceneLayers()
	, commandQueue()
	, worldBounds(0.f, 0.f, 1000.f, 600.f)
	, spawnPosition(worldView.getSize().x / 2.f, worldBounds.height - worldView.getSize().y / 2.f)

{
	sceneTexture.create(target.getSize().x, target.getSize().y);
	loadTextures();
	buildScene();

	worldView.setCenter(spawnPosition);
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

	//destroyEntitiesOutsideView();

	//adaptNPCPosition();

	while (!commandQueue.isEmpty()) {
		sceneGraph.onCommand(commandQueue.pop(), dt);
	}

	//adaptPlyerVelocity();

	//sceneGraph.removeWrecks();

	handleCollisions();

	sceneGraph.update(dt, getCommands());
	adaptPlayerPosition();
	updateSounds();
}

void World::updateSounds()
{
	/*sounds.setListenerPosition(playerAircraft->getWorldPoition());
	sounds.removeStoppedSounds();*/
}

void World::draw() {

	target.setView(worldView);
	target.draw(sceneGraph);
}

void World::loadTextures() {
	//textures.load(TextureID::Hero2, "Media/Textures/Hero2.png");
	textures.load(TextureID::Road, "Media/Textures/Road.png");
	textures.load(TextureID::Eagle, "Media/Textures/Eagle.png");
}

void World::buildScene() {
	
	for (std::size_t i = 0; i < LayerCount; ++i) {
		Category::Type category;
		switch (i) {
		case Floor:
		case Walls:
			category = Category::Type::Background;
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




	// prepare background texture
	sf::Texture& texture = textures.get(TextureID::Road);
	texture.setRepeated(true);

	//float viewHeight = worldView.getSize().y;
	sf::IntRect textureRect(worldBounds);
	//textureRect.height += static_cast<int>(viewHeight);

	// background spriteNode
	std::unique_ptr<SpriteNode> road(new SpriteNode(texture, textureRect));
	road->setPosition(worldBounds.left / 2, worldBounds.top / 2);
	sceneLayers[Floor]->attachChild(std::move(road));


	std::unique_ptr<Actor> hero_(new Actor(Actor::Type::Hero, textures, fonts));
	hero = hero_.get();
	hero->setPosition(250.f, 250.f);
	hero->setVelocity(0.f, 0.f);
	sceneLayers[PlayerLayer]->attachChild(std::move(hero_));

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
	enemySpawnPoints.push_back(spawn);
}

void World::handleCollisions()
{
	// get all colliding pairs
	std::set<SceneNode::Pair> collisionPairs;
	sceneGraph.checkSceneCollision(sceneGraph, collisionPairs);
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
	// worldView

	/*sf::FloatRect viewBounds(worldView.getCenter() - worldView.getSize() / 2.f,
		worldView.getSize());
	const float borderDistance = 40.f;

	sf::Vector2f position = playerAircraft->getPosition();
	position.x = std::max(position.x, viewBounds.left + borderDistance);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	position.y = std::max(position.y, viewBounds.top + borderDistance);
	position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);

	playerAircraft->setPosition(position);*/
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
