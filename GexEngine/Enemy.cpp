#include "Enemy.h"
#include "Utility.h"
#include "EnergyBolt.h"

Enemy::Enemy(Actor::Type type, const TextureHolder_t& textures, const FontHolder_t& fonts)
	: Actor(type, textures, fonts)
	, FightingCharacter(FightingCharacter::Type::Enemy)
{
	fireCommand.category = Category::SpellLayer;
	fireCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		this->createEnergyBolt(node, textures);
	};
}

unsigned int Enemy::getCategory() const
{
	return Category::FightingNPC;
}

void Enemy::createEnergyBolt(SceneNode& node, const TextureHolder_t& textures) const
{
	std::unique_ptr<EnergyBolt> bolt(new EnergyBolt(EnergyBolt::Type::EnemyBolt, 10, textures));

	sf::Vector2f offset(sprite_.getGlobalBounds().width,
		sprite_.getGlobalBounds().height);


	bolt->setPosition(getWorldPoition() + offset);
	//bolt->setPosition(getWorldPoition());
	bolt->setVelocity(0.f, -40.f);

	node.attachChild(std::move(bolt));
}

void Enemy::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	Actor::updateCurrent(dt, commands);
	
	attack();
	checkCastingAttackingSpell(dt, commands);

	if (isAttacking_) {
		switch (direction_)
		{
			case Actor::Direction::Back:
				state_ = Actor::State::SpellCastBack;
				break;
			case Actor::Direction::Front:
				state_ = Actor::State::SpellCastFront;
				break;
			case Actor::Direction::Left:
				state_ = Actor::State::SpellCastLeft;
				break;
			case Actor::Direction::Right:
				state_ = Actor::State::SpellCastRight;
				break;
		default:
			break;
		}
	}
}
