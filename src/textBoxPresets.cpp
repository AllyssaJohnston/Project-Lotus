#include "textBoxPresets.h"


StandardTextBoxPreset::StandardTextBoxPreset(std::string message) : TextBoxPreset() { mMessage = message; }

MiniGameStatBoxPreset::MiniGameStatBoxPreset(EGameStatBoxValueToDisplay gameStateValueToDisplay) 
{ 
	mGameStatToDisplay = gameStateValueToDisplay; 
	mType = ETextBoxType_MINI_GAME_STAT_BOX;
}

MiniGameCharacterBoxPreset::MiniGameCharacterBoxPreset(int characterIndex, bool showDuringAllCharacters, ECharacterStatBoxValueToDisplay characterStatToDisplay) : TextBoxPreset()
{
	mCombatCharacterIndex			= characterIndex;
	mShowDuringAllCharacters		= showDuringAllCharacters;
	mCharacterStatToDisplay			= characterStatToDisplay;
	mMiniGameStateWhenToShowList	= { EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT, EMiniGameState_PLAYER_MOVE_CHARACTER, EMiniGameState_PLAYER_WAIT_FOR_ACTION_INPUT,
										EMiniGameState_PLAYER_WAIT_FOR_ATTACK_INPUT, EMiniGameState_PLAYER_WAIT_FOR_ATTACK_SUB_INPUT,
										EMiniGameState_PLAYER_COMPLETE_DIRECTIONAL_ATTACK, EMiniGameState_PLAYER_TAKE_ACTION_ATTACK, EMiniGameState_PLAYER_TAKE_ACTION_DEFEND,
										EMiniGameState_ENEMY_MOVE_CHARACTER, EMiniGameState_ENEMY_TAKE_ACTION, EMiniGameState_BUFFER, };
	mType							= ETextBoxType_MINI_GAME_CHARACTER_BOX;
	mMessage						= "Z"; //default
}

MiniGameCharacterBoxPreset::MiniGameCharacterBoxPreset(int characterIndex, bool showDuringAllCharacters, ECharacterStatBoxValueToDisplay characterStatToDisplay, std::vector <EMiniGameState> miniGameStateWhenToShowList) : TextBoxPreset()
{
	mCombatCharacterIndex			= characterIndex;
	mShowDuringAllCharacters		= showDuringAllCharacters;
	mCharacterStatToDisplay			= characterStatToDisplay;
	mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
	mType							= ETextBoxType_MINI_GAME_CHARACTER_BOX;
	mMessage						= "Z"; //default
}

MiniGameCharacterBoxPreset::MiniGameCharacterBoxPreset(int characterIndex, bool showDuringAllCharacters, ECharacterStatBoxValueToDisplay characterStatToDisplay, int attackNum, std::vector <EMiniGameState> miniGameStateWhenToShowList) 
{
	mCombatCharacterIndex = characterIndex;
	mCharacterStatToDisplay			= characterStatToDisplay;
	mShowDuringAllCharacters		= showDuringAllCharacters;
	mAttackNum						= attackNum;
	mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
	mType							= ETextBoxType_MINI_GAME_CHARACTER_BOX;
	mMessage						= "Z"; //default
}

MiniGameCharacterBoxPreset::MiniGameCharacterBoxPreset(int characterIndex, bool showDuringAllCharacters, std::string message, std::vector <EMiniGameState> miniGameStateWhenToShowList)
{
	mCombatCharacterIndex = characterIndex;
	mShowDuringAllCharacters		= showDuringAllCharacters;
	mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
	mType							= ETextBoxType_MINI_GAME_CHARACTER_BOX;
	mMessage						= message;
}

MiniGamePlayerBoxPreset::MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay characterStatToDisplay, std::vector <EMiniGameState> miniGameStateWhenToShowList) : TextBoxPreset()
{
	mCharacterStatToDisplay			= characterStatToDisplay;
	mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
	mType							= ETextBoxType_MINI_GAME_PLAYER_BOX;
	mMessage						= "Z"; //default
}





MiniGameBoxPreset::MiniGameBoxPreset(std::string message, std::vector <EMiniGameState> miniGameStateWhenToShowList) : TextBoxPreset()
{
	mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
	mType							= ETextBoxType_MINI_GAME_BOX;
	mMessage						= message;
}

GameStatBoxPreset::GameStatBoxPreset(EGameStatBoxValueToDisplay gameStateValueToDisplay) : TextBoxPreset()
{
	mGameStatToDisplay		= gameStateValueToDisplay;
	mType					= ETextBoxType_GAME_STAT_BOX;
	mMessage				= "Z"; //default
}

//IMAGE BOXES
DontAutoShowImageBoxPreset::DontAutoShowImageBoxPreset(ETextBoxID id) : ImageBoxPreset()
{
	mAutoShow	= false;
	mID			= id;
}


//SHAPE BOXES

ShapeBoxPreset::ShapeBoxPreset(EShapeBoxClass type) : mType(type) { ; }

MiniGameShapeBoxPreset::MiniGameShapeBoxPreset(EShapeBoxClass type, int characterIndex, bool showDuringAllCharacters) : ShapeBoxPreset(type)
{
	mShowType = EShapeTypeShowType_MINI_GAME_CHARACTER_BOX;
	mCombatCharacterIndex = characterIndex;
	mShowDuringAllCharacters = showDuringAllCharacters;
}

MiniGameShapeBoxPreset::MiniGameShapeBoxPreset(EShapeBoxClass type, int characterIndex, bool showDuringAllCharacters, 
		std::vector <EMiniGameState> miniGameStateWhenToShowList) : ShapeBoxPreset(type)
{
	mShowType = EShapeTypeShowType_MINI_GAME_CHARACTER_BOX;
	mCombatCharacterIndex = characterIndex;
	mShowDuringAllCharacters = showDuringAllCharacters;
	mMiniGameStateWhenToShowList = miniGameStateWhenToShowList;
}

MiniGameShapeBoxPreset::MiniGameShapeBoxPreset(EShapeBoxClass type, int characterIndex, bool showDuringAllCharacters, int attackNum, 
		std::vector <EMiniGameState> miniGameStateWhenToShowList) : ShapeBoxPreset(type)
{
	mShowType = EShapeTypeShowType_MINI_GAME_CHARACTER_BOX;
	mCombatCharacterIndex = characterIndex;
	mShowDuringAllCharacters = showDuringAllCharacters;
	mAttackNum = attackNum;
	mMiniGameStateWhenToShowList = miniGameStateWhenToShowList;
}

MiniGameShapeBoxPreset::MiniGameShapeBoxPreset(EShapeBoxClass type, std::vector <EMiniGameState> miniGameStateWhenToShowList) : ShapeBoxPreset(type)
{
	mShowType = EShapeTypeShowType_MINI_GAME_CHARACTER_BOX;
	mMiniGameStateWhenToShowList = miniGameStateWhenToShowList;
}



HealthBoxPreset::HealthBoxPreset(int characterIndex) : mCombatCharacterIndex(characterIndex) { ; }