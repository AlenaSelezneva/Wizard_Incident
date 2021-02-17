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
		BaseAttack		= 1 << 6,

		Door			= 1 << 7,
		Wall			= 1 << 8,

		//ParticleSystem = 1 << 7,
		//SoundEffect = 1 << 4,


		NPC = TalkingNPC | FightingNPC,
		Spell = ShieldSpell | LightningSpell | BaseAttack
	};
}

