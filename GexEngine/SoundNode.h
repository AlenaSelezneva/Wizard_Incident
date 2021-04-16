
#pragma once
#include "SceneNode.h"
#include "ResourceIdentifiers.h"

class SoundPlayer;

class SoundNode : public SceneNode
{
public:
	SoundNode(SoundPlayer& player);
	void					playSound(EffectID, sf::Vector2f position);
	void					playSound(EffectID);

	virtual unsigned int	getCategory() const override;

private:
	SoundPlayer& sounds;
};

