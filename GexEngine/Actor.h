
#pragma once

#include "Entity.h"
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "Animation.h"
#include "CommandQueue.h"

#include <SFML/Graphics/Sprite.hpp>

class Actor : public Entity
{
public:
    enum class Type {
        Hero, FightingNPC, TalkingNPC
        //Frogger, Car, Log, NPC
    };

    enum class State {
        IdleLeft, MoveLeft, SpellCastLeft,
        IdleRight, MoveRight, SpellCastRight,
        IdleUp, MoveUp, SpellCastUp,
        IdleDown, MoveDown, SpellCastDown,
        Death,
        Count
    };

    enum class Direction
    {
        Left, Right, Up, Down
    };


public:
    Actor(Type type, const TextureHolder_t& textures, const FontHolder_t& fonts);
    ~Actor() = default;

    unsigned int    getCategory() const override;
    sf::FloatRect   getBoundingRect() const override;
    sf::FloatRect   getBaseTileRect() const override;
    float           getMaxSpeed() const;

    bool            isMarkedForRemoval() const override;

    void            attack();
    void            setState(State state);
    Actor::State    getState() const;


private:
    void         updateStates();
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
    std::size_t                     directionIndex_;
    bool                            attack_;
};