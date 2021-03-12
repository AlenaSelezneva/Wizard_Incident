#include "BattleScene.h"

BattleScene::BattleScene(sf::RenderTarget& outputTarget, const FontHolder_t& fonts, SoundPlayer& sounds, PlayerData* data)
    : World(outputTarget, fonts, sounds, data, 0)
{

	sceneTexture.create(target.getSize().x, target.getSize().y);
	loadTextures();

	worldBounds = sf::FloatRect(0.f, 0.f, 1000, 1000.f);

	float spawnMargin = 150.f;

	spawnPosition = sf::Vector2f(spawnMargin, spawnMargin);
	enemySpawnPoint = sf::Vector2f(worldView.getSize().x - spawnMargin, worldBounds.height - spawnMargin);

	worldView.setCenter(sf::Vector2f(worldView.getSize().x / 2.f, worldBounds.height - worldView.getSize().y / 2.f));

	currentLevel = LevelsTilesSchema::getBattleLevelLayout();
	tileData = initializeTileData();

	buildScene();
}

void BattleScene::update(sf::Time dt)
{
	hero->setVelocity(0.f, 0.f);
	hero->setSpellCasting(false);
	collidingToRedraw = std::list<SceneNode*>();

	while (!commandQueue.isEmpty()) {
		sceneGraph.onCommand(commandQueue.pop(), dt);
	}
	adaptPlayerVelocity();

	adaptPlayerPositionRelatingBlocks(dt, getCommands());
	handleCollisions(dt, commandQueue);

	updateSounds();

	sceneGraph.removeWrecks();

	sceneGraph.update(dt, getCommands());
	adaptPlayerPosition();
}

void BattleScene::draw()
{
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
}


bool BattleScene::hasAlivePlayer() const
{
    return hero->getHitpoints() > 0;
}

void BattleScene::loadTextures()
{
	textures.load(TextureID::Floor, "Media/Textures/floor_new.jpg");
	textures.load(TextureID::Wall, "Media/Textures/wall300.png");

	textures.load(TextureID::Hero, "Media/Textures/Hero.png");
	textures.load(TextureID::Archmage, "Media/Textures/Archmage_sheet.png");

	textures.load(TextureID::EnergyBallHero, "Media/Textures/energy_ball_hero.png");
	textures.load(TextureID::EnergyBallEnemy, "Media/Textures/energy_ball_enemy.png");

	/*textures.load(TextureID::QuestJournal, "Media/Textures/QuestJournal.png");
	textures.load(TextureID::HintBackground, "Media/Textures/Hint_Action.png");*/
}

void BattleScene::buildScene()
{
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
	sf::Texture& floorTexture = textures.get(TextureID::Floor);
	sf::Texture& wallTexture = textures.get(TextureID::Wall);

	sf::IntRect floorRect(0, 0, (int)tileData[Tile::Type::Floor].width, (int)tileData[Tile::Type::Floor].height);
	sf::IntRect wallRect(0, 0, (int)tileData[Tile::Type::Wall].width, (int)tileData[Tile::Type::Wall].height);

	float tileSize = 50.f;

	for (int i = 0; i < currentLevel.size(); ++i) {
		for (int j = 0; j < currentLevel[0].size(); ++j) {

			SpriteNode* sprite;

			switch (currentLevel[i][j])
			{
			case Tile::Type::Floor:
				sprite = new SpriteNode(floorTexture, floorRect);
				sprite->setPosition(j * tileSize, i * tileSize);
				walkOverTiles.push_back(sprite);
				break;
			case Tile::Type::Wall:
				sprite = new SpriteNode(wallTexture, wallRect);
				sprite->setPosition(j * tileSize, i * tileSize - tileData[Tile::Wall].height + tileSize);
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
		}
	}

	sf::IntRect textureRect(worldBounds);

	std::unique_ptr<Actor> hero_(new Actor(Actor::Type::Hero, textures, fonts));
	hero = hero_.get();
	hero->setPosition(spawnPosition);
	hero->setVelocity(0.f, 0.f);
	sceneLayers[PlayerLayer]->attachChild(std::move(hero_));


	/*std::unique_ptr<FriendlyNPC> archmage(new FriendlyNPC(Actor::Type::Archmage, ObjectWithQuest::Type::Archmage, textures, fonts));
	archmage->setPosition(spawnPosition.x + 200.f, spawnPosition.y - 200.f);
	archmage->setVelocity(0.f, 0.f);
	sceneLayers[PlayerLayer]->attachChild(std::move(archmage));*/
}

void BattleScene::handleCollisions(sf::Time dt, CommandQueue& commands)
{
}

void BattleScene::destroyEntitiesOutsideView()
{
}

void BattleScene::adaptPlayerPositionRelatingBlocks(sf::Time dt, CommandQueue& commands)
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

void BattleScene::adaptPlayerPosition()
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