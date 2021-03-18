#include "Actor.h"
#include "FightingCharacter.h"

#pragma once
class Hero : public Actor, public FightingCharacter
{
public:
	Hero(const TextureHolder_t& textures, const FontHolder_t& fonts);

    bool            isSpellCasting();
    void            setSpellCasting(bool b);

    bool            isCastingShield() const;
    void            setCastingShield(bool b);

    virtual void    attack() override;


private:
    void            updateStates() override;
    void            updateCurrent(sf::Time dt, CommandQueue& commands) override;

private:
	
    bool                            isSpellcasting_;
    bool                            isCastingShield_;

    const sf::Time                  ATTACKING_CASTING_TIME = sf::seconds(1.75f);
    sf::Time                        attackingCastingCountDown;

    UiNode*                         shield;
};

