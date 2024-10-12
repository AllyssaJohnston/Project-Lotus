#pragma once

enum ETextBoxFunction
{
	ETextBoxFunction_INVALID = -1,
	
	ETextBoxFunction_PLAY_GAME_BOX,
	ETextBoxFunction_PLAY_MINI_GAME_BOX,
	ETextBoxFunction_GO_TO_MAIN_MENU_BOX,

	ETextBoxFunction_NO_FUNCTION,
	ETextBoxFunction_NON_SELECTABLE,

	//For mini game
	ETextBoxFunction_ATTACK_CUR_COMBAT_CHARACTER_BOX,
	ETextBoxFunction_DEFEND_CUR_COMBAT_CHARACTER_BOX,
	ETextBoxFunction_PASS_CUR_COMBAT_CHARACTER_TURN_BOX,

	ETextBoxFunction_ATTACK_STYLE_1_BOX,
	ETextBoxFunction_ATTACK_STYLE_2_BOX,

	ETextBoxFunction_ATTACK_DIRECTION_UP_BOX,
	ETextBoxFunction_ATTACK_DIRECTION_DOWN_BOX,
	ETextBoxFunction_ATTACK_DIRECTION_LEFT_BOX,
	ETextBoxFunction_ATTACK_DIRECTION_RIGHT_BOX,
	
	ETextBoxFunction_MAX
};

enum ETextBoxType
{
	ETextBoxType_INVALID = -1,
	ETextBoxType_TEXT_BOX,
	//ETextBoxType_LEVEL_BOX = 1
	//ETextBoxType_STATS_BOX = 2
	//ETextBoxType_FILE_BOX = 3
	//ETextBoxType_SHOP_BOX = 4
	ETextBoxType_GAME_STAT_BOX,
	ETextBoxType_MINI_GAME_BOX,
	ETextBoxType_MINI_GAME_CHARACTER_BOX,
	ETextBoxType_MINI_GAME_PLAYER_BOX,
	//ETextBoxType_MINI_GAME_OPTION_BOX,
	//ETextBoxType_CHARACTER_STAT_AND_MINI_GAME_OPTION_BOX,
	//ETextBoxType_ACTION_LOG_BOX,
	ETextBoxType_MAX
};

enum EGameStatBoxValueToDisplay
{
	EGameStatBoxValueToDisplay_INVALID = -1,
	EGameStatBoxValueToDisplay_CUR_LEVEL_NUMBER,
	EGameStatBoxValueToDisplay_CUR_KEYS,
	EGameStatBoxValueToDisplay_CUR_TARGETS,
	EGameStatBoxValueToDisplay_CUR_COLLECTIBLES,
	EGameStatBoxValueToDisplay_CUR_ENEMIES_LEFT,
	EGameStatBoxValueToDisplay_LEVEL_RESET_KEY,
	EGameStatBoxValueToDisplay_CHECKPOINT_RESET_KEY,
	EGameStatBoxValueToDisplay_MOVEMENT_LEFT_KEY,
	EGameStatBoxValueToDisplay_MOVEMENT_UP_KEY,
	EGameStatBoxValueToDisplay_MOVEMENT_RIGHT_KEY,
	EGameStatBoxValueToDisplay_PROJECTILE_HORIZONTAL_KEY,
	EGameStatBoxValueToDisplay_PROJECTILE_VERTICAL_KEY,
	EGameStatBoxValueToDisplay_DOUBLE_JUMP_KEY,
	EGameStatBoxValueToDisplay_SLASH_KEY,
	EGameStatBoxValueToDisplay_TEXT_SIZE_FACTOR,
	EGameStatBoxValueToDisplay_MAX
};

enum ECharacterStatBoxValueToDisplay
{
	ECharacterStatBoxValueToDisplay_INVALID = -1,
	ECharacterStatBoxValueToDisplay_CHARACTER_NAME,
	ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK,
	ECharacterStatBoxValueToDisplay_CHARACTER_HEALTH,
	ECharacterStatBoxValueToDisplay_CHARACTER_DEFENSE,
	ECharacterStatBoxValueToDisplay_CHARACTER_STUN,
	ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_1_NAME,
	ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_1_TYPE,
	ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_1_DAMAGE,
	ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_1_SPECIAL_EFFECTS_AND_NOTES,
	ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_2_NAME,
	ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_2_TYPE,
	ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_2_DAMAGE,
	ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_2_SPECIAL_EFFECTS_AND_NOTES,
	ECharacterStatBoxValueToDisplay_CHARACTER_CUR_ATTACK_NAME,
	ECharacterStatBoxValueToDisplay_CHARACTER_MOVE_TYPE,
	ECharacterStatBoxValueToDisplay_MAX
};

enum ETextBoxID
{
	ETextBoxID_INVALID = -1,
	ETextBoxID_NA,
	ETextBoxID_PROJECTILE_UI,
	ETextBoxID_DOUBLE_JUMP_UI,
	ETextBoxID_SLASH_UI,
	ETextBoxID_TAKE_DAMAGE_SCREEN,
	ETextBoxID_MAX
};

enum ETextBoxPositionAlign
{
	ETextBoxPositionAlign_INVALID = -1,
	ETextBoxPositionAlign_CENTER,
	ETextBoxPositionAlign_LEFT,
	ETextBoxPositionAlign_RIGHT,
	ETextBoxPositionAlign_MAX
};

enum ETextBoxTextAlign
{
	ETextBoxTextAlign_INVALID = -1,
	ETextBoxTextAlign_CENTER,
	ETextBoxTextAlign_LEFT,
	ETextBoxTextAlign_RIGHT,
	ETextBoxTextAlign_MAX
};

class TextBoxPositionInfo
{
public:
	Vect2                 mPosition;
	ETextBoxPositionAlign mPositionAlign; //only affects x coord
	ETextBoxTextAlign     mTextAlign;

	int mMaxWidth;
	int mMaxHeight;
	Edges                 mMargins;

	TextBoxPositionInfo(Vect2 position, ETextBoxPositionAlign positionAlign, ETextBoxTextAlign textAlign, int maxWidth, int maxHeight, Edges margins)
	{
		mPosition      = position;
		mPositionAlign = positionAlign;
		mTextAlign     = textAlign;
		mMaxWidth      = maxWidth;
		mMaxHeight     = maxHeight;
		mMargins       = margins;
	}

	TextBoxPositionInfo(ETextBoxPositionAlign positionAlign, ETextBoxTextAlign textAlign, int maxWidth, int maxHeight, Edges margins)
	{
		mPositionAlign = positionAlign;
		if (mPositionAlign == ETextBoxPositionAlign_LEFT)
		{
			mPosition = Vect2(0, 0);
		}
		else if (mPositionAlign == ETextBoxPositionAlign_CENTER)
		{
			mPosition = Vect2(maxWidth, maxHeight);
		}
		mTextAlign     = textAlign;
		mMaxWidth      = maxWidth;
		mMaxHeight     = maxHeight;
		mMargins       = margins;
	}

	TextBoxPositionInfo(Vect2 position, ETextBoxPositionAlign positionAlign, ETextBoxTextAlign textAlign, int maxWidth, int maxHeight)
	{
		mPosition      = position;
		mPositionAlign = positionAlign;
		mTextAlign     = textAlign;
		mMaxWidth      = maxWidth;
		mMaxHeight     = maxHeight;
		mMargins       = Edges();
	}
};

class TextBoxColorInfo
{
public:
	SDL_Color mStandardTextColor;
	SDL_Color mHighlightedTextColor;
	SDL_Color mStandardTextBoxColor;
	SDL_Color mHighlightedTextBoxColor;

	TextBoxColorInfo(SDL_Color standardTextColor, SDL_Color highlightedTextColor, SDL_Color standardTextBoxColor, SDL_Color highlightedTextBoxColor)
	{
		mStandardTextColor       = standardTextColor;
		mHighlightedTextColor    = highlightedTextColor;
		mStandardTextBoxColor    = standardTextBoxColor;
		mHighlightedTextBoxColor = highlightedTextBoxColor;
	}

	TextBoxColorInfo(SDL_Color standardTextColor, SDL_Color standardTextBoxColor, SDL_Color highlightedTextBoxColor)
	{
		mStandardTextColor       = standardTextColor;
		mHighlightedTextColor    = standardTextColor;
		mStandardTextBoxColor    = standardTextBoxColor;
		mHighlightedTextBoxColor = highlightedTextBoxColor;
	}

	TextBoxColorInfo(SDL_Color standardTextColor, SDL_Color standardTextBoxColor)
	{
		mStandardTextColor       = standardTextColor;
		mHighlightedTextColor    = standardTextColor;
		mStandardTextBoxColor    = standardTextBoxColor;
		mHighlightedTextBoxColor = standardTextBoxColor;
	}

	TextBoxColorInfo(SDL_Color standardTextColor)
	{
		mStandardTextColor       = standardTextColor;
		mHighlightedTextColor    = standardTextColor;
		mStandardTextBoxColor    = { 0, 0, 0, 0 };
		mHighlightedTextBoxColor = { 0, 0, 0, 0 };
	}
};

class ImageBoxPositionInfo
{
public:
	Vect2 mPosition;
	int   mRotation;
	ETextBoxPositionAlign mPositionAlign;

	int mMaxWidth;
	int mMaxHeight;
	Edges mMargins;

	ImageBoxPositionInfo(Vect2 position, int rotation, ETextBoxPositionAlign positionAlign, int maxWidth, int maxHeight, Edges margins)
	{
		mPosition      = position;
		mRotation      = rotation;
		mPositionAlign = positionAlign;

		mMaxWidth      = maxWidth;
		mMaxHeight     = maxHeight;
		mMargins       = margins;
	}

	ImageBoxPositionInfo(int rotation, ETextBoxPositionAlign positionAlign, int maxWidth, int maxHeight, Edges margins)
	{
		if (mPositionAlign == ETextBoxPositionAlign_LEFT)
		{
			mPosition = Vect2(0, 0);
		}
		else if (mPositionAlign == ETextBoxPositionAlign_CENTER)
		{
			mPosition = Vect2(maxWidth, maxHeight);
		}
		mRotation      = rotation;
		mPositionAlign = positionAlign;

		mMaxWidth      = maxWidth;
		mMaxHeight     = maxHeight;
		mMargins       = margins;
	}

	ImageBoxPositionInfo(Vect2 position, int rotation, ETextBoxPositionAlign positionAlign, int maxWidth, int maxHeight)
	{
		mPosition      = position;
		mRotation      = rotation;
		mPositionAlign = positionAlign;

		mMaxWidth      = maxWidth;
		mMaxHeight     = maxHeight;
		mMargins       = Edges();
	}
};