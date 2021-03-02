#pragma once
#include "Actor.h"

#include <string>

class FriendlyNPC :
    public Actor
{
public:
                    FriendlyNPC(const TextureHolder_t& textures, const FontHolder_t& fonts);
    std::string     getDialog();
    void            setCanTalkToHero(bool canTalk);
    bool            canTalkToHero();


private:
    std::vector<std::string> dialog;
    int currentDialogOptionIndex;
    bool canTalkToHero_;
};

