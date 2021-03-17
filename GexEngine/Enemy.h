#include "Actor.h"
#include "FightingCharacter.h"


#pragma once
class Enemy : public Actor, public FightingCharacter
{
public:
    Enemy(Actor::Type type, const TextureHolder_t& textures, const FontHolder_t& fonts);
    ~Enemy() = default;

    unsigned int    getCategory() const override;

private:
    void            updateCurrent(sf::Time dt, CommandQueue & commands) override;
};

