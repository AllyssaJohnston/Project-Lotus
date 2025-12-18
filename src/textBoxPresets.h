#pragma once
#include "textBoxHelperClass.h"
#include "helperClass.h"

// TEXT BOX
struct TextBoxPreset
{
	TextBoxData mData;
	std::string mMessage = "Invalid";

	TextBoxPreset() { ; }
};

struct StandardTextBoxPreset : public TextBoxPreset { StandardTextBoxPreset(std::string message); };

struct MiniGameStatBoxPreset : public TextBoxPreset { MiniGameStatBoxPreset(EGameStatBoxValueToDisplay gameStateValueToDisplay);  };

struct MiniGameCharacterBoxPreset : public TextBoxPreset
{
	MiniGameCharacterBoxPreset(int characterIndex, bool showDuringAllCharacters, ECharacterStatBoxValueToDisplay characterStatToDisplay);
	MiniGameCharacterBoxPreset(int characterIndex, bool showDuringAllCharacters, ECharacterStatBoxValueToDisplay characterStatToDisplay, std::vector <EMiniGameState> miniGameStateWhenToShowList);
	MiniGameCharacterBoxPreset(int characterIndex, bool showDuringAllCharacters, ECharacterStatBoxValueToDisplay characterStatToDisplay, int attackNum, std::vector <EMiniGameState> miniGameStateWhenToShowList);
	MiniGameCharacterBoxPreset(int characterIndex, bool showDuringAllCharacters, std::string message, std::vector <EMiniGameState> miniGameStateWhenToShowList);
};

struct MiniGamePlayerBoxPreset : public TextBoxPreset {	 MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay characterStatToDisplay, std::vector <EMiniGameState> miniGameStateWhenToShowList); };

struct MiniGameBoxPreset : public TextBoxPreset { MiniGameBoxPreset(std::string message, std::vector <EMiniGameState> miniGameStateWhenToShowList); };

struct GameStatBoxPreset : public TextBoxPreset { GameStatBoxPreset(EGameStatBoxValueToDisplay gameStateValueToDisplay); };

// IMAGE BOXES
struct ImageBoxPreset
{
	bool       mAutoShow = true;
	ETextBoxID mID = ETextBoxID_NA;
};

struct DontAutoShowImageBoxPreset : public ImageBoxPreset { DontAutoShowImageBoxPreset(ETextBoxID id); };


// SHAPE BOXES
struct ShapeBoxPreset
{
	EShapeTypeShowType				mShowType					= EShapeTypeShowType_STANDARD;
	int								mCombatCharacterIndex		= -1;
	bool							mShowDuringAllCharacters	= false;
	int								mAttackNum					= -1;
	EShapeBoxClass					mType						= EShapeBoxClass_INVALID;
	std::vector <EMiniGameState>	mMiniGameStateWhenToShowList;
	

	ShapeBoxPreset(EShapeBoxClass type);
};

struct MiniGameShapeBoxPreset : public ShapeBoxPreset 
{
	MiniGameShapeBoxPreset(EShapeBoxClass type, int characterIndex, bool showDuringAllCharacters);
	MiniGameShapeBoxPreset(EShapeBoxClass type, int characterIndex, bool showDuringAllCharacters, std::vector <EMiniGameState> miniGameStateWhenToShowList);
	MiniGameShapeBoxPreset(EShapeBoxClass type, int characterIndex, bool showDuringAllCharacters, int attackNum, std::vector <EMiniGameState> miniGameStateWhenToShowList);
	MiniGameShapeBoxPreset(EShapeBoxClass type, std::vector <EMiniGameState> miniGameStateWhenToShowList);
};


struct HealthBoxPreset
{
	int mCombatCharacterIndex = -1;

	HealthBoxPreset(int characterIndex);
};