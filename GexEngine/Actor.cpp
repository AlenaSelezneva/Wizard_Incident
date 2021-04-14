/*
Alena Selezneva
*/

#include "Actor.h"
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "JsonFrameParser.h"
#include "Utility.h"
#include "DataTables.h"
#include "EnergyBolt.h"
#include "UiNode.h"
#include "SoundNode.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace
{
	const std::map<Actor::Type, ActorData> TABLE = initializeActorData();
}
Actor::Actor(Type type, const TextureHolder_t& textures, const FontHolder_t& fonts)
	: Entity(300)
	, type_(type)
	, state_(State::IdleFront)
	, sprite_()
	, direction_(Direction::Front)
	, travelDistance_(0.f)
	, textures(textures)
	, fonts(fonts)
{

	if (type_ == Actor::Type::Hero) {
		sprite_.setTexture(textures.get( TABLE.at(type).textureID));
		for (auto a : TABLE.at(type).animations)
		{
			animations_[a.first] = a.second;
		}
	}
	else {
		// change this when get more json!!!
		sprite_.setTexture(textures.get(TABLE.at(Actor::Type::Archmage).textureID));
		for (auto a : TABLE.at(Actor::Type::Archmage).animations)
		{
			animations_[a.first] = a.second;
		}
	}

}

unsigned int Actor::getCategory() const
{
	switch (type_)
	{
	case Type::Hero:
		return Category::Hero;
		break;
	case Type::FightingNPC:
		return Category::FightingNPC;
		break;
	case Type::Archmage:
	case Type::Dgery:
	case Type::Elony:
	case Type::Gazan:
	case Type::Lunars:
	case Type::MysteriousFigure:
	case Type::Semrid:
	case Type::TalkingNPC:
		return Category::TalkingNPC;
		break;
	}

	return Category::NPC;
}

sf::FloatRect Actor::getBoundingRect() const
{
	auto box = getWorldTransform().transformRect(sprite_.getGlobalBounds());

	if (type_ == Actor::Type::Hero) {
		box.width += 10; 
		box.height += 10;

		box.left -= 5;
		box.top -= 5;
	}

	return box;
}

sf::FloatRect Actor::getBaseTileRect() const
{
	//return getBoundingRect();

	auto rect = getBoundingRect();
	const float tileSize = 50.f;

	if (rect.height > tileSize) {
		rect.top = rect.top + rect.height - tileSize;
		rect.height = tileSize;
	}

	return rect;
}

float Actor::getMaxSpeed() const
{
	//return TABLE.at(type_).speed;
	if (type_ == Actor::Type::Hero)
		return 100.f;

	return 0.f;
}

bool Actor::isMarkedForRemoval() const
{
	return false; // (state_ == State::Dead && animations_.at(state_).isFinished());
}

void Actor::playLocalSound(CommandQueue& commands, EffectID effect)
{
	auto worldPosition = getWorldPoition();

	Command command;
	command.category = Category::SoundEffect;
	command.action = derivedAction<SoundNode>(
		[effect, worldPosition](SoundNode& node, sf::Time dt) {
			node.playSound(effect, worldPosition);
		});
	commands.push(command);
}

void Actor::setState(State state)
{
	state_ = state;
	animations_[state_].restart();
}

Actor::State Actor::getState() const
{
	return state_;
}

Actor::Type Actor::getType() const
{
	return type_;
}

std::string Actor::getFightHealthDisplayString()
{
	std::string heroString = "Hero: \n";
	heroString += "HP: ";
	heroString += std::to_string( getHitpoints());

	return heroString;
}

const TextureHolder_t& Actor::getTextures() const
{
	return textures;
}

const FontHolder_t& Actor::getFonts() const
{
	return fonts;
}


void Actor::updateStates()
{
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

void Actor::updateDirections()
{
	if (getVelocity().y > 0)
		direction_ = Direction::Front;
	if (getVelocity().y < 0)
		direction_ = Direction::Back;

	if (getVelocity().x > 0)
		direction_ = Direction::Right;
	if (getVelocity().x < 0)
		direction_ = Direction::Left;
}



void Actor::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	updateDirections();
	updateStates();

	//isCastingShield_ = false;
	Entity::updateCurrent(dt, commands);

	auto rec = animations_.at(state_).update(dt);

	sprite_.setTextureRect(rec);
	centerOrigin(sprite_);
}

void Actor::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite_, states);
}
