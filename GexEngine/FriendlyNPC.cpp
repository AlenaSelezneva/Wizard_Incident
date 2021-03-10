#include "FriendlyNPC.h"



FriendlyNPC::FriendlyNPC(Actor::Type type, ObjectWithQuest::Type questType, const TextureHolder_t& textures, const FontHolder_t& fonts)
    :Actor(type, textures, fonts)
    , ObjectWithQuest(questType)
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
