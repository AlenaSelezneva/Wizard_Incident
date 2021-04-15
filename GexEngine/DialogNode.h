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
						DialogNode( DialogNode::Type type, std::string text, bool isAgr = false);
						DialogNode(bool isAgr = false);

	std::string			getText();
	void				setText(std::string t);
	//bool				isAnswerRequired();
	DialogNode::Type	getType();
	void				attachChild(DialogNode* child);
	std::vector<DialogNode*>* getChildren();

	Attribute			getAttribute();

	bool				isAgreeing();

protected:
	Type						type;
	std::string					text;
	Attribute					attribute;
	std::vector<DialogNode*>	children;

	bool						isAgreeing_;

};

