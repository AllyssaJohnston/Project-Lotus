#include "combatHelpers.h"

std::string createTileName(const int row, const int col) { return std::to_string(row) + " " + std::to_string(col); }

bool characterTypeFit(ECombatAttackTargetType targetType, ECombatCharacterType characterType, bool targetAlive)
{
	if (targetType == ECombatAttackTargetType_ONE_CHARACTER)
	{
		return true;
	}
	if ((targetType == ECombatAttackTargetType_ALL_ALIVE_CHARACTERS || targetType == ECombatAttackTargetType_ALIVE_CHARACTERS || targetType == ECombatAttackTargetType_ONE_ALIVE_CHARACTER) && targetAlive)
	{
		return true;
	}
	if (targetType == ECombatAttackTargetType_ONE_PLAYER && characterType == ECombatCharacterType_PLAYER)
	{
		return true;
	}
	if ((targetType == ECombatAttackTargetType_ALL_ALIVE_PLAYERS || targetType == ECombatAttackTargetType_ALIVE_PLAYERS || targetType == ECombatAttackTargetType_ONE_ALIVE_PLAYER)
		&& characterType == ECombatCharacterType_PLAYER && targetAlive) 
	{
		return true;
	}
	if (targetType == ECombatAttackTargetType_ONE_ENEMY && characterType == ECombatCharacterType_ENEMY)
	{
		return true;
	}
	if ((targetType == ECombatAttackTargetType_ALL_ALIVE_ENEMIES || targetType == ECombatAttackTargetType_ALIVE_ENEMIES || targetType == ECombatAttackTargetType_ONE_ALIVE_ENEMY)
		&& characterType == ECombatCharacterType_ENEMY && targetAlive)
	{
		return true;
	}
	return false;
}

// TODO have attack hold combatCharactertype and an attack target type with alive, 1 alive, one all alive
ECombatCharacterType getCharacterTypeFromAttackTargetType(ECombatAttackTargetType targetType)
{
	switch (targetType)
	{
	case ECombatAttackTargetType_ALIVE_PLAYERS:
	case ECombatAttackTargetType_ONE_ALIVE_PLAYER:
	case ECombatAttackTargetType_ONE_PLAYER:
	case ECombatAttackTargetType_ALL_ALIVE_PLAYERS:
		return ECombatCharacterType_PLAYER;

	case ECombatAttackTargetType_ALIVE_ENEMIES:
	case ECombatAttackTargetType_ONE_ALIVE_ENEMY:
	case ECombatAttackTargetType_ONE_ENEMY:
	case ECombatAttackTargetType_ALL_ALIVE_ENEMIES:
		return ECombatCharacterType_ENEMY;
	default:
		SDL_assert(false);
	}
	return ECombatCharacterType_CHARACTER;
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

std::string returnDescriptionOfAttackTargetType(const ECombatAttackTargetType targetType)
{
	switch (targetType)
	{
	case ECombatAttackTargetType_SELF: return "SELF";

	case ECombatAttackTargetType_ALIVE_PLAYERS:
	case ECombatAttackTargetType_ALL_ALIVE_PLAYERS:
		return "PLAYERS IN RANGE";
	case ECombatAttackTargetType_ONE_ALIVE_PLAYER:
	case ECombatAttackTargetType_ONE_PLAYER:
		return "ONE PLAYER";
		
	case ECombatAttackTargetType_ALIVE_ENEMIES:
	case ECombatAttackTargetType_ALL_ALIVE_ENEMIES:
		return "ENEMIES IN RANGE";

	case ECombatAttackTargetType_ONE_ALIVE_ENEMY:
	case ECombatAttackTargetType_ONE_ENEMY:
		return "ONE ENEMY";
	
	case ECombatAttackTargetType_ALIVE_CHARACTERS:
	case ECombatAttackTargetType_ALL_ALIVE_CHARACTERS:
		return "CHARACTERS IN RANGE";
	case ECombatAttackTargetType_ONE_ALIVE_CHARACTER:
	case ECombatAttackTargetType_ONE_CHARACTER:
		return "ONE CHARACTER";
		
	default:
		SDL_assert(false);
	}
	return "";
}

std::string returnDescirptionOfNumTilesToAttack(const ECombatNumTilesToAttack numTiles)
{
	switch (numTiles)
	{
	case ECombatNumTilesToAttack_ONE:
		return "ONE TILE";
	case ECombatNumTilesToAttack_DIRECTION:
		return "ALL TILES IN DIRECTION";
	case ECombatNumTilesToAttack_ALL:
		return "ALL TILES";
	default:
		SDL_assert(false);
	}
}
