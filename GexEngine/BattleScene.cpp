//#include "BattleScene.h"
//#include "EnergyBolt.h"
//#include "Hero.h"
//#include "Utility.h"
//#include <iostream>
//
//BattleScene::BattleScene(sf::RenderTarget& outputTarget, const FontHolder_t& fonts, SoundPlayer& sounds, PlayerData* data)
//    : World(outputTarget, fonts, sounds, data, 0)
//	, uiGraph()
//{
//
//	sceneTexture.create(target.getSize().x, target.getSize().y);
//	loadTextures();
//
//	float margin = 50.f;
//
//	worldBounds = sf::FloatRect(0.f, 0.f, 800, 600.f);
//
//	worldView.setCenter(sf::Vector2f(worldView.getSize().x / 2.f, worldBounds.height - worldView.getSize().y / 2.f + margin));
//
//
//	spawnPosition = sf::Vector2f(300.f, 300.f);
//	enemySpawnPoint = sf::Vector2f(-100.f, -100.f);
//
//	/*spawnPosition = sf::Vector2f(worldView.getSize().x / 2.f + spawnMargin, worldBounds.height - worldView.getSize().y / 2.f + spawnMargin);
//	enemySpawnPoint = sf::Vector2f(worldView.getSize().x / 2.f - spawnMargin, worldBounds.height - worldView.getSize().y / 2.f - spawnMargin);*/
//
//
//	currentLevel = LevelsTilesSchema::getBattleLevelLayout();
//	tileData = initializeTileData();
//
//	buildScene();
//	buildUiStats();
//}
//
//void BattleScene::update(sf::Time dt)
//{
//	hero->setVelocity(0.f, 0.f);
//	hero->setSpellCasting(false);
//	collidingToRedraw = std::list<SceneNode*>();
//
//	guideEnergyBolts();
//
//	hero->setCastingShield(false);
//
//	while (!commandQueue.isEmpty()) {
//		sceneGraph.onCommand(commandQueue.pop(), dt);
//	}
//	adaptPlayerVelocity();
//
//	adaptPlayerPositionRelatingBlocks(dt, getCommands());
//	handleCollisions(dt, commandQueue);
//
//	updateStatTexts();
//
//	//updateSounds();
//
//	sceneGraph.removeWrecks();
//
//	sceneGraph.update(dt, getCommands());
//	adaptPlayerPosition();
//	destroyEntitiesOutsideView();
//}
//
//void BattleScene::draw()
//{
//	target.setView(worldView);
//	for (auto t : walkOverTiles) {
//		target.draw(*t);
//	}
//
//	target.draw(sceneGraph);
//
//	for (auto t : blockingTiles) {
//		target.draw(*t);
//	}
//
//	for (auto r : collidingToRedraw) {
//		target.draw(*r);
//	}
//
//	target.draw(*uiGraph);
//}
//
//
//bool BattleScene::hasAlivePlayer() const
//{
//    return hero->getHitpoints() > 0;
//}
//
//void BattleScene::loadTextures()
//{
//	textures.load(TextureID::Floor, "Media/Textures/floor_new.jpg");
//	//textures.load(TextureID::Wall, "Media/Textures/wall300.png");
//
//	textures.load(TextureID::Wall, "Media/Textures/wall_new.jpg");
//
//	textures.load(TextureID::Hero, "Media/Textures/Hero.png");
//	textures.load(TextureID::Archmage, "Media/Textures/Archmage_sheet.png");
//
//	textures.load(TextureID::EnergyBallHero, "Media/Textures/energy_ball_hero.png");
//	textures.load(TextureID::EnergyBallEnemy, "Media/Textures/energy_ball_enemy.png");
//	textures.load(TextureID::ShieldSpell, "Media/Textures/shield_spell_no_glitter.png");
//
//	textures.load(TextureID::HealthDisplay, "Media/Textures/helth_display.png");
//
//	/*textures.load(TextureID::QuestJournal, "Media/Textures/QuestJournal.png");
//	textures.load(TextureID::HintBackground, "Media/Textures/Hint_Action.png");*/
//}
//
//void BattleScene::buildScene()
//{
//	for (std::size_t i = 0; i < LayerCount; ++i) {
//		Category::Type category;
//		switch (i) {
//		case Floor:
//			category = Category::Type::Background;
//		case Walls:
//			category = Category::Type::Wall;
//			break;
//		case PlayerLayer:
//		default:
//			category = Category::Type::None;
//			break;
//		case SpellsLayer:
//			category = Category::Type::SpellLayer;
//			break;
//		}
//
//		SceneNode::Ptr layer(new SceneNode(category));
//		layer.get()->setPosition(400, 300.f);
//
//		sceneLayers[i] = layer.get();
//
//		sceneGraph.attachChild(std::move(layer));
//	}
//
//	walkOverTiles = std::vector<SpriteNode*>();
//	blockingTiles = std::vector<SpriteNode*>();
//	invisibleWallTiles = std::vector<SpriteNode*>();
//
//
//	// prepare background texture
//	sf::Texture& floorTexture = textures.get(TextureID::Floor);
//	sf::Texture& wallTexture = textures.get(TextureID::Wall);
//
//	sf::IntRect floorRect(0, 0, (int)tileData[Tile::Type::Floor].width, (int)tileData[Tile::Type::Floor].height);
//	sf::IntRect wallRect(0, 0, (int)tileData[Tile::Type::Wall].width, (int)tileData[Tile::Type::Wall].height);
//
//	float tileSize = 50.f;
//
//	for (int i = 0; i < currentLevel.size(); ++i) {
//		for (int j = 0; j < currentLevel[0].size(); ++j) {
//
//			SpriteNode* sprite;
//
//			switch (currentLevel[i][j])
//			{
//			case Tile::Type::Floor:
//				sprite = new SpriteNode(floorTexture, floorRect);
//				sprite->setPosition(j * tileSize, i * tileSize);
//				walkOverTiles.push_back(sprite);
//				break;
//			case Tile::Type::Wall:
//				sprite = new SpriteNode(wallTexture, wallRect);
//				sprite->setPosition(j * tileSize, i * tileSize - tileData[Tile::Wall].height + tileSize);
//				blockingTiles.push_back(sprite);
//				break;
//			case Tile::Type::InvisibleWall:
//				sprite = new SpriteNode(wallTexture, wallRect);
//				sprite->setPosition(j * tileSize, i * tileSize - tileData[Tile::Wall].height + tileSize);
//				invisibleWallTiles.push_back(sprite);
//				break;
//			default:
//				break;
//			}
//		}
//	}
//
//	sf::IntRect textureRect(worldBounds);
//
//	std::unique_ptr<Actor> hero_(new Actor(Actor::Type::Hero, textures, fonts));
//	hero = hero_.get();
//	hero->setPosition(spawnPosition);
//	hero->setVelocity(0.f, 0.f);
//	sceneLayers[PlayerLayer]->attachChild(std::move(hero_));
//
//
//	std::unique_ptr<Enemy> enemy_(new Enemy(Actor::Type::Archmage, textures, fonts));
//	enemy_->setPosition(enemySpawnPoint);
//	enemy_->setVelocity(0.f, 0.f);
//
//	enemy = enemy_.get();
//	sceneLayers[PlayerLayer]->attachChild(std::move(enemy_));
//}
//
//void BattleScene::buildUiStats()
//{
//	float margin = 20.f;
//
//	uiGraph = new SceneNode(Category::None);
//
//	std::unique_ptr<UiNode> heroHealth(new UiNode(textures.get(TextureID::HealthDisplay)));
//	heroHealth->setPosition(margin, margin);
//	heroHealth->setVisible(true);
//
//	std::unique_ptr<TextNode> heroStats(new TextNode(fonts, "", 30));
//	heroStats->setString("Hero:\n");
//	//heroStats->setTextColor(sf::Color(125, 120, 186));
//	heroStats->setPosition(heroHealth.get()->getBoundingRect().width / 2, heroHealth.get()->getBoundingRect().height / 2);
//
//	heroStatText = heroStats.get();
//
//	heroHealth->attachChild(std::move(heroStats));
//	uiGraph->attachChild(std::move(heroHealth));
//
//
//
//	std::unique_ptr<UiNode> enemyHealth(new UiNode(textures.get(TextureID::HealthDisplay)));
//	enemyHealth->setPosition(worldView.getSize().x - enemyHealth->getBoundingRect().width - margin, margin);
//	enemyHealth->setVisible(true);
//
//	std::unique_ptr<TextNode> enemyStats(new TextNode(fonts, "", 30));
//	enemyStats->setString("Enemy:\n");
//	//heroStats->setTextColor(sf::Color(125, 120, 186));
//	enemyStats->setPosition(enemyHealth.get()->getBoundingRect().width / 2, enemyHealth.get()->getBoundingRect().height / 2);
//
//	enemyStatsText = enemyStats.get();
//
//	enemyHealth->attachChild(std::move(enemyStats));
//	uiGraph->attachChild(std::move(enemyHealth));
//}
//
//void BattleScene::updateStatTexts()
//{
//	//std::cout << "\nHitpoints: Hero(" << hero->getHitpoints() << "), Enemy (" << enemy->getHitpoints() << ")";
//
//
//	std::string enemyString = "Enemy:\nHP:" + std::to_string( enemy->getHitpoints());
//
//	std::string heroHealth = hero->getFightHealthDisplayString();
//
//	heroStatText->setString(heroHealth);
//	enemyStatsText->setString(enemyString);
//}
//
//void BattleScene::handleCollisions(sf::Time dt, CommandQueue& commands)
//{
//	std::set<SceneNode::Pair> collisionPairs;
//	sceneGraph.checkSceneCollision(sceneGraph, collisionPairs);
//
//	for (auto pair : collisionPairs) {
//		
//		if (matchesCategories(pair, Category::Hero, Category::baseAttackEnemy)) {
//			auto& hero_ = static_cast<Actor&>(*pair.first);
//			auto& bolt = static_cast<EnergyBolt&>(*pair.second);
//
//			if(!hero->isCastingShield())
//				hero_.damage(bolt.getDamage());
//
//			bolt.destroy();
//		}
//
//		if (matchesCategories(pair, Category::FightingNPC, Category::BaseAttackAllied)) {
//			auto& enemy_ = static_cast<Actor&>(*pair.first);
//			auto& bolt = static_cast<EnergyBolt&>(*pair.second);
//
//			enemy_.damage(bolt.getDamage());
//			bolt.destroy();
//		}
//
//		if ( matchesCategories(pair, Category::Hero, Category::FightingNPC)) {
//			adaptHeroPositionRelatingEntity(static_cast<Entity*>(pair.second), dt, commands);
//		}
//	}
//}
//
//void BattleScene::destroyEntitiesOutsideView()
//{
//	Command command;
//	command.category = Category::BaseAttack;
//	command.action = derivedAction<Entity>([this](Entity& e, sf::Time t) {
//		if (!getViewBounds().intersects(e.getBoundingRect())) {
//			e.destroy();
//		}
//		});
//	commandQueue.push(command);
//}
//
//void BattleScene::adaptPlayerPositionRelatingBlocks(sf::Time dt, CommandQueue& commands)
//{
//	size_t deltaTime = 20;
//
//	auto heroCopy = new Actor(Actor::Type::Hero, textures, fonts);
//	heroCopy->setPosition(hero->getPosition());
//	heroCopy->setVelocity(hero->getVelocity());
//
//	heroCopy->update(sf::milliseconds(dt.asMilliseconds() + deltaTime), commands);
//
//	for (auto t : blockingTiles) {
//		if (hero->getBoundingRect().intersects(t->getBoundingRect()) &&
//			hero->getBoundingRect().top + hero->getBoundingRect().height >= t->getBoundingRect().top + t->getBoundingRect().height)
//			collidingToRedraw.push_back(hero);
//
//		if (heroCopy->getBaseTileRect().intersects(t->getBaseTileRect())) {
//			if (hero->getBaseTileRect().intersects(t->getBaseTileRect())) {
//				//hero->accelerate(-1.75 * hero->getVelocity().x, -1.75 * hero->getVelocity().y);
//				adaptPosition(hero, t);
//			}
//			else {
//				hero->setVelocity(0, 0);
//			}
//			break;
//		}
//	}
//
//	for (auto t : invisibleWallTiles) {
//		if (hero->getBoundingRect().intersects(t->getBoundingRect()) &&
//			hero->getBoundingRect().top + hero->getBoundingRect().height >= t->getBoundingRect().top + t->getBoundingRect().height)
//			collidingToRedraw.push_back(hero);
//
//		if (heroCopy->getBaseTileRect().intersects(t->getBaseTileRect())) {
//			if (hero->getBaseTileRect().intersects(t->getBaseTileRect())) {
//				//hero->accelerate(-1.75 * hero->getVelocity().x, -1.75 * hero->getVelocity().y);
//				adaptPosition(hero, t);
//			}
//			else {
//				hero->setVelocity(0, 0);
//			}
//			break;
//		}
//	}
//}
//
//void BattleScene::adaptPlayerPosition()
//{
//	const float borderDistance = 100.f;
//	const float scrollSpeed = 5.f;
//
//	sf::FloatRect viewBounds(worldView.getCenter() - worldView.getSize() / 2.f,
//		worldView.getSize());
//
//	// worldView
//	if (hero->getPosition().x <= viewBounds.left + borderDistance)
//		worldView.move(-scrollSpeed, 0);
//	if (hero->getPosition().x + hero->getBoundingRect().width >= viewBounds.left + viewBounds.width - borderDistance)
//		worldView.move(scrollSpeed, 0);
//	if (hero->getPosition().y <= viewBounds.top + borderDistance)
//		worldView.move(0, -scrollSpeed);
//	if (hero->getPosition().y + hero->getBoundingRect().height >= viewBounds.top + viewBounds.height - borderDistance)
//		worldView.move(0, scrollSpeed);
//
//	uiGraph->setPosition(viewBounds.left, viewBounds.top);
//	//std::cout << "\nHero position: " << hero->getWorldPoition().x << ", " << hero->getWorldPoition().y;
//	//std::cout << "\nHero state world position: " << heroStatText->getWorldPoition().x << ", " << heroStatText->getWorldPoition().y;
//}
//
//void BattleScene::guideEnergyBolts()
//{
//	Command heroBoltGuider;
//	heroBoltGuider.category = Category::BaseAttackAllied;
//	heroBoltGuider.action = derivedAction<EnergyBolt>([this](EnergyBolt& missile, sf::Time dt) {
//		missile.guideTowards(enemy->getWorldPoition());
//		});
//
//	Command enemyBoltGuider;
//	enemyBoltGuider.category = Category::baseAttackEnemy;
//	enemyBoltGuider.action = derivedAction<EnergyBolt>([this](EnergyBolt& missile, sf::Time dt) {
//		missile.guideTowards(hero->getWorldPoition());
//		});
//
//	commandQueue.push(heroBoltGuider);
//	commandQueue.push(enemyBoltGuider);
//}