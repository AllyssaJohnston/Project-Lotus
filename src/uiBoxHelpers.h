#pragma once
#include "helpers.h"
#include "hitbox.h"
#include "combatHelpers.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <string>
#include <map>

enum EUIClass
{
	EUIClass_INVALID = -1,
	EUIClass_ELEMENT,
	EUIClass_BLOCK,
	EUIClass_BOX,
	EUIClass_MAX
};

enum EUIBoxClass
{
	EUIBoxClass_INVALID = -1,
	EUIBoxClass_TEXTBOX,
	EUIBoxClass_IMAGEBOX,
	EUIBoxClass_SHAPEBOX,
	EUIBoxClass_HEALTHBOX,
	EUIBoxClass_MAX
};

enum ETextBoxFunction
{
	ETextBoxFunction_INVALID = -1,
	
	// main game
	ETextBoxFunction_PLAY_GAME_BOX,
	ETextBoxFunction_PLAY_MINI_GAME_BOX,
	ETextBoxFunction_GO_TO_MAIN_MENU_BOX,

	ETextBoxFunction_NO_FUNCTION,

	// For mini game
	ETextBoxFunction_MINI_GAME_UNDO_BOX,

	ETextBoxFunction_ATTACK_CUR_COMBAT_CHARACTER_BOX,
	ETextBoxFunction_SUPPORT_CUR_COMBAT_CHARACTER_BOX,
	ETextBoxFunction_DEFEND_CUR_COMBAT_CHARACTER_BOX,
	ETextBoxFunction_HEAL_CUR_COMBAT_CHARACTER_BOX,
	ETextBoxFunction_PASS_CUR_COMBAT_CHARACTER_TURN_BOX,

	ETextBoxFunction_ATTACK_STYLE_BOX,
	ETextBoxFunction_ATTACK_CHARACTER_BOX,
	ETextBoxFunction_ATTACK_DIRECTION_BOX,
	
	ETextBoxFunction_MAX
};

enum EUIBoxType
{
	EUIBoxType_INVALID = -1,
	EUIBoxType_STANDARD,
	EUIBoxType_GAME_STAT_BOX,
	EUIBoxType_MINI_GAME_BOX,
	EUIBoxType_MINI_GAME_DIRECTION_BOX,
	EUIBoxType_MINI_GAME_STAT_BOX,
	EUIBoxType_MINI_GAME_CHARACTER_BOX,
	EUIBoxType_MINI_GAME_PLAYER_ATTACK_BOX,
	EUIBoxType_MINI_GAME_PLAYER_BOX,
	EUIBoxType_PROJECTILE_UI,
	EUIBoxType_DOUBLE_JUMP_UI,
	EUIBoxType_SLASH_UI,
	EUIBoxType_TAKE_DAMAGE_SCREEN,
	EUIBoxType_MAX
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
	EGameStatBoxValueToDisplay_CUR_KEYBOARD,
	EGameStatBoxValueToDisplay_MINI_GAME_DEBUG_LINE,
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
	ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_NAME,
	ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_TYPE,
	ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_DAMAGE,
	ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_SPECIAL_EFFECTS_AND_NOTES,
	ECharacterStatBoxValueToDisplay_CHARACTER_CUR_ATTACK_NAME,
	ECharacterStatBoxValueToDisplay_CHARACTER_MOVE_TYPE,
	ECharacterStatBoxValueToDisplay_MAX
};

enum ETextBoxPositionAlign
{
	ETextBoxPositionAlign_INVALID = -1,
	ETextBoxPositionAlign_CENTER,
	ETextBoxPositionAlign_LEFT,
	ETextBoxPositionAlign_TOP,
	ETextBoxPositionAlign_RIGHT,
	ETextBoxPositionAlign_BOTTOM,
	ETextBoxPositionAlign_MAX
};

enum EShapeBoxClass
{
	EShapeBoxClass_INVALID = -1,
	EShapeBoxClass_CIRCLE,
	EShapeBoxClass_RECT,
	EShapeBoxClass_MAX
};


struct UIBoxData 
{
	EGameStatBoxValueToDisplay			mGameStatToDisplay			= EGameStatBoxValueToDisplay_INVALID;
	int									mCombatCharacterIndex		= -1;
	EMiniGameCombatCharacterType        mCombatCharacterType		= EMiniGameCombatCharacterType_CHARACTER;
	bool								mShowDuringAllCharacters	= false;
	ECharacterStatBoxValueToDisplay		mCharacterStatToDisplay		= ECharacterStatBoxValueToDisplay_INVALID;
	int									mAttackNum					= -1;
	EMiniGameCombatAttackCategoryType	mAttackCategory				= EMiniGameCombatAttackCategoryType_INVALID;
	EDirection							mAttackDirection			= EDirection_INVALID;
	EUIBoxType							mType						= EUIBoxType_INVALID;
	std::vector <EMiniGameState>		mMiniGameStateWhenToShowList;

	UIBoxData() { ; }

	UIBoxData(EGameStatBoxValueToDisplay gameStatToDisplay, int combatCharacterIndex, bool showDuringAllCharacters, ECharacterStatBoxValueToDisplay characterStatToDisplay,
			int attackNum, EMiniGameCombatAttackCategoryType attackCategory, EUIBoxType mType, std::vector <EMiniGameState>& miniGameStateWhenToShowList);
};

struct TextBoxPositionInfo
{
	Vect2                 mPosition;
	ETextBoxPositionAlign mPositionAlignH; // only affects x coord
	ETextBoxPositionAlign mPositionAlignV; // only affects y coord

	int mMaxWidth;
	int mMaxHeight;
	Edges                 mMargins;

	TextBoxPositionInfo(ETextBoxPositionAlign positionAlignH, ETextBoxPositionAlign positionAlignV, int maxWidth, int maxHeight, Edges margins);
};

struct TextBoxSizeInfo
{
	int	mStandardFontSize;
	int	mHighlightedFontSize;
	int	mOutlineWidth = 0;

	TextBoxSizeInfo(int standardSize, int highlightSize, int outlineWidth);

	TextBoxSizeInfo(int standardSize, int highlightSize);

	TextBoxSizeInfo(int standardSize);
};

struct TextBoxColorInfo
{
	SDL_Color mStandardTextColor;
	SDL_Color mHighlightedTextColor;
	SDL_Color mStandardTextBoxColor;
	SDL_Color mHighlightedTextBoxColor;
	SDL_Color mOutlineColor = { 0, 0, 0, 0 };
	SDL_Color mHighlightedOutlineColor = { 0, 0, 0, 0 };

	TextBoxColorInfo(SDL_Color standardTextColor, SDL_Color highlightedTextColor, SDL_Color standardTextBoxColor, SDL_Color highlightedTextBoxColor, SDL_Color outlineColor, SDL_Color highlightedOutlineColor);

	TextBoxColorInfo(SDL_Color standardTextColor, SDL_Color highlightedTextColor, SDL_Color standardTextBoxColor, SDL_Color highlightedTextBoxColor);

	TextBoxColorInfo(SDL_Color standardTextColor, SDL_Color standardTextBoxColor, SDL_Color highlightedTextBoxColor);

	TextBoxColorInfo(SDL_Color standardTextColor, SDL_Color standardTextBoxColor);

	TextBoxColorInfo(SDL_Color standardTextColor);
};

struct ImageBoxPositionInfo
{
	Vect2 mPosition;
	int   mRotation;
	ETextBoxPositionAlign mPositionAlignH;
	ETextBoxPositionAlign mPositionAlignV;

	int mMaxWidth;
	int mMaxHeight;
	Edges mMargins;

	ImageBoxPositionInfo(int rotation, ETextBoxPositionAlign positionAlignH, ETextBoxPositionAlign positionAlignV, int maxWidth, int maxHeight, Edges margins);
};

struct FontSizeChart
{
	const static int mMinFontSize = 6;
	const static int mMaxFontSize = 150;

	
	// can't easily make static, since all values would have to be provided at start
	// font name maps to a map of font sizes paired with actual sizing details
	std::map<const char*, std::map<int, SDL_Point>> mFontChart;

	void createFontChart(const char*, SDL_Renderer* pRenderer);
};

