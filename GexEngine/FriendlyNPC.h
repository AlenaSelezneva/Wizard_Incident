#pragma once
#include "Actor.h"

#include <string>

class FriendlyNPC :
    public Actor
{
public:
    FriendlyNPC(const TextureHolder_t& textures, const FontHolder_t& fonts);
    std::string getDialog();

private:
    std::vector<std::string> dialog;
    int currentDialogOptionIndex;
};

