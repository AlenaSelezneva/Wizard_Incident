#include <string>

#pragma once
class PlayerData
{
public:
	std::string				getCurrentDialog() const;
	void					setCurrentDialog(std::string s);

private:
	std::string				currentDialog;

};

