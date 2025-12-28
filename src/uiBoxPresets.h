#pragma once
#include "uiBoxHelpers.h"
#include "combatHelpers.h"
#include "helpers.h"

// TEXT BOX
struct TextBoxPreset
{
	UIBoxData mData;
	std::string mMessage = "Invalid";

	TextBoxPreset() { ; }
};

struct StandardTextBoxPreset : public TextBoxPreset { StandardTextBoxPreset(std::string message); };

struct MiniGameStatBoxPreset : public TextBoxPreset { MiniGameStatBoxPreset(EUIBoxValueToDisplay gameStateValueToDisplay);  };

struct MiniGameCharacterBoxPreset : public TextBoxPreset
{
	MiniGameCharacterBoxPreset(int characterIndex,												bool showDuringAllCharacters, EUIBoxValueToDisplay characterStatToDisplay);
	MiniGameCharacterBoxPreset(int characterIndex,												bool showDuringAllCharacters, EUIBoxValueToDisplay characterStatToDisplay,	std::vector <EMiniGameState> miniGameStateWhenToShowList);
	MiniGameCharacterBoxPreset(int characterIndex, EMiniGameCombatCharacterType characterType,	bool showDuringAllCharacters, EUIBoxValueToDisplay characterStatToDisplay,	std::vector <EMiniGameState> miniGameStateWhenToShowList);
	MiniGameCharacterBoxPreset(int characterIndex,												bool showDuringAllCharacters, std::string message,							std::vector <EMiniGameState> miniGameStateWhenToShowList);
	MiniGameCharacterBoxPreset(int characterIndex, EMiniGameCombatCharacterType characterType,	bool showDuringAllCharacters, EUIBoxValueToDisplay characterStatToDisplay);
	MiniGameCharacterBoxPreset(int characterIndex,												bool showDuringAllCharacters, std::string message);
};

struct MiniGamePlayerBoxPreset : public TextBoxPreset {	 MiniGamePlayerBoxPreset(EUIBoxValueToDisplay characterStatToDisplay, std::vector <EMiniGameState> miniGameStateWhenToShowList); };

struct MiniGamePlayerAttackBoxPreset : public TextBoxPreset { MiniGamePlayerAttackBoxPreset(int characterIndex, bool showDuringAllCharacters, EUIBoxValueToDisplay characterStatToDisplay, int attackNum, EMiniGameCombatAttackCategoryType attackCategory, std::vector <EMiniGameState> miniGameStateWhenToShowList); };

struct MiniGameBoxPreset : public TextBoxPreset { MiniGameBoxPreset(std::string message, std::vector <EMiniGameState> miniGameStateWhenToShowList); };

struct MiniGameDirectionBoxPreset : public TextBoxPreset { MiniGameDirectionBoxPreset(std::string message, std::vector <EMiniGameState> miniGameStateWhenToShowList, EDirection direction); };

struct GameStatBoxPreset : public TextBoxPreset { GameStatBoxPreset(EUIBoxValueToDisplay gameStateValueToDisplay); };


// IMAGE BOXES
struct ImageBoxPreset
{
	UIBoxData mData;
};

struct DontAutoShowImageBoxPreset : public ImageBoxPreset { DontAutoShowImageBoxPreset(EUIBoxType id); };

struct MiniGameCharacterImageBoxPreset : public ImageBoxPreset 
{
	MiniGameCharacterImageBoxPreset(int characterIndex,												bool showDuringAllCharacters);
	MiniGameCharacterImageBoxPreset(int characterIndex,												bool showDuringAllCharacters,	std::vector <EMiniGameState> miniGameStateWhenToShowList);
	MiniGameCharacterImageBoxPreset(int characterIndex, EMiniGameCombatCharacterType characterType,	bool showDuringAllCharacters,	std::vector <EMiniGameState> miniGameStateWhenToShowList);
};


// SHAPE BOXES
struct ShapeBoxPreset
{
	UIBoxData			mData;
	EShapeBoxClass		mType = EShapeBoxClass_INVALID;

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


// HEALTH BOXES
struct HealthBoxPreset
{
	UIBoxData mData;

	HealthBoxPreset(const int characterIndex, const EUIBoxValueToDisplay stat);
};