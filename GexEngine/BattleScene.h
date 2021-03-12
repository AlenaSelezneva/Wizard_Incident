#include "World.h"

#pragma once
class BattleScene : public World
{
public:
	explicit				BattleScene(sf::RenderTarget& window,
									const FontHolder_t& fonts,
									SoundPlayer& sounds, PlayerData* data);

	void					update(sf::Time dt);
	void					draw();

	CommandQueue&			getCommands();

	bool					hasAlivePlayer() const;

protected:
	void					loadTextures();
	void					buildScene();

	void					handleCollisions(sf::Time dt, CommandQueue& commands);
	//bool					matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2);
	void					destroyEntitiesOutsideView();

	//void					updateSounds();

private:
	sf::FloatRect			getBattlefieldBounds() const;

};

