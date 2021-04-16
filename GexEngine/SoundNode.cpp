

#include "SoundNode.h"
#include "SoundPlayer.h"

SoundNode::SoundNode(SoundPlayer& player)
	:SceneNode()
	, sounds(player)
{}

void SoundNode::playSound(EffectID effect, sf::Vector2f position)
{
	sounds.play(effect, position);
}

void SoundNode::playSound(EffectID effect)
{
	sounds.play(effect);
}

unsigned int SoundNode::getCategory() const
{
	return Category::SoundEffect;
	//return Category::None;
}
