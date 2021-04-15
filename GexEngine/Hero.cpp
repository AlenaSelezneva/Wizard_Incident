#include "Hero.h"
#include "UiNode.h"
#include "Utility.h"
#include "EnergyBolt.h"

#include <math.h>

Hero::Hero(const TextureHolder_t& textures, const FontHolder_t& fonts, PlayerData* data)
	:Actor(Actor::Type::Hero, textures, fonts)
	, FightingCharacter(FightingCharacter::Type::Hero)
	, playerData(data)
	, attackingCastingCountDown(ATTACKING_CASTING_TIME)
	, manapoints(MAX_MANA_POINTS)
{

	//ATTACKING_INTERVAL
	/*fireCommand.category = Category::SpellLayer;*/
	fireCommand.category = Category::SpellLayer;
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

void Hero::finishFight()
{
	manapoints = MAX_MANA_POINTS;
	hitPoints = MAX_HITPOINTS;
}

std::string Hero::getFightHealthDisplayString()
{
	std::string heroString = "Hero: \n";
	heroString += "HP: ";
	heroString += std::to_string(getHitpoints());
	heroString += "\nMana: " + std::to_string((int)floor(manapoints));

	return heroString;
}

void Hero::attack()
{
	int cost = getSpellManaCost(HeroManaCost::BaseAttack);
	if (manapoints >= cost) {
		isAttacking_ = true;
		attackingCastingCountDown = ATTACKING_CASTING_TIME;
		manapoints -= cost;
	}
}

void Hero::createEnergyBolt(SceneNode& node, const TextureHolder_t& textures) const
{
	std::unique_ptr<EnergyBolt> bolt(new EnergyBolt
		(EnergyBolt::Type::AlliedBolt, 10 + playerData->getHeroAttribute(Attribute::Arrogance), textures));

	/*sf::Vector2f offset(sprite_.getGlobalBounds().width / 2,
		sprite_.getGlobalBounds().height);*/

	sf::Vector2f offset(0.f, -20.f);
	sf::Vector2f velocity(0.f, 0.f);

	//float widthCenter = sprite_.getLocalBounds().width / 2;

	float velocity_ = 20.f;

	switch (direction_) {
	case Direction::Back:
		offset = sf::Vector2f(-15.f, -30.f);
		velocity = sf::Vector2f(0.f, -velocity_);
		break;
	case Direction::Front:
		offset = sf::Vector2f(15.f, -10.f);
		velocity = sf::Vector2f(0.f, velocity_);
		break;
	case Direction::Left:
		offset = sf::Vector2f(-35.f, -10.f);
		velocity = sf::Vector2f(-velocity_, 0.f);
		break;
	case Direction::Right:
		offset = sf::Vector2f(35.f, -30.f);
		velocity = sf::Vector2f(velocity_, 0.f);
		break;
	}

	bolt->setPosition(getWorldPoition() + offset);
	bolt->setVelocity(velocity);

	node.attachChild(std::move(bolt));
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
	manapoints = std::min(MAX_MANA_POINTS, manapoints + dt.asSeconds() * getSpellManaCost(HeroManaCost::RegeneratePerSecond));

	if (manapoints < 10.f)
		isCastingShield_ = false;

	if (isSpellcasting_ || isAttacking_ || isCastingShield_)
		setVelocity(0.f, 0.f);

	if (isCastingShield_) 
		manapoints -= dt.asSeconds() * getSpellManaCost(HeroManaCost::ShieldPerSecond);

	updateDirections();
	updateStates();

	if (isAttacking_ && attackingCastingCountDown <= sf::Time::Zero) {
		commands.push(fireCommand);
		playLocalSound(commands, EffectID::AlliedAttack);
		isAttacking_ = false;
		attackingCastingCountDown = ATTACKING_CASTING_TIME;
	}

	//isCastingShield_ = false;
	Entity::updateCurrent(dt, commands);

	auto rec = animations_.at(state_).update(dt);

	sprite_.setTextureRect(rec);
	centerOrigin(sprite_);

	//checkCastingAttackingSpell(dt, commands);

	shield->setVisible(isCastingShield_);
	attackingCastingCountDown -= dt;
}

int Hero::getSpellManaCost(HeroManaCost spell) const
{
	int res = 0;
	switch (spell)
	{
	case HeroManaCost::BaseAttack:
		res = 10;
		break;
	case HeroManaCost::ShieldPerSecond:
		res = 30 - playerData->getHeroAttribute(Attribute::Intelligence);
		if (res < 5) res = 5;
		break;
	case HeroManaCost::RegeneratePerSecond:
		res = 5 + playerData->getHeroAttribute(Attribute::Wisdom);
		break;
	}

	return res;
}
