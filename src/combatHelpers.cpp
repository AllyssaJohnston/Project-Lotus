#include "combatHelpers.h"

std::string createTileName(const int row, const int col) { return std::to_string(row) + " " + std::to_string(col); }

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
	case EMiniGameCombatMoveAttackTypes_ONE_CHARACTER:	return "ONE CHARACTER";
	case EMiniGameCombatMoveAttackTypes_ONE_PLAYER:		return "ONE PLAYER";
	case EMiniGameCombatMoveAttackTypes_ONE_ENEMY:		return "ONE ENEMY";
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