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

private:
    bool            canTalkToHero_;
    std::string     name;
};

