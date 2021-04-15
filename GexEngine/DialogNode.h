/*
Alena Selezneva
*/

#pragma once
#include <string>
#include <vector>
#include <list>

#include "Attribute.h"

class DialogNode
{
public:
	enum class Type {
		Message, HeroOption, Count
	};


public:
						DialogNode( DialogNode::Type type, std::string text);
						DialogNode();

	std::string			getText();
	void				setText(std::string t);
	//bool				isAnswerRequired();
	DialogNode::Type	getType();
	void				attachChild(DialogNode* child);
	std::vector<DialogNode*>* getChildren();

	Attribute			getAttribute();

protected:
	Type						type;
	std::string					text;
	Attribute					attribute;
	std::vector<DialogNode*>	children;


};

