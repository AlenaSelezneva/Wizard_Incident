
#pragma once

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "SceneNode.h"
#include "SpriteNode.h"
#include "CommandQueue.h"
#include "Command.h"
#include "SoundPlayer.h"
#include "Actor.h"
#include "LevelsTilesSchema.h"
#include "DataTables.h"
#include "PlayerData.h"
#include "TextNode.h"
#include "UiNode.h"
#include "Hero.h"
#include "Enemy.h"
#include "Level.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include <array>

namespace sf {
	class RenderTarget;
}

class World : private sf::NonCopyable {

public:
	explicit				World(sf::RenderTarget& window,
								const FontHolder_t& fonts,
								SoundPlayer& sounds, PlayerData* data);

	explicit				World(sf::RenderTarget& window,
								const FontHolder_t& fonts,
								SoundPlayer& sounds, PlayerData* data, int n);

	void					update(sf::Time dt);
	void					draw();

	CommandQueue&			getCommands();

	bool					hasAlivePlayer() const;

protected:
	virtual void			loadTextures();
	void					buildScene();
	void					buildLevel();
	void					buildLevelObjects();

	void					handleCollisions(sf::Time dt, CommandQueue& commands);
	bool					matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2);
	
	void					destroyEntitiesOutsideView();

	void					adaptPlayerVelocity();
	void					adaptPlayerPosition();

	void					adaptPlayerPositionRelatingBlocks(sf::Time dt, CommandQueue& commands);
	void					adaptPosition(Entity* ent1, SceneNode* sceneNode);
	void					adaptHeroPositionRelatingEntity(Entity* ent2, sf::Time dt, CommandQueue& commands);
	void					updateSounds();

	sf::FloatRect			getViewBounds() const;

	void					guideEnergyBolts();

private:

	void					buildQuestView();
	void					buildHintView();
	void					buildHeroStatsView();
	void					buildUiGraph();
	void					buildFightingUiStats();

	void					updateCasualUiElements();
	void					updateFightingStatTexts();

	//void					adaptNPCPosition();
	void					resetNPCsCanTalk();


	//sf::FloatRect			getBattlefieldBounds() const;

protected:
	enum Layer
	{
		Floor,
		Walls,
		PlayerLayer,
		SpellsLayer,
		LayerCount
	};

	/*struct SpawnPoint {
		SpawnPoint(Actor::Type type, float x, float y)
			:type(type), x(x), y(y) {}

		Actor::Type type;
		float x, y;
	};*/


protected:
	sf::RenderTarget&					target;
	sf::RenderTexture					sceneTexture;
	sf::View							worldView;
	const FontHolder_t&					fonts;
	SoundPlayer&						sounds;

	SceneNode							sceneGraph;

	std::vector< SpriteNode*>			walkOverTiles;
	std::vector< SpriteNode*>			blockingTiles;
	std::vector< SpriteNode*>			invisibleWallTiles;

	std::list<SceneNode*>				collidingToRedraw;

	// old school array instead of vector
	std::array<SceneNode*, LayerCount>	sceneLayers;
	CommandQueue						commandQueue;

	Level								currentLevel;

	Hero*								hero;
	Enemy*								enemy;

	sf::FloatRect						worldBounds;
	sf::Vector2f						spawnPosition;
	float								scrollSpeed;

	//std::vector<std::vector<Tile::Type> > currentLevelLayout;
	std::map<Tile::Type, TileData>		tileData;

	PlayerData*							playerData;


private:
	TextureHolder_t						textures;
	SpriteNode*							portal;

	UiNode*								questsView;
	UiNode*								hintView;
	UiNode*								heroAttributesView;

	TextNode*							questLog;
	TextNode*							hintText;

	SceneNode* uiGraph;
	SceneNode*							fightingUiGraph;

	UiNode*								heroStatsView;
	UiNode*								enemyStatsView;

	TextNode*							heroStatText;
	TextNode*							enemyStatsText;
	TextNode*							heroAttributesText;
};
