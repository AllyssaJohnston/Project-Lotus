#include "combatHelpers.h"

std::string createTileName(const int row, const int col) { return std::to_string(row) + " " + std::to_string(col); }

bool characterTypeFit(EAttackTargetType targetType, EMiniGameCombatCharacterType characterType, bool targetAlive)
{
	if (targetType == EAttackTargetType_ONE_CHARACTER)
	{
		return true;
	}
	if ((targetType == EAttackTargetType_ALL_ALIVE_CHARACTERS || targetType == EAttackTargetType_ALIVE_CHARACTERS || targetType == EAttackTargetType_ONE_ALIVE_CHARACTER) && targetAlive)
	{
		return true;
	}
	if (targetType == EAttackTargetType_ONE_PLAYER && characterType == EMiniGameCombatCharacterType_PLAYER)
	{
		return true;
	}
	if ((targetType == EAttackTargetType_ALL_ALIVE_PLAYERS || targetType == EAttackTargetType_ALIVE_PLAYERS || targetType == EAttackTargetType_ONE_ALIVE_PLAYER)
		&& characterType == EMiniGameCombatCharacterType_PLAYER && targetAlive) 
	{
		return true;
	}
	if (targetType == EAttackTargetType_ONE_ENEMY && characterType == EMiniGameCombatCharacterType_ENEMY)
	{
		return true;
	}
	if ((targetType == EAttackTargetType_ALL_ALIVE_ENEMIES || targetType == EAttackTargetType_ALIVE_ENEMIES || targetType == EAttackTargetType_ONE_ALIVE_ENEMY)
		&& characterType == EMiniGameCombatCharacterType_ENEMY && targetAlive)
	{
		return true;
	}
	return false;
}

EMiniGameCombatCharacterType getCharacterTypeFromAttackTargetType(EAttackTargetType targetType)
{
	switch (targetType)
	{
	case EAttackTargetType_ALIVE_PLAYERS:
	case EAttackTargetType_ONE_ALIVE_PLAYER:
	case EAttackTargetType_ONE_PLAYER:
	case EAttackTargetType_ALL_ALIVE_PLAYERS:
		return EMiniGameCombatCharacterType_PLAYER;

	case EAttackTargetType_ALIVE_ENEMIES:
	case EAttackTargetType_ONE_ALIVE_ENEMY:
	case EAttackTargetType_ONE_ENEMY:
	case EAttackTargetType_ALL_ALIVE_ENEMIES:
		return EMiniGameCombatCharacterType_ENEMY;
	default:
		SDL_assert(false);
	}
	return EMiniGameCombatCharacterType_CHARACTER;
}

std::string returnDescriptionOfMoveAttackType(const EMiniGameCombatMoveAttackTypes moveAttackType, const int num, const int out)
{
	std::string type;
	switch (moveAttackType)
	{
	case EMiniGameCombatMoveAttackTypes_SQUARE: 
		type = "SQUARE";
		break;
	case EMiniGameCombatMoveAttackTypes_CROSS:
		type = "CROSS";
		break;
	case EMiniGameCombatMoveAttackTypes_CHECKERBOARD:
		type = "CHECKERBOARD";
		break;
	case EMiniGameCombatMoveAttackTypes_WHOLE_GRID:		return "WHOLE GRID";
	case EMiniGameCombatMoveAttackTypes_ANY_ONE_TILE:	return "ANY ONE TILE";
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

std::string returnDescriptionOfAttackTargetType(const EAttackTargetType targetType)
{
	switch (targetType)
	{
	case EAttackTargetType_SELF: return "SELF";

	case EAttackTargetType_ALIVE_PLAYERS:
	case EAttackTargetType_ALL_ALIVE_PLAYERS:
		return "PLAYERS IN RANGE";
	case EAttackTargetType_ONE_ALIVE_PLAYER:
	case EAttackTargetType_ONE_PLAYER:
		return "ONE PLAYER";
		
	case EAttackTargetType_ALIVE_ENEMIES:
	case EAttackTargetType_ALL_ALIVE_ENEMIES:
		return "ENEMIES IN RANGE";

	case EAttackTargetType_ONE_ALIVE_ENEMY:
	case EAttackTargetType_ONE_ENEMY:
		return "ONE ENEMY";
	
	case EAttackTargetType_ALIVE_CHARACTERS:
	case EAttackTargetType_ALL_ALIVE_CHARACTERS:
		return "CHARACTERS IN RANGE";
	case EAttackTargetType_ONE_ALIVE_CHARACTER:
	case EAttackTargetType_ONE_CHARACTER:
		return "ONE CHARACTER";
		
	default:
		SDL_assert(false);
	}
	return "";
}