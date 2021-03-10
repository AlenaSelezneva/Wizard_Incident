#pragma once
#include "Actor.h"

#include <string>

class FriendlyNPC :
    public Actor
{
public:
                    FriendlyNPC(Actor::Type type, const TextureHolder_t& textures, const FontHolder_t& fonts);
    void            setCanTalkToHero(bool canTalk);
    bool            canTalkToHero();


private:
    bool canTalkToHero_;
};

