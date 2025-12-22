#include "combatHelperClass.h"


std::string returnDescriptionOfMoveAttackType(int moveAttackType)
{
	switch (moveAttackType)
	{
	case EMiniGameCombatMoveAttackTypes_SQUARE1:
		return "SQUARE 1";
	case EMiniGameCombatMoveAttackTypes_SQUARE2:
		return "SQUARE 2";
	case EMiniGameCombatMoveAttackTypes_SQUARE2_1UNIT_OUT:
		return "SQUARE 2 1 UNIT OUT";
	case EMiniGameCombatMoveAttackTypes_SQUARE3_2UNITS_OUT:
		return "SQUARE 3 2 UNITS OUT";
	case EMiniGameCombatMoveAttackTypes_CROSS1:
		return "CROSS 1";
	case EMiniGameCombatMoveAttackTypes_CROSS1_1UNIT_OUT:
		return "CROSS 1 1 UNIT OUT";
	case EMiniGameCombatMoveAttackTypes_CROSS2:
		return "CROSS 2";
	case EMiniGameCombatMoveAttackTypes_CROSS2_1UNIT_OUT:
		return "CROSS 2 1 UNIT OUT";
	case EMiniGameCombatMoveAttackTypes_CROSS3:
		return "CROSS 3";
	case EMiniGameCombatMoveAttackTypes_CROSS4:
		return "CROSS 4";
	case EMiniGameCombatMoveAttackTypes_CHECKERBOARD2:
		return "CHECKERBOARD 2";
	case EMiniGameCombatMoveAttackTypes_WHOLE_GRID:
		return "WHOLE GRID";
	case EMiniGameCombatMoveAttackTypes_ONE_CHARACTER:
		return "ONE CHARACTER";
	case EMiniGameCombatMoveAttackTypes_ONE_PLAYER:
		return "ONE PLAYER";
	case EMiniGameCombatMoveAttackTypes_ONE_ENEMY:
		return "ONE ENEMY";
	default:
		SDL_assert(false);
		break;
	}
	
	return "";
}