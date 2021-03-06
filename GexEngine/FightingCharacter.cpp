#include "FightingCharacter.h"
#include "EnergyBolt.h"

FightingCharacter::FightingCharacter(Type t)
	: type(t)
	, attackingCountDown(ATTACKING_INTERVAL)
{

}

void FightingCharacter::attack()
{
	if (attackingCountDown <= sf::Time::Zero) {
		isAttacking_ = true;
		attackingCountDown = ATTACKING_INTERVAL;
	}
}

void FightingCharacter::checkCastingAttackingSpell(sf::Time dt, CommandQueue& commands)
{
	if (isAttacking_) {
		commands.push(fireCommand);
	}
	else if (attackingCountDown > sf::Time::Zero) {
		attackingCountDown -= dt;
	}
	isAttacking_ = false;
}

void FightingCharacter::createEnergyBolt(SceneNode& node, const TextureHolder_t& textures) const
{
}
