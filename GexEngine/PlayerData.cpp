#include "PlayerData.h"


std::string PlayerData::getCurrentDialog() const
{
	return currentDialog;
}

void PlayerData::setCurrentDialog(std::string s)
{
	currentDialog = s;
}