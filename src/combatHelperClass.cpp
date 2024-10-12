#include "combatHelperClass.h"
#include <SDL3/SDL.h>
#include <string>

std::string returnDescriptionOfMoveAttackType(int moveAttackType)
{
	std::string description;
	switch (moveAttackType)
	{
	case EMiniGameCombatMoveAttackTypes_SQUARE1:
		description = "SQUARE 1";
		break;
	case EMiniGameCombatMoveAttackTypes_SQUARE2:
		description = "SQUARE 2";
		break;
	case EMiniGameCombatMoveAttackTypes_SQUARE2_1UNIT_OUT:
		description = "SQUARE 2 1 UNIT OUT";
		break;
	case EMiniGameCombatMoveAttackTypes_SQUARE3_2UNITS_OUT:
		description = "SQUARE 3 2 UNITS OUT";
		break;
	case EMiniGameCombatMoveAttackTypes_CROSS1:
		description = "CROSS 1";
		break;
	case EMiniGameCombatMoveAttackTypes_CROSS1_1UNIT_OUT:
		description = "CROSS 1 1 UNIT OUT";
		break;
	case EMiniGameCombatMoveAttackTypes_CROSS2:
		description = "CROSS 2";
		break;
	case EMiniGameCombatMoveAttackTypes_CROSS2_1UNIT_OUT:
		description = "CROSS 2 1 UNIT OUT";
		break;
	case EMiniGameCombatMoveAttackTypes_CROSS4:
		description = "CROSS 4";
		break;
	case EMiniGameCombatMoveAttackTypes_CHECKERBOARD2UNITS:
		description = "CHECKERBOARD 2 UNITS";
		break;
	default:
		SDL_assert(false);
		break;
	}
	
	return description;
}