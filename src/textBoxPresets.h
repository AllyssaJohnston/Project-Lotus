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
	MiniGameCharacterBoxPreset(int characterIndex,												bool showDuringAllCharacters, ECharacterStatBoxValueToDisplay characterStatToDisplay);
	MiniGameCharacterBoxPreset(int characterIndex,												bool showDuringAllCharacters, ECharacterStatBoxValueToDisplay characterStatToDisplay,	std::vector <EMiniGameState> miniGameStateWhenToShowList);
	MiniGameCharacterBoxPreset(int characterIndex, EMiniGameCombatCharacterType characterType,	bool showDuringAllCharacters, ECharacterStatBoxValueToDisplay characterStatToDisplay,	std::vector <EMiniGameState> miniGameStateWhenToShowList);
	MiniGameCharacterBoxPreset(int characterIndex,												bool showDuringAllCharacters, std::string message,										std::vector <EMiniGameState> miniGameStateWhenToShowList);
};

struct MiniGamePlayerBoxPreset : public TextBoxPreset {	 MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay characterStatToDisplay, std::vector <EMiniGameState> miniGameStateWhenToShowList); };

struct MiniGamePlayerAttackBoxPreset : public TextBoxPreset { MiniGamePlayerAttackBoxPreset(int characterIndex, bool showDuringAllCharacters, ECharacterStatBoxValueToDisplay characterStatToDisplay, int attackNum, EMiniGameCombatAttackCategoryType attackCategory, std::vector <EMiniGameState> miniGameStateWhenToShowList); };

struct MiniGameBoxPreset : public TextBoxPreset { MiniGameBoxPreset(std::string message, std::vector <EMiniGameState> miniGameStateWhenToShowList); };

struct MiniGameDirectionBoxPreset : public TextBoxPreset { MiniGameDirectionBoxPreset(std::string message, std::vector <EMiniGameState> miniGameStateWhenToShowList, EDirection direction); };

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
	EShapeTypeShowType					mShowType					= EShapeTypeShowType_STANDARD;
	int									mCombatCharacterIndex		= -1;
	EMiniGameCombatCharacterType		mCombatCharacterType		= EMiniGameCombatCharacterType_INVALID;
	bool								mShowDuringAllCharacters	= false;
	int									mAttackNum					= -1;
	EMiniGameCombatAttackCategoryType	mAttackCategory				= EMiniGameCombatAttackCategoryType_INVALID;
	EShapeBoxClass						mType						= EShapeBoxClass_INVALID;
	std::vector <EMiniGameState>		mMiniGameStateWhenToShowList;
	

	ShapeBoxPreset(EShapeBoxClass type);
};

struct MiniGameCharacterShapeBoxPreset : public ShapeBoxPreset 
{
	MiniGameCharacterShapeBoxPreset(EShapeBoxClass type, int characterIndex,												bool showDuringAllCharacters);
	MiniGameCharacterShapeBoxPreset(EShapeBoxClass type, int characterIndex,												bool showDuringAllCharacters, std::vector <EMiniGameState> miniGameStateWhenToShowList);
	MiniGameCharacterShapeBoxPreset(EShapeBoxClass type, int characterIndex, EMiniGameCombatCharacterType characterType,	bool showDuringAllCharacters, std::vector <EMiniGameState> miniGameStateWhenToShowList);
	MiniGameCharacterShapeBoxPreset(EShapeBoxClass type, std::vector <EMiniGameState> miniGameStateWhenToShowList);
};

struct MiniGamePlayerAttackShapeBoxPreset : public ShapeBoxPreset { MiniGamePlayerAttackShapeBoxPreset(EShapeBoxClass type, int characterIndex, bool showDuringAllCharacters, int attackNum, EMiniGameCombatAttackCategoryType attackCategory, std::vector <EMiniGameState> miniGameStateWhenToShowList); };


struct HealthBoxPreset
{
	int mCombatCharacterIndex = -1;
	ECharacterStatBoxValueToDisplay mStatToDisplay = ECharacterStatBoxValueToDisplay_INVALID;

	HealthBoxPreset(int characterIndex, ECharacterStatBoxValueToDisplay stat);
};