#include "Entity.h"
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"

#include <SFML/Graphics/Sprite.hpp>

#pragma once
class EnergyBolt : public Entity
{
public:
	enum class Type {
		AlliedBolt,
		EnemyBolt
	};

public:
	EnergyBolt(Type type, int dmg, const TextureHolder_t& textures);

	void					guideTowards(sf::Vector2f position);

	virtual unsigned int	getCategory() const override;

	float					getMaxSpeed() const;
	int						getDamage() const;
	virtual sf::FloatRect	getBoundingRect() const override;

private:
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void			updateCurrent(sf::Time dt, CommandQueue& commands) override;

private:
	Type					type;
	sf::Sprite				sprite;
	sf::Vector2f			targetDirection;
	int						damage;

};

