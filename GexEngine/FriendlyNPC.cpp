#include "FriendlyNPC.h"



FriendlyNPC::FriendlyNPC(Actor::Type type, const TextureHolder_t& textures, const FontHolder_t& fonts)
    :Actor(type, textures, fonts)
{
}

void FriendlyNPC::setCanTalkToHero(bool canTalk)
{
    canTalkToHero_ = canTalk;
}

bool FriendlyNPC::canTalkToHero()
{
    return canTalkToHero_;
}
