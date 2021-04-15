/*
Alena Selezneva
*/

#include "DialogNode.h"

DialogNode::DialogNode(DialogNode::Type type, std::string text, bool isAgr)
	: type(type)
	, text(text)
	, attribute(Attribute::None)
	, isAgreeing_(isAgr)
{
	children = std::vector<DialogNode*>();
}

DialogNode::DialogNode(bool isAgr)
	: type(Type::Message)
	, text("")
	, attribute(Attribute::None)
	, isAgreeing_(isAgr)
{
}

std::string DialogNode::getText()
{
	return text;
}

void DialogNode::setText(std::string t)
{
	text = t;
}

//bool DialogNode::isAnswerRequired()
//{
//	return children.size() > 1;
//}

DialogNode::Type DialogNode::getType()
{
	return type;
}

void DialogNode::attachChild(DialogNode* child)
{
	children.push_back(child);
}

std::vector<DialogNode*>* DialogNode::getChildren()
{
	return &children;
}

Attribute DialogNode::getAttribute()
{
	return attribute;
}

bool DialogNode::isAgreeing()
{
	return isAgreeing_;
}
