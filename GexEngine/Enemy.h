#include "Actor.h"


#pragma once
class Enemy : public Actor
{
public:
    Enemy(Type type, const TextureHolder_t& textures, const FontHolder_t& fonts);
    ~Enemy() = default;

    unsigned int    getCategory() const override;
    //bool            isMarkedForRemoval() const override;

    void            attack();

private:
    void            updateCurrent(sf::Time dt, CommandQueue & commands) override;
    void            checkCastingAttackingSpell(sf::Time dt, CommandQueue& commands);
    void            createEnergyBolt(SceneNode& node, const TextureHolder_t& textures) const;

private:
    const sf::Time  ATTACKING_INTERVAL = sf::seconds(1.5f);

    sf::Time        attackingCountDown;

    Command				fireCommand;
};

