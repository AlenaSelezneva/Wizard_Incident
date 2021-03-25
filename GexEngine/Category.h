/*
Alena Selezneva
*/

#pragma once

namespace Category
{
	enum Type
	{
		None = 0,			// 000000000
		Background		= 1 << 0,		// 000000001

		TalkingNPC		= 1 << 1,
		FightingNPC		= 1 << 2,
		Hero			= 1 << 3,

		ShieldSpell		= 1 << 4,
		LightningSpell  = 1 << 5,
		
		SpellLayer		= 1 << 6,

		Door			= 1 << 7,
		Wall			= 1 << 8,

		InteractableObject = 1 << 9,

		BaseAttackAllied	= 1 << 10,
		baseAttackEnemy		= 1 << 11,

		Portal				= 1 << 12,

		//ParticleSystem = 1 << 7,
		//SoundEffect = 1 << 4,

		BaseAttack = BaseAttackAllied | baseAttackEnemy,
		NPC = TalkingNPC | FightingNPC,
		AttackingSpell = LightningSpell | BaseAttack,
		QuestObject = TalkingNPC | InteractableObject,
		//InteractableObject = BookshelfQuest | BookshelfNotQuest
	};
}

