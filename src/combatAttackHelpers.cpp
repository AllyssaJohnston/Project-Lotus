#include "combatAttackHelpers.h"

std::string getAttackType(const Attack& attack) { return returnDescriptionOfGridPattern(attack.mType, attack.mNum, attack.mOut); }

std::string getAttackDamage(const Attack& attack, const int characterDamage) { return std::to_string((int)(attack.mDamagePercent * characterDamage)) + " DAMAGE"; }

std::string getAttackCurCooldown(const Attack& attack) { return std::to_string(attack.mCurCooldown); }

std::string getAttackCooldownAmount(const Attack& attack) { return std::to_string(attack.mCooldownAmount); }

std::string getSpecialEffectAndNotes(const Attack& attack)
{
	std::string message = returnDescriptionCharacterAttack(attack.mNumTilesToAttack, attack.mAttackTargetType) + ". ";
	message += (attack.mDamageDistanceDependent ? "Damage is distance dependent." : "");
	for (SpecialEffect curSpecialEffect : attack.mCharacterTileSpecialEffects)
	{
		message += curSpecialEffect.mName + ". ";
	}
	for (SpecialEffect curSpecialEffect : attack.mGenericSpecialEffects)
	{
		message += curSpecialEffect.mName + ". ";
	}
	return message;
}
