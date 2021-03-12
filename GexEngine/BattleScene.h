#include "World.h"
#include "Enemy.h"

#pragma once
class BattleScene : public World
{
public:
	explicit				BattleScene(sf::RenderTarget& window,
									const FontHolder_t& fonts,
									SoundPlayer& sounds, PlayerData* data);

	void					update(sf::Time dt);
	void					draw();

	bool					hasAlivePlayer() const;

	void					guideEnergyBolts();

protected:
	void					loadTextures() override;
	void					buildScene();

	void					handleCollisions(sf::Time dt, CommandQueue& commands);
	//bool					matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2);
	void					destroyEntitiesOutsideView();

	//void					updateSounds();

	void					adaptPlayerPosition();
	void					adaptPlayerPositionRelatingBlocks(sf::Time dt, CommandQueue& commands);
	//void					adaptPosition(Entity* ent1, SceneNode* sceneNode);
	//void					adaptHeroPositionRelatingEntity(Entity* ent2, sf::Time dt, CommandQueue& commands);

private:
	TextureHolder_t			textures;

	sf::Vector2f			enemySpawnPoint;

	Enemy*					enemy;

	//sf::FloatRect			getBattlefieldBounds() const;

};

