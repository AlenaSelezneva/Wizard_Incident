/*
Alena Selezneva
*/

#include "Actor.h"
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "JsonFrameParser.h"
#include "Utility.h"
#include "DataTables.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace
{
	//const std::map<Actor::Type, ActorData> TABLE = initializeActorData();
}
Actor::Actor(Type type, const TextureHolder_t& textures, const FontHolder_t& fonts)
	: Entity(100)
	, type_(type)
	, state_(State::Idle)
	//, sprite_(textures.get(TABLE.at(type).texture))
	, sprite_(textures.get(TextureID::Eagle))
	, direction_(Direction::Right)
	, travelDistance_(0.f)
	, directionIndex_(0)
	, attack_(false)
{

	/*for (auto a : TABLE.at(type).animations)
	{
		animations_[a.first] = a.second;
	}

	if (Actor::getCategory() == Category::Zombie)
		state_ = State::Rise;  // zombies spawn in rise state

	sprite_.setTextureRect(sf::IntRect());
	centerOrigin(sprite_);

	//
	// set up text for health and missiles
	// 
	std::unique_ptr<TextNode> health(new TextNode(fonts, ""));
	healthDisplay_ = health.get();
	attachChild(std::move(health));

	updateTexts();*/
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
		box.width += 30; 
		box.height += 30;

		box.left -= 15;
		box.top -= 15;
	}

	return box;
}

sf::FloatRect Actor::getBaseTileRect() const
{
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

void Actor::attack()
{
	attack_ = true;
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

void Actor::updateStates()
{
	
}

void Actor::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	Entity::updateCurrent(dt, commands);

	/*updateStates();

	auto rec = animations_.at(state_).update(dt);

	if (state_ != State::Dead)
	{
		if (direction_ == Direction::Left && getVelocity().x > 0)
			direction_ = Direction::Right;
		if (direction_ == Direction::Right && getVelocity().x < 0)
			direction_ = Direction::Left;
	}

	// flip image left right
	if (direction_ == Direction::Left)
		rec = flip(rec);

	sprite_.setTextureRect(rec);
	centerOrigin(sprite_);

	if (state_ != State::Dead) // dont move it while dying
		Entity::updateCurrent(dt, commands);

	updateMovementPattern(dt);
	updateTexts();*/
}

void Actor::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite_, states);
}
