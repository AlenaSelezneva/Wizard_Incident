#include "Enemy.h"
#include "Utility.h"
#include "EnergyBolt.h"

Enemy::Enemy(Actor::Type type, const TextureHolder_t& textures, const FontHolder_t& fonts)
	: Actor(type, textures, fonts)
	, FightingCharacter(FightingCharacter::Type::Enemy)
{
	hitPoints = 150;

	state_ = Actor::State::SpellCastFront;

	fireCommand.category = Category::SpellLayer;
	fireCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		this->createEnergyBolt(node, textures);
	};
}

Enemy::Enemy(Actor* actor)
	:Actor(actor->getType(), actor->getTextures(), actor->getFonts())
	, FightingCharacter(FightingCharacter::Type::Enemy)
{
	this->setPosition(actor->getPosition());
	this->setState(actor->getState());
	//this->setVelocity(actor->getVelocity());
	this->setVelocity(0.f, 0.f);

	hitPoints = 150;

	fireCommand.category = Category::SpellLayer;
	fireCommand.action = [this](SceneNode& node, sf::Time)
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
	std::unique_ptr<EnergyBolt> bolt(new EnergyBolt(EnergyBolt::Type::EnemyBolt, 30, textures));


	sf::Vector2f offset(0.f, -20.f);
	sf::Vector2f velocity(0.f, 0.f);

	float velocity_ = 20.f;

	//switch (direction_) {
	//case Direction::Back:
	//	offset = sf::Vector2f(-15.f, -30.f);
	//	velocity = sf::Vector2f(0.f, -velocity_);
	//	break;
	//case Direction::Front:
	//	offset = sf::Vector2f(15.f, -10.f);
	//	velocity = sf::Vector2f(0.f, velocity_);
	//	break;
	//case Direction::Left:
	//	offset = sf::Vector2f(-35.f, -10.f);
	//	velocity = sf::Vector2f(-velocity_, 0.f);
	//	break;
	//case Direction::Right:
	//	offset = sf::Vector2f(35.f, -30.f);
	//	velocity = sf::Vector2f(velocity_, 0.f);
	//	break;
	//}

	bolt->setPosition(getWorldPoition() + offset);
	bolt->setVelocity(velocity);

	node.attachChild(std::move(bolt));
}

void Enemy::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	//Actor::updateCurrent(dt, commands);
	//state_ = Actor::State::SpellCastFront;
	
	//updateDirections();
	//updateStates();

	attack();
	if (isAttacking_) {
		playLocalSound(commands, EffectID::EnemyAttack);
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
	state_ = Actor::State::SpellCastFront;

	Entity::updateCurrent(dt, commands);

	auto rec = animations_.at(state_).update(dt);

	sprite_.setTextureRect(rec);
	centerOrigin(sprite_);

	checkCastingAttackingSpell(dt, commands);
}
