#include "uiBoxPresets.h"

// TEXT BOXES
StandardTextBoxPreset::StandardTextBoxPreset(std::string message) : TextBoxPreset()
{ 
	mMessage = message; 
	mData = UIBoxData();
	mData.mType = EUIBoxType_STANDARD;
}

MiniGameStatBoxPreset::MiniGameStatBoxPreset(EUIBoxValueToDisplay gameStateValueToDisplay): TextBoxPreset()
{ 
	mData.mGameStatToDisplay = gameStateValueToDisplay; 
	mData.mType = EUIBoxType_MINI_GAME_STAT_BOX;
}

MiniGameCharacterBoxPreset::MiniGameCharacterBoxPreset(int characterIndex, bool showDuringAllCharacters, EUIBoxValueToDisplay characterStatToDisplay) : TextBoxPreset()
{
	mData.mCombatCharacterIndex			= characterIndex;
	mData.mCombatCharacterType			= ECombatCharacterType_CHARACTER;
	mData.mShowDuringAllCharacters		= showDuringAllCharacters;
	mData.mGameStatToDisplay			= characterStatToDisplay;
	mData.mMiniGameStateWhenToShowList	= allMiniStates;
	mData.mType							= EUIBoxType_MINI_GAME_CHARACTER_BOX;
}

MiniGameCharacterBoxPreset::MiniGameCharacterBoxPreset(int characterIndex, bool showDuringAllCharacters, EUIBoxValueToDisplay characterStatToDisplay, std::vector <EMiniGameState> miniGameStateWhenToShowList) : TextBoxPreset()
{
	mData.mCombatCharacterIndex			= characterIndex;
	mData.mCombatCharacterType			= ECombatCharacterType_CHARACTER;
	mData.mShowDuringAllCharacters		= showDuringAllCharacters;
	mData.mGameStatToDisplay			= characterStatToDisplay;
	mData.mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
	mData.mType							= EUIBoxType_MINI_GAME_CHARACTER_BOX;
}

MiniGameCharacterBoxPreset::MiniGameCharacterBoxPreset(int characterIndex, ECombatCharacterType characterType, bool showDuringAllCharacters, EUIBoxValueToDisplay characterStatToDisplay,
		std::vector <EMiniGameState> miniGameStateWhenToShowList)
{
	mData.mCombatCharacterIndex			= characterIndex;
	mData.mCombatCharacterType			= characterType;
	mData.mShowDuringAllCharacters		= showDuringAllCharacters;
	mData.mGameStatToDisplay			= characterStatToDisplay;
	mData.mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
	mData.mType							= EUIBoxType_MINI_GAME_CHARACTER_BOX;
}


MiniGameCharacterBoxPreset::MiniGameCharacterBoxPreset(int characterIndex, bool showDuringAllCharacters, std::string message, std::vector <EMiniGameState> miniGameStateWhenToShowList) : TextBoxPreset()
{
	mData.mCombatCharacterIndex			= characterIndex;
	mData.mCombatCharacterType			= ECombatCharacterType_CHARACTER;
	mData.mShowDuringAllCharacters		= showDuringAllCharacters;
	mData.mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
	mData.mType							= EUIBoxType_MINI_GAME_CHARACTER_BOX;
	mMessage							= message;
}

MiniGameCharacterBoxPreset::MiniGameCharacterBoxPreset(int characterIndex, ECombatCharacterType characterType, bool showDuringAllCharacters, EUIBoxValueToDisplay characterStatToDisplay)
{
	mData.mCombatCharacterIndex			= characterIndex;
	mData.mCombatCharacterType			= characterType;
	mData.mShowDuringAllCharacters		= showDuringAllCharacters;
	mData.mGameStatToDisplay			= characterStatToDisplay;
	mData.mMiniGameStateWhenToShowList	= allMiniStates;
	mData.mType							= EUIBoxType_MINI_GAME_CHARACTER_BOX;
}

MiniGameCharacterBoxPreset::MiniGameCharacterBoxPreset(int characterIndex, bool showDuringAllCharacters, std::string message)
{
	mData.mCombatCharacterIndex			= characterIndex;
	mData.mCombatCharacterType			= ECombatCharacterType_CHARACTER;
	mData.mMiniGameStateWhenToShowList	= allMiniStates;
	mData.mType							= EUIBoxType_MINI_GAME_CHARACTER_BOX;
	mData.mShowDuringAllCharacters		= showDuringAllCharacters;
	mMessage							= message;
}


MiniGamePlayerBoxPreset::MiniGamePlayerBoxPreset(EUIBoxValueToDisplay characterStatToDisplay, std::vector <EMiniGameState> miniGameStateWhenToShowList) : TextBoxPreset()
{
	mData.mGameStatToDisplay			= characterStatToDisplay;
	mData.mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
	mData.mType							= EUIBoxType_MINI_GAME_PLAYER_BOX;
}

MiniGamePlayerAttackBoxPreset::MiniGamePlayerAttackBoxPreset(int characterIndex, bool showDuringAllCharacters, EUIBoxValueToDisplay characterStatToDisplay, int attackNum,
	ECombatAttackCategoryType attackCategory, std::vector <EMiniGameState> miniGameStateWhenToShowList) : TextBoxPreset()
{
	mData.mCombatCharacterIndex			= characterIndex;
	mData.mGameStatToDisplay			= characterStatToDisplay;
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


GameStatBoxPreset::GameStatBoxPreset(EUIBoxValueToDisplay gameStateValueToDisplay) : TextBoxPreset()
{
	mData.mGameStatToDisplay	= gameStateValueToDisplay;
	mData.mType					= EUIBoxType_GAME_STAT_BOX;
}


// IMAGE BOXES
DontAutoShowImageBoxPreset::DontAutoShowImageBoxPreset(EUIBoxType id) : ImageBoxPreset() { mData.mType	= id; }

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

MiniGameCharacterImageBoxPreset::MiniGameCharacterImageBoxPreset(int characterIndex, ECombatCharacterType characterType, bool showDuringAllCharacters, std::vector <EMiniGameState> miniGameStateWhenToShowList) : ImageBoxPreset()
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
	mData.mCombatCharacterType		= ECombatCharacterType_CHARACTER;
	mData.mShowDuringAllCharacters	= showDuringAllCharacters;
}

MiniGameCharacterShapeBoxPreset::MiniGameCharacterShapeBoxPreset(EShapeBoxClass type, int characterIndex, bool showDuringAllCharacters,
		std::vector <EMiniGameState> miniGameStateWhenToShowList) : ShapeBoxPreset(type)
{
	mData.mType							= EUIBoxType_MINI_GAME_CHARACTER_BOX;
	mData.mCombatCharacterIndex			= characterIndex;
	mData.mCombatCharacterType			= ECombatCharacterType_CHARACTER;
	mData.mShowDuringAllCharacters		= showDuringAllCharacters;
	mData.mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
}

MiniGameCharacterShapeBoxPreset::MiniGameCharacterShapeBoxPreset(EShapeBoxClass type, int characterIndex, ECombatCharacterType characterType, bool showDuringAllCharacters, 
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
	mData.mCombatCharacterType			= ECombatCharacterType_CHARACTER;
	mData.mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
}

MiniGamePlayerAttackShapeBoxPreset::MiniGamePlayerAttackShapeBoxPreset(EShapeBoxClass type, int characterIndex, bool showDuringAllCharacters, int attackNum,
	ECombatAttackCategoryType attackCategory, std::vector <EMiniGameState> miniGameStateWhenToShowList) : ShapeBoxPreset(type)
{
	mData.mType							= EUIBoxType_MINI_GAME_PLAYER_ATTACK_BOX;
	mData.mCombatCharacterIndex			= characterIndex;
	mData.mShowDuringAllCharacters		= showDuringAllCharacters;
	mData.mAttackNum					= attackNum;
	mData.mAttackCategory				= attackCategory;
	mData.mMiniGameStateWhenToShowList	= miniGameStateWhenToShowList;
}


// HEALTH BOXES
HealthBoxPreset::HealthBoxPreset(const int characterIndex, const EUIBoxValueToDisplay stat)
{
	mData.mType = EUIBoxType_MINI_GAME_CHARACTER_BOX;
	mData.mCombatCharacterIndex			= characterIndex; 
	mData.mGameStatToDisplay			= stat;
	mData.mShowDuringAllCharacters		= true;
	mData.mMiniGameStateWhenToShowList	= allMiniStates;
}