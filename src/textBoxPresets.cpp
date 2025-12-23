#include "textBoxPresets.h"

// TEXT BOXES
StandardTextBoxPreset::StandardTextBoxPreset(std::string message) : TextBoxPreset()
{ 
	mMessage = message; 
	mData = UIBoxData();
	mData.mType = EUIBoxType_STANDARD;
}

MiniGameStatBoxPreset::MiniGameStatBoxPreset(EGameStatBoxValueToDisplay gameStateValueToDisplay): TextBoxPreset()
{ 
	mData.mGameStatToDisplay = gameStateValueToDisplay; 
	mData.mType = EUIBoxType_MINI_GAME_STAT_BOX;
}

MiniGameCharacterBoxPreset::MiniGameCharacterBoxPreset(int characterIndex, bool showDuringAllCharacters, ECharacterStatBoxValueToDisplay characterStatToDisplay) : TextBoxPreset()
{
	mData.mCombatCharacterIndex			= characterIndex;
	mData.mCombatCharacterType			= EMiniGameCombatCharacterType_CHARACTER;
	mData.mShowDuringAllCharacters		= showDuringAllCharacters;
	mData.mCharacterStatToDisplay		= characterStatToDisplay;
	mData.mMiniGameStateWhenToShowList	= allMiniStates;
	mData.mType							= EUIBoxType_MINI_GAME_CHARACTER_BOX;
}

MiniGameCharacterBoxPreset::MiniGameCharacterBoxPreset(int characterIndex, bool showDuringAllCharacters, ECharacterStatBoxValueToDisplay characterStatToDisplay, std::vector <EMiniGameState> miniGameStateWhenToShowList) : TextBoxPreset()
{
	mData.mCombatCharacterIndex			= characterIndex;
	mData.mCombatCharacterType			= EMiniGameCombatCharacterType_CHARACTER;
	mData.mShowDuringAllCharacters		= showDuringAllCharacters;
	mData.mCharacterStatToDisplay		= characterStatToDisplay;
	mData.mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
	mData.mType							= EUIBoxType_MINI_GAME_CHARACTER_BOX;
}

MiniGameCharacterBoxPreset::MiniGameCharacterBoxPreset(int characterIndex, EMiniGameCombatCharacterType characterType, bool showDuringAllCharacters, ECharacterStatBoxValueToDisplay characterStatToDisplay,
		std::vector <EMiniGameState> miniGameStateWhenToShowList)
{
	mData.mCombatCharacterIndex			= characterIndex;
	mData.mCombatCharacterType			= characterType;
	mData.mShowDuringAllCharacters		= showDuringAllCharacters;
	mData.mCharacterStatToDisplay		= characterStatToDisplay;
	mData.mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
	mData.mType							= EUIBoxType_MINI_GAME_CHARACTER_BOX;
}


MiniGameCharacterBoxPreset::MiniGameCharacterBoxPreset(int characterIndex, bool showDuringAllCharacters, std::string message, std::vector <EMiniGameState> miniGameStateWhenToShowList) : TextBoxPreset()
{
	mData.mCombatCharacterIndex			= characterIndex;
	mData.mCombatCharacterType			= EMiniGameCombatCharacterType_CHARACTER;
	mData.mShowDuringAllCharacters		= showDuringAllCharacters;
	mData.mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
	mData.mType							= EUIBoxType_MINI_GAME_CHARACTER_BOX;
	mMessage							= message;
}

MiniGameCharacterBoxPreset::MiniGameCharacterBoxPreset(int characterIndex, EMiniGameCombatCharacterType characterType, bool showDuringAllCharacters, ECharacterStatBoxValueToDisplay characterStatToDisplay)
{
	mData.mCombatCharacterIndex			= characterIndex;
	mData.mCombatCharacterType			= characterType;
	mData.mShowDuringAllCharacters		= showDuringAllCharacters;
	mData.mCharacterStatToDisplay		= characterStatToDisplay;
	mData.mMiniGameStateWhenToShowList	= allMiniStates;
	mData.mType							= EUIBoxType_MINI_GAME_CHARACTER_BOX;
}

MiniGameCharacterBoxPreset::MiniGameCharacterBoxPreset(int characterIndex, bool showDuringAllCharacters, std::string message)
{
	mData.mCombatCharacterIndex			= characterIndex;
	mData.mCombatCharacterType			= EMiniGameCombatCharacterType_CHARACTER;
	mData.mMiniGameStateWhenToShowList	= allMiniStates;
	mData.mType							= EUIBoxType_MINI_GAME_CHARACTER_BOX;
	mData.mShowDuringAllCharacters		= showDuringAllCharacters;
	mMessage							= message;
}


MiniGamePlayerBoxPreset::MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay characterStatToDisplay, std::vector <EMiniGameState> miniGameStateWhenToShowList) : TextBoxPreset()
{
	mData.mCharacterStatToDisplay		= characterStatToDisplay;
	mData.mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
	mData.mType							= EUIBoxType_MINI_GAME_PLAYER_BOX;
}

MiniGamePlayerAttackBoxPreset::MiniGamePlayerAttackBoxPreset(int characterIndex, bool showDuringAllCharacters, ECharacterStatBoxValueToDisplay characterStatToDisplay, int attackNum,
	EMiniGameCombatAttackCategoryType attackCategory, std::vector <EMiniGameState> miniGameStateWhenToShowList) : TextBoxPreset()
{
	mData.mCombatCharacterIndex			= characterIndex;
	mData.mCharacterStatToDisplay		= characterStatToDisplay;
	mData.mShowDuringAllCharacters		= showDuringAllCharacters;
	mData.mAttackNum					= attackNum;
	mData.mAttackCategory				= attackCategory;
	mData.mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
	mData.mType							= EUIBoxType_MINI_GAME_PLAYER_ATTACK_BOX;
}

MiniGameBoxPreset::MiniGameBoxPreset(std::string message, std::vector <EMiniGameState> miniGameStateWhenToShowList) : TextBoxPreset()
{
	mData.mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
	mData.mType							= EUIBoxType_MINI_GAME_BOX;
	mMessage							= message;
}

MiniGameDirectionBoxPreset::MiniGameDirectionBoxPreset(std::string message, std::vector <EMiniGameState> miniGameStateWhenToShowList, EDirection direction) : TextBoxPreset()
{
	mData.mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
	mData.mType							= EUIBoxType_MINI_GAME_DIRECTION_BOX;
	mData.mAttackDirection				= direction;
	mMessage							= message;
}


GameStatBoxPreset::GameStatBoxPreset(EGameStatBoxValueToDisplay gameStateValueToDisplay) : TextBoxPreset()
{
	mData.mGameStatToDisplay	= gameStateValueToDisplay;
	mData.mType					= EUIBoxType_GAME_STAT_BOX;
}


// IMAGE BOXES
DontAutoShowImageBoxPreset::DontAutoShowImageBoxPreset(EUIBoxType id) : ImageBoxPreset()
{
	mAutoShow	= false;
	mData.mType	= id;
}

MiniGameCharacterImageBoxPreset::MiniGameCharacterImageBoxPreset(int characterIndex, bool showDuringAllCharacters) : ImageBoxPreset()
{
	mData.mType							= EUIBoxType_MINI_GAME_CHARACTER_BOX;
	mData.mCombatCharacterIndex			= characterIndex; 
	mData.mShowDuringAllCharacters		= showDuringAllCharacters;
	mData.mMiniGameStateWhenToShowList	= allMiniStates;
}

MiniGameCharacterImageBoxPreset::MiniGameCharacterImageBoxPreset(int characterIndex, bool showDuringAllCharacters, std::vector <EMiniGameState> miniGameStateWhenToShowList) : ImageBoxPreset()
{
	mData.mType = EUIBoxType_MINI_GAME_CHARACTER_BOX;
	mData.mCombatCharacterIndex = characterIndex;
	mData.mShowDuringAllCharacters = showDuringAllCharacters;
	mData.mMiniGameStateWhenToShowList = miniGameStateWhenToShowList;
}

MiniGameCharacterImageBoxPreset::MiniGameCharacterImageBoxPreset(int characterIndex, EMiniGameCombatCharacterType characterType, bool showDuringAllCharacters, std::vector <EMiniGameState> miniGameStateWhenToShowList) : ImageBoxPreset()
{
	mData.mType = EUIBoxType_MINI_GAME_CHARACTER_BOX;
	mData.mCombatCharacterIndex = characterIndex;
	mData.mCombatCharacterType = characterType;
	mData.mShowDuringAllCharacters = showDuringAllCharacters;
	mData.mMiniGameStateWhenToShowList = miniGameStateWhenToShowList;
}


// SHAPE BOXES
ShapeBoxPreset::ShapeBoxPreset(EShapeBoxClass type) : mType(type) { ; }

MiniGameCharacterShapeBoxPreset::MiniGameCharacterShapeBoxPreset(EShapeBoxClass type, int characterIndex, bool showDuringAllCharacters) : ShapeBoxPreset(type)
{
	mData.mType						= EUIBoxType_MINI_GAME_CHARACTER_BOX;
	mData.mCombatCharacterIndex		= characterIndex;
	mData.mCombatCharacterType		= EMiniGameCombatCharacterType_CHARACTER;
	mData.mShowDuringAllCharacters	= showDuringAllCharacters;
}

MiniGameCharacterShapeBoxPreset::MiniGameCharacterShapeBoxPreset(EShapeBoxClass type, int characterIndex, bool showDuringAllCharacters,
		std::vector <EMiniGameState> miniGameStateWhenToShowList) : ShapeBoxPreset(type)
{
	mData.mType							= EUIBoxType_MINI_GAME_CHARACTER_BOX;
	mData.mCombatCharacterIndex			= characterIndex;
	mData.mCombatCharacterType			= EMiniGameCombatCharacterType_CHARACTER;
	mData.mShowDuringAllCharacters		= showDuringAllCharacters;
	mData.mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
}

MiniGameCharacterShapeBoxPreset::MiniGameCharacterShapeBoxPreset(EShapeBoxClass type, int characterIndex, EMiniGameCombatCharacterType characterType, bool showDuringAllCharacters, 
		std::vector <EMiniGameState> miniGameStateWhenToShowList) : ShapeBoxPreset(type)
{
	mData.mType							= EUIBoxType_MINI_GAME_CHARACTER_BOX;
	mData.mCombatCharacterIndex			= characterIndex;
	mData.mCombatCharacterType			= characterType;
	mData.mShowDuringAllCharacters		= showDuringAllCharacters;
	mData.mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
}

MiniGameCharacterShapeBoxPreset::MiniGameCharacterShapeBoxPreset(EShapeBoxClass type, std::vector <EMiniGameState> miniGameStateWhenToShowList) : ShapeBoxPreset(type)
{
	mData.mType							= EUIBoxType_MINI_GAME_CHARACTER_BOX;
	mData.mCombatCharacterType			= EMiniGameCombatCharacterType_CHARACTER;
	mData.mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
}

MiniGamePlayerAttackShapeBoxPreset::MiniGamePlayerAttackShapeBoxPreset(EShapeBoxClass type, int characterIndex, bool showDuringAllCharacters, int attackNum,
	EMiniGameCombatAttackCategoryType attackCategory, std::vector <EMiniGameState> miniGameStateWhenToShowList) : ShapeBoxPreset(type)
{
	mData.mType							= EUIBoxType_MINI_GAME_PLAYER_ATTACK_BOX;
	mData.mCombatCharacterIndex			= characterIndex;
	mData.mShowDuringAllCharacters		= showDuringAllCharacters;
	mData.mAttackNum					= attackNum;
	mData.mAttackCategory				= attackCategory;
	mData.mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
}


// HEALTH BOXES
HealthBoxPreset::HealthBoxPreset(int characterIndex, ECharacterStatBoxValueToDisplay stat) 
{
	mData.mType = EUIBoxType_MINI_GAME_CHARACTER_BOX;
	mData.mCombatCharacterIndex = characterIndex; 
	mData.mCharacterStatToDisplay = stat;
	mData.mShowDuringAllCharacters = true;
	mData.mMiniGameStateWhenToShowList = allMiniStates;
}