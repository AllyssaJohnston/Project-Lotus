#pragma once
#include "textBoxHelperClass.h"
#include "helperClass.h"

//TEXT BOX
class TextBoxPreset
{
public:
	EGameStatBoxValueToDisplay		mGameStatToDisplay		= EGameStatBoxValueToDisplay_INVALID;
	int								mCombatCharacterIndex	= -1;
	ECharacterStatBoxValueToDisplay mCharacterStatToDisplay = ECharacterStatBoxValueToDisplay_INVALID;
	ETextBoxType					mType					= ETextBoxType_INVALID;
	std::vector <EMiniGameState>	mMiniGameStateWhenToShowList;
	std::string                     mMessage				= "Invalid";

};

class StandardTextBoxPreset : public TextBoxPreset
{
public:
	StandardTextBoxPreset(std::string message)
	{
		mMessage = message;
	}
};

class MiniGameCharacterBoxPreset : public TextBoxPreset
{
public: 
	MiniGameCharacterBoxPreset(int characterIndex, ECharacterStatBoxValueToDisplay characterStatToDisplay) : TextBoxPreset()
	{
		mCombatCharacterIndex   = characterIndex;
		mCharacterStatToDisplay = characterStatToDisplay;
		mType                   = ETextBoxType_MINI_GAME_CHARACTER_BOX;
		mMessage                = "Z"; //default
	}
};

class MiniGamePlayerBoxPreset : public TextBoxPreset
{
public:
	MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay characterStatToDisplay, std::vector <EMiniGameState> miniGameStateWhenToShowList) : TextBoxPreset()
	{
		mCharacterStatToDisplay		   = characterStatToDisplay;
		mMiniGameStateWhenToShowList   = miniGameStateWhenToShowList;
		mType						   = ETextBoxType_MINI_GAME_PLAYER_BOX;
		mMessage					   = "Z"; //default
	}
};

class MiniGameBoxPreset : public TextBoxPreset
{
public:
	MiniGameBoxPreset(std::string message, std::vector <EMiniGameState> miniGameStateWhenToShowList) : TextBoxPreset()
	{
		mMiniGameStateWhenToShowList   = miniGameStateWhenToShowList;
		mType						   = ETextBoxType_MINI_GAME_BOX;
		mMessage					   = message;
	}
};

class GameStatBoxPreset : public TextBoxPreset
{
public:
	GameStatBoxPreset(EGameStatBoxValueToDisplay gameStateValueToDisplay) : TextBoxPreset()
	{
		mGameStatToDisplay             = gameStateValueToDisplay;
		mType						   = ETextBoxType_GAME_STAT_BOX;
		mMessage					   = "Z"; //default
	}
};

//IMAGE BOXES
class ImageBoxPreset
{
public:
	bool       mAutoShow = true;
	ETextBoxID mID = ETextBoxID_NA;
};

class DontAutoShowImageBoxPreset : public ImageBoxPreset
{
public:
	DontAutoShowImageBoxPreset(ETextBoxID id) : ImageBoxPreset()
	{
		mAutoShow = false;
		mID = id;
	}
};