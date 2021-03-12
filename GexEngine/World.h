
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
	void					update(sf::Time dt);
	void					draw();

	CommandQueue&			getCommands();

	bool					hasAlivePlayer() const;
	bool					hasPlayerReachedEnd() const;

private:
	void					loadTextures();
	void					buildScene();
	void					buildQuestView();
	void					buildHintView();

	void					addEnemies();
	void					addEnemy(Actor::Type type, float relX, float relY);
	void					handleCollisions(sf::Time dt, CommandQueue& commands);
	bool					matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2);
	void					destroyEntitiesOutsideView();

	void					adaptPlayerVelocity();
	void					adaptPlayerPosition();
	void					adaptPlayerPositionRelatingBlocks(sf::Time dt, CommandQueue& commands);
	void					adaptPosition(Entity* ent1, SceneNode* sceneNode);
	void					adaptHeroPositionRelatingEntity(Entity* ent2, sf::Time dt, CommandQueue& commands);
	void					updateSounds();
	void					updateUiElements();

	void					adaptNPCPosition();
	void					resetNPCsCanTalk();

	sf::FloatRect			getViewBounds() const;
	sf::FloatRect			getBattlefieldBounds() const;

private:
	enum Layer
	{
		Floor,
		Walls,
		PlayerLayer,
		SpellsLayer,
		LayerCount
	};

	struct SpawnPoint {
		SpawnPoint(Actor::Type type, float x, float y)
			:type(type), x(x), y(y) {}

		Actor::Type type;
		float x, y;
	};

private:
	sf::RenderTarget&					target;
	sf::RenderTexture					sceneTexture;
	sf::View							worldView;
	TextureHolder_t						textures;
	const FontHolder_t&					fonts;
	SoundPlayer&						sounds;

	SceneNode							sceneGraph;
	SceneNode*							uiGraph;

	std::vector< SpriteNode*>			walkOverTiles;
	std::vector< SpriteNode*>			blockingTiles;
	std::vector< SpriteNode*>			invisibleWallTiles;

	std::list<SceneNode*>				collidingToRedraw;

	// old school array instead of vector
	std::array<SceneNode*, LayerCount>	sceneLayers;
	CommandQueue						commandQueue;

	Actor*								hero;

	sf::FloatRect						worldBounds;
	sf::Vector2f						spawnPosition;
	float								scrollSpeed;

	std::vector<std::vector<Tile::Type> > currentLevel;
	std::map<Tile::Type, TileData>		tileData;

	std::string							currentDialog;
	PlayerData*							playerData;

	UiNode*								questsView;
	UiNode*								hintView;

	TextNode*							questLog;
	TextNode*							hintText;
};
