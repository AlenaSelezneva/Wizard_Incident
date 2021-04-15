/*
Alena Selezneva
*/

#include "DialogAnswer.h"

DialogAnswer::DialogAnswer(std::string text, bool isAgr):
	DialogNode(DialogNode::Type::HeroOption, text, isAgr)
{}

DialogAnswer::DialogAnswer(std::string text, Attribute attr, bool isAgr)
	: DialogNode(DialogNode::Type::HeroOption, text, isAgr)
{
	attribute = attr;
}
