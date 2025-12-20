#include "textBoxPresets.h"

// TEXT BOXES
StandardTextBoxPreset::StandardTextBoxPreset(std::string message) : TextBoxPreset()
{ 
	mMessage = message; 
	mData = TextBoxData();
}

MiniGameStatBoxPreset::MiniGameStatBoxPreset(EGameStatBoxValueToDisplay gameStateValueToDisplay): TextBoxPreset()
{ 
	mData.mGameStatToDisplay = gameStateValueToDisplay; 
	mData.mType = ETextBoxType_MINI_GAME_STAT_BOX;
}

MiniGameCharacterBoxPreset::MiniGameCharacterBoxPreset(int characterIndex, bool showDuringAllCharacters, ECharacterStatBoxValueToDisplay characterStatToDisplay) : TextBoxPreset()
{
	mData.mCombatCharacterIndex			= characterIndex;
	mData.mShowDuringAllCharacters		= showDuringAllCharacters;
	mData.mCharacterStatToDisplay		= characterStatToDisplay;
	mData.mMiniGameStateWhenToShowList	= { EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT, EMiniGameState_PLAYER_MOVE_CHARACTER, EMiniGameState_PLAYER_WAIT_FOR_ACTION_INPUT,
											EMiniGameState_PLAYER_WAIT_FOR_ATTACK_OPTION_INPUT, EMiniGameState_PLAYER_WAIT_FOR_ATTACK_DIRECTION_INPUT,
											EMiniGameState_PLAYER_WAIT_FOR_ATTACK_TILE_INPUT, EMiniGameState_PLAYER_COMPLETE_ACTION_ATTACK, EMiniGameState_PLAYER_COMPLETE_ACTION_DEFEND,
											EMiniGameState_ENEMY_MOVE_CHARACTER, EMiniGameState_ENEMY_TAKE_ACTION, EMiniGameState_BUFFER, EMiniGameState_CHARACTER_STUNNED };
	mData.mType							= ETextBoxType_MINI_GAME_CHARACTER_BOX;
}

MiniGameCharacterBoxPreset::MiniGameCharacterBoxPreset(int characterIndex, bool showDuringAllCharacters, ECharacterStatBoxValueToDisplay characterStatToDisplay, 
		std::vector <EMiniGameState> miniGameStateWhenToShowList) : TextBoxPreset()
{
	mData.mCombatCharacterIndex			= characterIndex;
	mData.mShowDuringAllCharacters		= showDuringAllCharacters;
	mData.mCharacterStatToDisplay		= characterStatToDisplay;
	mData.mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
	mData.mType							= ETextBoxType_MINI_GAME_CHARACTER_BOX;
}

MiniGameCharacterBoxPreset::MiniGameCharacterBoxPreset(int characterIndex, bool showDuringAllCharacters, ECharacterStatBoxValueToDisplay characterStatToDisplay, int attackNum,
	std::vector <EMiniGameState> miniGameStateWhenToShowList) : TextBoxPreset()
{
	mData.mCombatCharacterIndex			= characterIndex;
	mData.mCharacterStatToDisplay		= characterStatToDisplay;
	mData.mShowDuringAllCharacters		= showDuringAllCharacters;
	mData.mAttackNum					= attackNum;
	mData.mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
	mData.mType							= ETextBoxType_MINI_GAME_CHARACTER_BOX;
}

MiniGameCharacterBoxPreset::MiniGameCharacterBoxPreset(int characterIndex, bool showDuringAllCharacters, std::string message, std::vector <EMiniGameState> miniGameStateWhenToShowList) 
	: TextBoxPreset()
{
	mData.mCombatCharacterIndex			= characterIndex;
	mData.mShowDuringAllCharacters		= showDuringAllCharacters;
	mData.mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
	mData.mType							= ETextBoxType_MINI_GAME_CHARACTER_BOX;
	mMessage							= message;
}

MiniGamePlayerBoxPreset::MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay characterStatToDisplay, std::vector <EMiniGameState> miniGameStateWhenToShowList) : TextBoxPreset()
{
	mData.mCharacterStatToDisplay		= characterStatToDisplay;
	mData.mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
	mData.mType							= ETextBoxType_MINI_GAME_PLAYER_BOX;
}

MiniGameBoxPreset::MiniGameBoxPreset(std::string message, std::vector <EMiniGameState> miniGameStateWhenToShowList) : TextBoxPreset()
{
	mData.mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
	mData.mType							= ETextBoxType_MINI_GAME_BOX;
	mMessage							= message;
}

MiniGameDirectionBoxPreset::MiniGameDirectionBoxPreset(std::string message, std::vector <EMiniGameState> miniGameStateWhenToShowList, EDirection direction) : TextBoxPreset()
{
	mData.mMiniGameStateWhenToShowList = miniGameStateWhenToShowList;
	mData.mType = ETextBoxType_MINI_GAME_DIRECTION_BOX;
	mData.mAttackDirection = direction;
	mMessage = message;
}


GameStatBoxPreset::GameStatBoxPreset(EGameStatBoxValueToDisplay gameStateValueToDisplay) : TextBoxPreset()
{
	mData.mGameStatToDisplay	= gameStateValueToDisplay;
	mData.mType					= ETextBoxType_GAME_STAT_BOX;
}

// IMAGE BOXES
DontAutoShowImageBoxPreset::DontAutoShowImageBoxPreset(ETextBoxID id) : ImageBoxPreset()
{
	mAutoShow	= false;
	mID			= id;
}


// SHAPE BOXES
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



HealthBoxPreset::HealthBoxPreset(int characterIndex, ECharacterStatBoxValueToDisplay stat) : mCombatCharacterIndex(characterIndex), mStatToDisplay(stat) { ; }