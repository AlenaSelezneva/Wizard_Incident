/*
Alena Selezneva
*/

#include "DialogNode.h"

#pragma once
class DialogMessage : DialogNode
{
public:
	DialogMessage(std::string text);
	bool isAnswerRequired();

private:

};

