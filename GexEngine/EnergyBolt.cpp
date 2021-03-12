#include "EnergyBolt.h"
#include "Utility.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include <cmath>

EnergyBolt::EnergyBolt(Type type, int dmg, const TextureHolder_t& textures)
	: Entity(1)
	, type(type)
	, sprite()
	, damage(dmg)
{
	switch (type) {
	case Type::AlliedBolt:
		sprite.setTexture(textures.get(TextureID::EnergyBallHero));
		break;
	case Type::EnemyBolt:
		sprite.setTexture(textures.get(TextureID::EnergyBallEnemy));
		break;
	}
	/*std::unique_ptr<EmitterNode> smoke(new EmitterNode(Particle::Type::Smoke));
	smoke->setPosition(0.f, getBoundingRect().height / 2.f);
	attachChild(std::move(smoke));

	std::unique_ptr<EmitterNode> fire(new EmitterNode(Particle::Type::Propellant));
	fire->setPosition(0.f, getBoundingRect().height / 2.f);
	attachChild(std::move(fire));*/

}

void EnergyBolt::guideTowards(sf::Vector2f position)
{
	targetDirection = normalize(position - getWorldPoition());
}

unsigned int EnergyBolt::getCategory() const
{
	if (type == Type::AlliedBolt)
		return Category::BaseAttackAllied;
	else
		return Category::baseAttackEnemy;
}

float EnergyBolt::getMaxSpeed() const
{
	return 300.f;
}

int EnergyBolt::getDamage() const
{
	return damage;
}

sf::FloatRect EnergyBolt::getBoundingRect() const
{
	return getWorldTransform().transformRect(sprite.getGlobalBounds());
}

void EnergyBolt::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

void EnergyBolt::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	const float approachRate = 200.f;

	sf::Vector2f newVelocity =
		normalize(approachRate * dt.asSeconds() * targetDirection + getVelocity());

	newVelocity *= getMaxSpeed();
	float angle = std::atan2(newVelocity.y, newVelocity.x);
	setRotation(toDegree(angle) + 90.f);
	setVelocity(newVelocity);

	Entity::updateCurrent(dt, commands);
}
