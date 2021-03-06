#include "Actor.h"
#include "FightingCharacter.h"
#include "PlayerData.h"
#include "EnergyBolt.h"


#pragma once
class Hero : public Actor, public FightingCharacter
{
private:
    enum HeroManaCost {
        BaseAttack,
        PowerAttack,
        HealingSpell,
        ShieldPerSecond,
        RegeneratePerSecond,

        Count
    };

public:
	Hero(const TextureHolder_t& textures, const FontHolder_t& fonts, PlayerData* data);

    bool            isSpellCasting();
    void            setSpellCasting(bool b);

    bool            isCastingShield() const;
    void            setCastingShield(bool b);

    void            finishFight();

    virtual std::string   getFightHealthDisplayString();

    virtual void    attack() override;
    void            castPowerAttackSpell();
    void            castHealingSpell();

    virtual void    createEnergyBolt(EnergyBolt::Type boltType, SceneNode& node, const TextureHolder_t& textures) const;
    virtual void    createEnergyBolt(SceneNode& node, const TextureHolder_t& textures) const override;


private:
    void            updateStates() override;
    void            updateCurrent(sf::Time dt, CommandQueue& commands) override;

    int             getSpellManaCost(HeroManaCost spell) const;

private:
    Command			                powerAttackCommand;
    bool                            isSpellcasting_;
    bool                            isCastingShield_;
    bool                            isCastingPowerAttack_;

    const float                     MAX_MANA_POINTS = 300;
    const int                       MAX_HITPOINTS = 300;

    float                           manapoints;
    PlayerData*                     playerData;

    const sf::Time                  ATTACKING_CASTING_TIME = sf::seconds(1.75f);
    sf::Time                        attackingCastingCountDown;

    UiNode*                         shield;
};

