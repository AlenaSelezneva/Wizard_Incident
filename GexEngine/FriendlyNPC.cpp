#include "FriendlyNPC.h"



FriendlyNPC::FriendlyNPC(const TextureHolder_t& textures, const FontHolder_t& fonts)
    :Actor(Actor::Type::TalkingNPC, textures, fonts)
{
    dialog = std::vector<std::string>();
    dialog.push_back("Hello");
    dialog.push_back("How is your day?");
    dialog.push_back("Leave me alone");
}

std::string FriendlyNPC::getDialog()
{
    if (dialog.size() == 0)
        return "";
    if (currentDialogOptionIndex >= dialog.size())
        currentDialogOptionIndex = 0;

    auto res = dialog[currentDialogOptionIndex];
    currentDialogOptionIndex++;
    return res;
}
