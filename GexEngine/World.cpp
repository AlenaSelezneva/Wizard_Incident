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
#include "EnergyBolt.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <iostream>
#include "SoundNode.h"
#include "MusicPlayer.h"


World::World(sf::RenderTarget& outputTarget, const FontHolder_t& fonts, SoundPlayer& sounds, MusicPlayer& music, PlayerData* data)
	: target(outputTarget)
	, sceneTexture()
	, worldView(outputTarget.getDefaultView())
	, textures()
	, fonts(fonts)
	, sounds(sounds)
	, music(music)
	, playerData(data)
	, sceneGraph()
	, uiGraph()
	, sceneLayers()
	, commandQueue()
	, currentLevel(Level::First)
	, worldBounds(0.f, 0.f, 1500.f, 1500.f)
	, spawnPosition(worldView.getSize().x / 2.f, worldBounds.height - worldView.getSize().y / 2.f)
{
	sceneTexture.create(target.getSize().x, target.getSize().y);
	loadTextures();

	worldView.setCenter(spawnPosition);

	//currentLevelLayout = LevelsTilesSchema::getLevelLayout(currentLevel);
	tileData = initializeTileData();

	buildScene();
}

CommandQueue& World::getCommands()
{
	return commandQueue;
}

bool World::hasAlivePlayer() const
{
	return hero != nullptr && hero->getHitpoints() > 0;
}

void World::moveToLevel(Level l)
{
	currentLevel = l;

	sceneGraph.clearChildren();
	sceneLayers = {};

	for (std::size_t i = 0; i < LayerCount; ++i) {
		Category::Type category;
		switch (i) {
		case Floor:
			category = Category::Type::Background;
		case Walls:
			category = Category::Type::Wall;
			break;
		case SpellsLayer:
			category = Category::Type::SpellLayer;
			break;
		case PlayerLayer:
		default:
			category = Category::Type::None;
			break;
		}

		SceneNode::Ptr layer(new SceneNode(category));

		sceneLayers[i] = layer.get();

		sceneGraph.attachChild(std::move(layer));
	}

	//npcs = std::vector< FriendlyNPC*>();
	enemy = nullptr;

	std::unique_ptr<SoundNode> soundNode(new SoundNode(sounds));
	sceneGraph.attachChild(std::move(soundNode));

	buildLevel();
	buildLevelObjects();

	sf::IntRect textureRect(worldBounds);

	std::unique_ptr<SpriteNode> portal_(new SpriteNode(textures.get(TextureID::Portal)));
	/*portal_->setPosition(hero->getPosition().x - 600.f, hero->getPosition().y - 800.f);*/
	portal_->setPosition(spawnPosition.x + getLevelportalData().at(currentLevel).x, spawnPosition.y + getLevelportalData().at(currentLevel).y);
	portal = portal_.get();
	sceneLayers[PlayerLayer]->attachChild(std::move(portal_));


	std::unique_ptr<Hero> hero_(new Hero(textures, fonts, playerData));
	hero = hero_.get();
	//hero->setPosition(spawnPosition);
	hero->setVelocity(0.f, 0.f);
	sceneLayers[PlayerLayer]->attachChild(std::move(hero_));

	hero->setPosition(portal->getPosition().x + portal->getBoundingRect().width / 2,
		portal->getPosition().y + portal->getBoundingRect().height + 40.f);

	hero->playLocalSound(commandQueue, EffectID::Portal);
}

void World::startFight(Actor::Type type)
{
	Actor* en = nullptr;
	for (int i = 0; i < npcs.size(); ++i) {
		if (npcs[i]->getType() == type) {
			en = npcs[i];
			//npcs[i] = nullptr;
			npcs.erase(npcs.begin() + i);
		}
	}

	if (en == nullptr)
		return;

	//playerData->setInFightState(true);
	playerData->startFight();

	std::unique_ptr<Enemy> enemyPtr(new Enemy(en));
	sceneLayers[PlayerLayer]->detachChild(*en);

	enemy = enemyPtr.get();

	sceneLayers[PlayerLayer]->attachChild(std::move(enemyPtr));

	buildFightingUiStats();
	music.play(MusicID::FightTheme);
}

void World::stopFight()
{
	hero->playLocalSound(commandQueue, EffectID::Dadadedada);
	//playerData->setInFightState(false);
	playerData->finishFight(enemy->getType());
	fightingUiGraph = nullptr;
	hero->finishFight();

	Actor* a = enemy;

	std::unique_ptr<FriendlyNPC> npc(new FriendlyNPC(a));
	sceneLayers[PlayerLayer]->detachChild(*enemy);
	enemy = nullptr;

	npcs.push_back(npc.get());
	sceneLayers[PlayerLayer]->attachChild(std::move(npc));

	Command destroySpells;
	destroySpells.category = Category::BaseAttack;
	destroySpells.action = derivedAction<EnergyBolt>([this](EnergyBolt& bolt, sf::Time dt) {
		if (enemy != nullptr)
			bolt.destroy();
		});

	commandQueue.push(destroySpells);

	//buildUiGraph();
	music.play(MusicID::GameTheme);
}

Level World::getCurrentLevel()
{
	return currentLevel;
}


void World::update(sf::Time dt) {
	
	// reset player velocity
	hero->setVelocity(0.f, 0.f);
	hero->setSpellCasting(false);
	hero->setCastingShield(false);
	resetNPCsCanTalk();
	playerData->setIntersectsWithPortal(false);
	hintView->setVisible(false);
	hintText->setString("Press ENTER to interact");
	collidingToRedraw = std::list<SceneNode*>();

	guideEnergyBolts();

	while (!commandQueue.isEmpty()) {
		sceneGraph.onCommand(commandQueue.pop(), dt);
	}
	adaptPlayerVelocity();

	adaptPlayerPositionRelatingBlocks(dt, getCommands());
	handleCollisions(dt, commandQueue);

	updateSounds();

	//if (playerData->isInFightState() && enemy != nullptr && enemy->getHitpoints() <= 0) {
	//	stopFight();
	//}

	if (playerData->isInFightState() && enemy == nullptr) {
		startFight(playerData->getCurrentActor());
	}

	if (!playerData->isInFightState()) {
		updateCasualUiElements();
	}
	else {
		updateFightingStatTexts();
	}

	sceneGraph.removeWrecks();

	sceneGraph.update(dt, getCommands());
	adaptPlayerPosition();

	destroyEntitiesOutsideView();
}

void World::updateSounds()
{
	sounds.setListenerPosition(hero->getWorldPoition());
	sounds.removeStoppedSounds();
}

void World::updateCasualUiElements()
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

	if (playerData->isInFightState() && fightingUiGraph != nullptr)
		target.draw(*fightingUiGraph);
	else 
		target.draw(*uiGraph);
}

void World::loadTextures() {
	//textures.load(TextureID::Floor, "Media/Textures/Floor50.png");
	textures.load(TextureID::Floor, "Media/Textures/floor_new.jpg");
	//textures.load(TextureID::Wall, "Media/Textures/wall300.png");

	textures.load(TextureID::Wall, "Media/Textures/wall_new.jpg");
	

	textures.load(TextureID::Hero, "Media/Textures/Hero.png");
	textures.load(TextureID::Elony, "Media/Textures/elony.png");
	textures.load(TextureID::Archmage, "Media/Textures/Archmage_sheet.png");
	textures.load(TextureID::MisteriousFigure, "Media/Textures/summoning_figure.png");

	textures.load(TextureID::BookShelf, "Media/Textures/book_shelf.png");

	textures.load(TextureID::QuestJournal, "Media/Textures/QuestJournal.png");
	textures.load(TextureID::HintBackground, "Media/Textures/Hint_Action.png");

	/*textures.load(TextureID::Portal, "Media/Textures/portal.png");*/
	textures.load(TextureID::Portal, "Media/Textures/portal_Small.png");

	

	textures.load(TextureID::HealthDisplay, "Media/Textures/helth_display.png");

	textures.load(TextureID::ShieldSpell, "Media/Textures/shield_spell_no_glitter.png");
	textures.load(TextureID::EnergyBallHero, "Media/Textures/energy_ball_hero.png");
	textures.load(TextureID::EnergyBallEnemy, "Media/Textures/energy_ball_enemy.png");
	textures.load(TextureID::PowerAttack, "Media/Textures/hero_attack_2.png");
	
}

void World::buildScene() {

	moveToLevel(Level::First);

	buildUiGraph();
}

void World::buildLevel()
{
	music.play(MusicID::GameTheme);

	walkOverTiles = std::vector<SpriteNode*>();
	blockingTiles = std::vector<SpriteNode*>();
	invisibleWallTiles = std::vector<SpriteNode*>();

	sf::Texture& floorTexture = textures.get(TextureID::Floor);
	sf::Texture& wallTexture = textures.get(TextureID::Wall);

	sf::IntRect floorRect(0, 0, (int)tileData[Tile::Type::Floor].width, (int)tileData[Tile::Type::Floor].height);
	sf::IntRect wallRect(0, 0, (int)tileData[Tile::Type::Wall].width, (int)tileData[Tile::Type::Wall].height);

	float tileSize = 50.f;

	auto currentLevelLayout = LevelsTilesSchema::getLevelLayout(currentLevel);

	for (int i = 0; i < currentLevelLayout.size(); ++i) {
		for (int j = 0; j < currentLevelLayout[0].size(); ++j) {

			SpriteNode* sprite;

			switch (currentLevelLayout[i][j])
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
}

void World::buildLevelObjects()
{
	auto actors = getLevelActorData().at(currentLevel);
	auto objects = getLevelObjectData().at(currentLevel);

	npcs = std::vector<FriendlyNPC* >();

	for (auto a : actors) {
		std::unique_ptr<FriendlyNPC> actor(new FriendlyNPC(a.type, a.questType, textures, fonts));
		actor->setPosition(spawnPosition.x + a.x, spawnPosition.y + a.y);
		actor->setVelocity(0.f, 0.f);
		npcs.push_back(actor.get());
		sceneLayers[PlayerLayer]->attachChild(std::move(actor));
	}

	for (auto o : objects) {

		std::unique_ptr<InteractableObject> object(new InteractableObject(o.type, o.questType, textures, fonts));
		object->setPosition(spawnPosition.x + o.x, spawnPosition.y + o.y);
		sceneLayers[PlayerLayer]->attachChild(std::move(object));
	}
}

void World::buildQuestView()
{
	float margin = 20.f;

	uiGraph = new SceneNode(Category::None);

	std::unique_ptr<UiNode> qJournalBackground(new UiNode(textures.get(TextureID::QuestJournal)));
	qJournalBackground->setPosition(margin, 150.f);
	questsView = qJournalBackground.get();

	std::unique_ptr<TextNode> questHeaderNode(new TextNode(fonts, "", 30));
	questHeaderNode->setString("Quests\n");
	questHeaderNode->setTextColor(sf::Color(125, 120, 186));

	questHeaderNode->setPosition(qJournalBackground.get()->getBoundingRect().width / 2, 80.f);

	std::unique_ptr<TextNode> questTextNode(new TextNode(fonts, "", 16));
	questTextNode->setString("Some Quest: Do this");
	questTextNode->setTextColor(sf::Color(125, 120, 186));
	questTextNode->setPosition(qJournalBackground.get()->getBoundingRect().width / 2, qJournalBackground.get()->getBoundingRect().height / 2);
	questLog = questTextNode.get();

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
	hintView->setVisible(false);

	std::unique_ptr<TextNode> hint(new TextNode(fonts, "", 20));
	hint->setString("Press ENTER to interact");
	//questHeaderNode->setTextColor(sf::Color(125, 120, 186));

	hint->setPosition(hintBackground.get()->getBoundingRect().width / 2, hintBackground.get()->getBoundingRect().height / 2);
	hintText = hint.get();

	hintBackground->attachChild(std::move(hint));

	uiGraph->attachChild(std::move(hintBackground));
}



void World::buildUiGraph()
{
	buildQuestView();
	buildHintView();
	//buildHeroStatsView();
}

void World::buildFightingUiStats()
{
	float margin = 20.f;

	fightingUiGraph = new SceneNode(Category::None);

	std::unique_ptr<UiNode> heroHealth(new UiNode(textures.get(TextureID::HealthDisplay)));
	heroHealth->setPosition(margin, margin);
	heroHealth->setVisible(true);

	std::unique_ptr<TextNode> heroStats(new TextNode(fonts, "", 30));
	heroStats->setString("Hero:\n");
	heroStats->setPosition(heroHealth.get()->getBoundingRect().width / 2, heroHealth.get()->getBoundingRect().height / 2);

	heroStatText = heroStats.get();

	heroHealth->attachChild(std::move(heroStats));
	fightingUiGraph->attachChild(std::move(heroHealth));



	std::unique_ptr<UiNode> enemyHealth(new UiNode(textures.get(TextureID::HealthDisplay)));
	enemyHealth->setPosition(worldView.getSize().x - enemyHealth->getBoundingRect().width - margin, margin);
	enemyHealth->setVisible(true);

	std::unique_ptr<TextNode> enemyStats(new TextNode(fonts, "", 30));
	enemyStats->setString("Enemy:\n");
	enemyStats->setPosition(enemyHealth.get()->getBoundingRect().width / 2, enemyHealth.get()->getBoundingRect().height / 2);

	enemyStatsText = enemyStats.get();

	enemyHealth->attachChild(std::move(enemyStats));
	fightingUiGraph->attachChild(std::move(enemyHealth));
}


void World::handleCollisions(sf::Time dt, CommandQueue& commands)
{
	std::set<SceneNode::Pair> collisionPairs;
	sceneGraph.checkSceneCollision(sceneGraph, collisionPairs);

	if (hero->getBoundingRect().intersects(portal->getBoundingRect())) {
		hintView->setVisible(true);
		playerData->setIntersectsWithPortal(true);
	}

	for (auto pair : collisionPairs) {

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
			}
			else {
				auto& obj = static_cast<InteractableObject&>(*pair.second);

				if (playerData->hasPendingQuest(obj.getQuestObjectType())) {
					goOnQuest = true;
					playerData->setCurrentQuestDialog(obj.getQuestObjectType());
					hintView->setVisible(true);
				}

				adaptHeroPositionRelatingEntity(static_cast<Entity*>(pair.second), dt, commands);
			}
		}

		if (!goOnQuest && matchesCategories(pair, Category::Hero, Category::TalkingNPC)) {
			auto& npc = static_cast<FriendlyNPC&>(*pair.second);
			npc.setCanTalkToHero(true);

			playerData->setCurrentDialog(npc.getType());

			auto& hero = static_cast<Actor&>(*pair.first);

			adaptHeroPositionRelatingEntity(static_cast<Entity*>(pair.second), dt, commands);

			hintText->setString("Press ENTER to talk");
			hintView->setVisible(true);
		}

		if (matchesCategories(pair, Category::Hero, Category::baseAttackEnemy)) {
			auto& hero_ = static_cast<Actor&>(*pair.first);
			auto& bolt = static_cast<EnergyBolt&>(*pair.second);
				
			if (!hero->isCastingShield()) {
				hero_.damage(bolt.getDamage());
				hero_.playLocalSound(commands, EffectID::HeroHurt);
			}
			else {
				hero_.playLocalSound(commands, EffectID::AttackBlocked);
			}
				
			bolt.destroy();
		}
				
		if (matchesCategories(pair, Category::FightingNPC, Category::BaseAttackAllied)) {
			auto& enemy_ = static_cast<Actor&>(*pair.first);
			auto& bolt = static_cast<EnergyBolt&>(*pair.second);
				
			enemy_.damage(bolt.getDamage());
			enemy_.playLocalSound(commands, EffectID::EnemyHurt);
			bolt.destroy();

			if (enemy->getHitpoints() <= 0)
				stopFight();
		}
	}	
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
	command.category = Category::AttackingSpell;
	command.action = derivedAction<Entity>([this](Entity& e, sf::Time t) {
		if (!getViewBounds().intersects(e.getBoundingRect())) {
			e.destroy();
		}
		});
	commandQueue.push(command);
}

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

	if (playerData->isInFightState() && fightingUiGraph != nullptr)
		fightingUiGraph->setPosition(viewBounds.left, viewBounds.top);
	else
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

void World::updateFightingStatTexts()
{
	if (fightingUiGraph == nullptr || enemy == nullptr)
		return;

	std::string enemyString = "Enemy:\nHP:" + std::to_string(enemy->getHitpoints());
	std::string heroHealth = hero->getFightHealthDisplayString();

	heroStatText->setString(heroHealth);
	enemyStatsText->setString(enemyString);
}

void World::resetNPCsCanTalk()
{
	playerData->setCurrentDialog(Actor::Type::None);
}

sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(worldView.getCenter() - worldView.getSize() / 2.f, worldView.getSize());
}

void World::guideEnergyBolts()
{
	Command heroBoltGuider;
	heroBoltGuider.category = Category::BaseAttackAllied;
	heroBoltGuider.action = derivedAction<EnergyBolt>([this](EnergyBolt& bolt, sf::Time dt) {
		if (enemy != nullptr)
			bolt.guideTowards(enemy->getWorldPoition());
		});

	Command enemyBoltGuider;
	enemyBoltGuider.category = Category::baseAttackEnemy;
	enemyBoltGuider.action = derivedAction<EnergyBolt>([this](EnergyBolt& bolt, sf::Time dt) {
		bolt.guideTowards(hero->getWorldPoition());
		});

	Command collideWithWalls;
	collideWithWalls.category = Category::AttackingSpell;
	collideWithWalls.action = derivedAction<EnergyBolt>([this](EnergyBolt& bolt, sf::Time dt) {
		for (auto wall : blockingTiles) {
			if (bolt.getBoundingRect().intersects(wall->getBoundingRect())) {
				bolt.destroy();
			}
		}
	});

	commandQueue.push(heroBoltGuider);
	commandQueue.push(enemyBoltGuider);
	commandQueue.push(collideWithWalls);
}
