#include "combatHelpers.h"

std::string createTileName(const int row, const int col) { return std::to_string(row) + " " + std::to_string(col); }

bool characterTypeFit(ECombatCharacterType targetType, ECombatAttackTargetAlive targetAlive, ECombatCharacterType givenCharacterType, bool givenCharacterAlive)
{
	if (targetType == ECombatCharacterType_CHARACTER || targetType == givenCharacterType)
	{
		switch (targetAlive)
		{
		case ECombatAttackTargetAlive_ALIVE:
			return givenCharacterAlive;
		case ECombatAttackTargetAlive_DEAD:
			return !givenCharacterAlive;
		default:
			SDL_assert(false);
		}
	}
	return false;
}

std::string returnDescriptionOfGridPattern(const ECombatActionGridPattern moveAttackType, const int num, const int out)
{
	std::string type;
	switch (moveAttackType)
	{
	case ECombatActionGridPattern_SQUARE:
		type = "SQUARE";
		break;
	case ECombatActionGridPattern_CROSS:
		type = "CROSS";
		break;
	case ECombatActionGridPattern_CHECKERBOARD:
		type = "CHECKERBOARD";
		break;
	case ECombatActionGridPattern_WHOLE_GRID: return "WHOLE GRID";
	default:
		SDL_assert(false);
		break;
	}
	
	
	if (out == 0)
	{
		return type + " " + std::to_string(num);
	}
	return type + " " + std::to_string(num) + ", " + std::to_string(out);
}

std::string returnDescriptionCharacterAttack(const ECombatNumTilesToAttack numTiles, const ECombatCharacterType targetType)
{
	std::string target;
	switch (targetType)
	{
	case ECombatCharacterType_CHARACTER:
		target = "CHARACTER";
		break;
	case ECombatCharacterType_PLAYER:
		target = "PLAYER";
		break;
	case ECombatCharacterType_ENEMY:
		target = "ENEMY";
		break;
	default:
		SDL_assert(false);
		break;
	}
	
	switch (numTiles)
	{
	case ECombatNumTilesToAttack_ONE:		return "ONE " + target + " TILE";
	case ECombatNumTilesToAttack_DIRECTION: return "ALL " + target + " TILES IN ONE DIRECTION";
	case ECombatNumTilesToAttack_ALL:		return "ALL " + target + "TILES";
	default:
		SDL_assert(false);
	}
	return "";
}
