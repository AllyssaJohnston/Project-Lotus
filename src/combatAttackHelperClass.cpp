#include "combatAttackHelperClass.h"

AttackTile::AttackTile(Tile& tile, Attack& attack) : mTile(tile), mAttack(attack) {}

std::string getAttackType(const Attack& attack) { return returnDescriptionOfMoveAttackType(attack.mType); }

std::string getAttackDamage(const Attack& attack, int characterDamage) { return std::to_string((int)(attack.mDamagePercent * characterDamage)); }

std::string getSpecialEffect(const Attack& attack)
{
	std::string message = (attack.mRequiresDirectionInput ? "Requires directional input." : "");
	message += (attack.mDamageDistanceDependent ? "Damage is distance dependent." : "");
	for (SpecialEffect curSpecialEffect : attack.mSpecialEffects)
	{
		message += curSpecialEffect.mName + ". ";
	}

	// TODO trim start
	return message;
}
