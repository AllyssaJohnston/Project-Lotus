#pragma once
#include "menuHelperClass.h"

std::string getAttackName(Attack attack)
{
	return attack.mDescription;
}

std::string getAttackType(Attack attack)
{
	return returnDescriptionOfMoveAttackType(attack.mType);
}

std::string getAttackDamage(Attack attack, int characterDamage)
{
	return std::to_string(attack.mDamagePercent * characterDamage);
}

std::string getSpecialEffect(Attack attack)
{
	std::string message;
	std::string requiresDirectionalInput;
	if (attack.mRequiresDirectionInput)
	{
		requiresDirectionalInput = "Requires directional input";
	}
	std::string specialEffects;
	for (SpecialEffect curSpecialEffect : attack.mSpecialEffects)
	{
		specialEffects += curSpecialEffect.mName + " ";
	}
	std::string distanceDependent;
	if (attack.mDamageDistanceDependent)
	{
		requiresDirectionalInput = "Damage is distance dependent";
	}
	message = requiresDirectionalInput + " " + specialEffects + " " + distanceDependent;
	//TODO trim start
	return message;
}