/*
Alena Selezneva
*/

#include "DialogNode.h"

#pragma once
class DialogMessage : public DialogNode
{
public:
	DialogMessage(std::string text);
	bool isAnswerRequired();

private:

};

