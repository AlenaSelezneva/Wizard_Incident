#include "Actor.h"
#include "FightingCharacter.h"


#pragma once
class Enemy : public Actor, public FightingCharacter
{
public:
    Enemy(Actor::Type type, const TextureHolder_t& textures, const FontHolder_t& fonts);
    ~Enemy() = default;

    Enemy(Actor* actor);

    unsigned int    getCategory() const override;

    virtual void    createEnergyBolt(SceneNode& node, const TextureHolder_t& textures) const override;

private:
    void            updateCurrent(sf::Time dt, CommandQueue & commands) override;
};

