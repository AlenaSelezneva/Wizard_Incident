#include "Enemy.h"
#include "Utility.h"
#include "EnergyBolt.h"

Enemy::Enemy(Type type, const TextureHolder_t& textures, const FontHolder_t& fonts)
	: Actor(type, textures, fonts)
{
	fireCommand.category = Category::FightingNPC;
	fireCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		this->createEnergyBolt(node, textures);
	};

}

unsigned int Enemy::getCategory() const
{
	return Category::FightingNPC;
}

void Enemy::attack()
{
	if (attackingCountDown <= sf::Time::Zero) {
		isAttacking_ = true;
		attackingCountDown = ATTACKING_INTERVAL;
	}
}


void Enemy::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	Actor::updateCurrent(dt, commands);

	spellcasting_ = true;
	checkCastingAttackingSpell(dt, commands);
}

void Enemy::checkCastingAttackingSpell(sf::Time dt, CommandQueue& commands)
{
	attack();

	if (isAttacking_) {
		commands.push(fireCommand);
	}
	else if (attackingCountDown > sf::Time::Zero) {
		attackingCountDown -= dt;
	}
	isAttacking_ = false;
}

void Enemy::createEnergyBolt(SceneNode& node, const TextureHolder_t& textures) const
{
	std::unique_ptr<EnergyBolt> bolt(new EnergyBolt(EnergyBolt::Type::EnemyBolt, 10, textures));

	bolt->setPosition(0.f, 0.f);
	//bolt->setPosition(getWorldPoition());
	bolt->setVelocity(0.f, 0.f);

	node.attachChild(std::move(bolt));
}