/*
Alena Selezneva
*/

#include "DialogNode.h"

#pragma once
class DialogAnswer : public DialogNode
{
public:
	DialogAnswer(std::string text, bool isAgr = false);
	DialogAnswer(std::string text, Attribute attr, bool isAgr = false);

private:
	// add stats change

};

