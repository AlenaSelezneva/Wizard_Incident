//#include "World.h"
//#include "Enemy.h"
//
//#pragma once
//class BattleScene : public World
//{
//public:
//	explicit				BattleScene(sf::RenderTarget& window,
//									const FontHolder_t& fonts,
//									SoundPlayer& sounds, PlayerData* data);
//
//	void					update(sf::Time dt);
//	void					draw();
//
//	bool					hasAlivePlayer() const;
//
//	void					guideEnergyBolts();
//
//protected:
//	void					loadTextures() override;
//	void					buildScene();
//
//	void					buildUiStats();
//
//	void					updateStatTexts();
//
//	void					handleCollisions(sf::Time dt, CommandQueue& commands);
//
//
//private:
//	TextureHolder_t			textures;
//
//	sf::Vector2f			enemySpawnPoint;
//
//	Enemy*					enemy;
//
//
//
//private:
//	SceneNode* uiGraph;
//
//	UiNode* heroStatsView;
//	UiNode* enemyStatsView;
//
//	TextNode* heroStatText;
//	TextNode* enemyStatsText;
//
//
//};
//
