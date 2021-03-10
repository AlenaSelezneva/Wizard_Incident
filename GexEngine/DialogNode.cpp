/*
Alena Selezneva
*/

#include "DialogNode.h"

DialogNode::DialogNode(DialogNode::Type type, std::string text)
	: type(type)
	, text(text)

{
	children = std::vector<DialogNode*>();
}

DialogNode::DialogNode()
	: type(Type::Message)
	, text("")
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
