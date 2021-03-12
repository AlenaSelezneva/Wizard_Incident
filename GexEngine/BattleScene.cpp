#include "BattleScene.h"

BattleScene::BattleScene(sf::RenderTarget& window, const FontHolder_t& fonts, SoundPlayer& sounds, PlayerData* data)
{
}

void BattleScene::update(sf::Time dt)
{
}

void BattleScene::draw()
{
}

CommandQueue& BattleScene::getCommands()
{
    // TODO: insert return statement here
}

bool BattleScene::hasAlivePlayer() const
{
    return false;
}

void BattleScene::loadTextures()
{
}

void BattleScene::buildScene()
{
}

void BattleScene::handleCollisions(sf::Time dt, CommandQueue& commands)
{
}

void BattleScene::destroyEntitiesOutsideView()
{
}

sf::FloatRect BattleScene::getBattlefieldBounds() const
{
    return sf::FloatRect();
}
