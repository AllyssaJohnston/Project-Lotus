#include "combatAttackHelpers.h"

std::string getAttackType(const Attack& attack) { return returnDescriptionOfMoveAttackType(attack.mType, attack.mNum, attack.mOut); }

std::string getAttackDamage(const Attack& attack, const int characterDamage) { return std::to_string((int)(attack.mDamagePercent * characterDamage)); }

std::string getSpecialEffect(const Attack& attack)
{
	std::string message = (attack.mRequiresDirectionInput ? "Requires directional input." : "");
	message += (attack.mDamageDistanceDependent ? "Damage is distance dependent." : "");
	for (SpecialEffect curSpecialEffect : attack.mSpecialEffects)
	{
		message += curSpecialEffect.mName + ". ";
	}

	return message;
}
