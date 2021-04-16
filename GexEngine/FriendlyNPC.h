#pragma once
#include "Actor.h"
#include "ObjectWithQuest.h"

#include <string>

class FriendlyNPC :
    public Actor, public ObjectWithQuest
{
public:
                    FriendlyNPC(Actor::Type type, ObjectWithQuest::Type questType, const TextureHolder_t& textures, const FontHolder_t& fonts);
                    FriendlyNPC(Actor* actor);

    void            setCanTalkToHero(bool canTalk);
    bool            canTalkToHero();

    std::string     getName();
    void            setName(std::string n);

    void			updateMovementPattern(sf::Time dt);
    void            updateCurrent(sf::Time dt, CommandQueue& commands) override;

    Actor::Direction getNextDirection();


private:
    bool            canTalkToHero_;
    std::string     name;

    const sf::Time  MOVEMENT_TIME_MAX = sf::seconds(2.f);
    sf::Time        movementCountDown;
    bool            isMoving_;
    Direction       directionToMove;
};

