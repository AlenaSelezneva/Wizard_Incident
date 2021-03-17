#include "Hero.h"
#include "UiNode.h"
#include "Utility.h"

Hero::Hero(const TextureHolder_t& textures, const FontHolder_t& fonts)
	:Actor(Actor::Type::Hero, textures, fonts)
	, FightingCharacter(FightingCharacter::Type::Hero)
{
	fireCommand.category = Category::Hero;
	fireCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		this->createEnergyBolt(node, textures);
	};

	std::unique_ptr<UiNode> shield_(new UiNode(textures.get(TextureID::ShieldSpell)));
	shield_->setPosition(-50.f, -50.f);
	shield_->setVisible(false);
	shield = shield_.get();

	attachChild(std::move(shield_));
}


bool Hero::isSpellCasting()
{
	return isSpellcasting_;
}

void Hero::setSpellCasting(bool b)
{
	isSpellcasting_ = b;
}

bool Hero::isCastingShield() const
{
	return isCastingShield_;
}

void Hero::setCastingShield(bool b)
{
	if (isAttacking_) {
		isCastingShield_ = false;
		return;
	}

	isCastingShield_ = b;

	if (isCastingShield_ == true) {
		isSpellcasting_ = true;
	}
}

void Hero::updateStates()
{
	if (isCastingShield_ || isAttacking_ || isSpellcasting_) {
		switch (direction_)
		{
		case Direction::Front:
			state_ = Actor::State::SpellCastFront;
			break;
		case Direction::Back:
			state_ = Actor::State::SpellCastBack;
			break;
		case Direction::Right:
			state_ = Actor::State::SpellCastRight;
			break;
		case Direction::Left:
			state_ = Actor::State::SpellCastLeft;
			break;
		default:
			break;
		}
		return;
	}

	float delta = 0.01f;
	if (abs(getVelocity().x) < delta && abs(getVelocity().y) <= delta) {
		switch (direction_)
		{
		case Direction::Front:
			state_ = Actor::State::IdleFront;
			break;
		case Direction::Back:
			state_ = Actor::State::IdleBack;
			break;
		case Direction::Right:
			state_ = Actor::State::IdleRight;
			break;
		case Direction::Left:
			state_ = Actor::State::IdleLeft;
			break;
		default:
			break;
		}
	}
	else {
		if (getVelocity().x > 0)
			state_ = Actor::State::MoveRight;
		if (getVelocity().x < 0)
			state_ = Actor::State::MoveLeft;
		if (getVelocity().y > 0)
			state_ = Actor::State::MoveFront;
		if (getVelocity().y < 0)
			state_ = Actor::State::MoveBack;
	}
}

void Hero::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (isSpellcasting_ || isAttacking_ || isCastingShield_)
		setVelocity(0.f, 0.f);

	updateDirections();
	updateStates();

	//isCastingShield_ = false;
	Entity::updateCurrent(dt, commands);

	auto rec = animations_.at(state_).update(dt);

	sprite_.setTextureRect(rec);
	centerOrigin(sprite_);

	checkCastingAttackingSpell(dt, commands);

	shield->setVisible(isCastingShield_);

}
