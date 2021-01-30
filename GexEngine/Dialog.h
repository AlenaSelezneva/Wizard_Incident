/*
Alena Selezneva
*/

#include "DialogNode.h"

#include<memory>

#pragma once
class Dialog
{
public:
	DialogNode*		getCurrentOption();
	DialogNode*		getStart();
	void			moveToNext();
	void			chooseAnswer(int i);


private:
	std::unique_ptr<DialogNode> dialogStart;
	DialogNode*					currentOption;
};

