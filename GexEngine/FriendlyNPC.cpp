#include "FriendlyNPC.h"



FriendlyNPC::FriendlyNPC(Actor::Type type, const TextureHolder_t& textures, const FontHolder_t& fonts)
    :Actor(type, textures, fonts)
{
    //dialog = std::vector<std::string>();
    //dialog.push_back("Hello");
    //dialog.push_back("How is your day?");
    //dialog.push_back("Leave me alone");

    //sprite_ = sf::Sprite(textures.get(TextureID::Raptor));
}

//std::string FriendlyNPC::getDialog()
//{
//    if (dialog.size() == 0)
//        return "";
//    if (currentDialogOptionIndex >= dialog.size())
//        currentDialogOptionIndex = 0;
//
//    auto res = dialog[currentDialogOptionIndex];
//    currentDialogOptionIndex++;
//    return res;
//}

void FriendlyNPC::setCanTalkToHero(bool canTalk)
{
    canTalkToHero_ = canTalk;
}

bool FriendlyNPC::canTalkToHero()
{
    return canTalkToHero_;
}
