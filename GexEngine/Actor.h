
#pragma once

#include "Entity.h"
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "Animation.h"
#include "CommandQueue.h"

#include <SFML/Graphics/Sprite.hpp>

class UiNode;

class Actor : public Entity
{
public:
    enum class Type {
        None = 0,
        Hero = 1,
        Archmage = 1 << 1,
        Dgery = 1 << 2,
        Elony = 1 << 3,
        Gazan = 1 << 4,
        Lunars = 1 << 5,
        Semrid = 1 << 6,
        MysteriousFigure = 1 << 7,

        TalkingNPC = Archmage | Dgery | Elony | Gazan | Lunars | Semrid,

        FightingNPC, 
        //Frogger, Car, Log, NPC
    };

    enum class State {
        IdleLeft, MoveLeft, SpellCastLeft,
        IdleRight, MoveRight, SpellCastRight,
        IdleBack, MoveBack, SpellCastBack,
        IdleFront, MoveFront, SpellCastFront,
        Dead,
        Count
    };

    enum class Direction
    {
        Left, Right, Back, Front, Count
    };


public:
    Actor(Type type, const TextureHolder_t& textures, const FontHolder_t& fonts);
    ~Actor() = default;
    Actor(Actor&&) = default;

    unsigned int    getCategory() const override;
    sf::FloatRect   getBoundingRect() const override;
    sf::FloatRect   getBaseTileRect() const override;
    float           getMaxSpeed() const;

    bool            isMarkedForRemoval() const override;

    void            setState(State state);
    Actor::State    getState() const;

    Actor::Type     getType() const;

    virtual std::string   getFightHealthDisplayString();

    const TextureHolder_t& getTextures() const;
    const FontHolder_t&   getFonts() const;

protected:
    virtual void   updateStates();
    void         updateDirections();
    void         updateCurrent(sf::Time dt, CommandQueue& commands) override;
    void         drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:
    Type                            type_;
    State                           state_;
    /*mutable sf::Sprite              sprite_;*/
    sf::Sprite                      sprite_;
    std::map<State, Animation>      animations_;
    Direction                       direction_;

    float                           travelDistance_;
    const FontHolder_t&       fonts;
    const TextureHolder_t&    textures;

};

