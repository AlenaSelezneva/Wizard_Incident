

#include "CommandQueue.h"
#include "ResourceIdentifiers.h"

#pragma once
class FightingCharacter
{
public:
    enum class Type {
        Hero,
        Ally,
        Enemy
    };

    FightingCharacter(Type t);

    virtual void    attack();

protected:
    void            checkCastingAttackingSpell(sf::Time dt, CommandQueue& commands);
    void            createEnergyBolt(SceneNode& node, const TextureHolder_t& textures) const;

protected:
    Type            type;
    bool            isAttacking_;

    const sf::Time  ATTACKING_INTERVAL = sf::seconds(3.5f);

    sf::Time        attackingCountDown;
    Command			fireCommand;
};

