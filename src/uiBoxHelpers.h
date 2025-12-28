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

enum EUIBoxValueToDisplay
{
	EUIBoxValueToDisplay_INVALID = -1,
	EUIBoxValueToDisplay_CUR_LEVEL_NUMBER,
	EUIBoxValueToDisplay_CUR_KEYS,
	EUIBoxValueToDisplay_CUR_TARGETS,
	EUIBoxValueToDisplay_CUR_COLLECTIBLES,
	EUIBoxValueToDisplay_CUR_ENEMIES_LEFT,
	EUIBoxValueToDisplay_LEVEL_RESET_KEY,
	EUIBoxValueToDisplay_CHECKPOINT_RESET_KEY,
	EUIBoxValueToDisplay_MOVEMENT_LEFT_KEY,
	EUIBoxValueToDisplay_MOVEMENT_UP_KEY,
	EUIBoxValueToDisplay_MOVEMENT_RIGHT_KEY,
	EUIBoxValueToDisplay_PROJECTILE_HORIZONTAL_KEY,
	EUIBoxValueToDisplay_PROJECTILE_VERTICAL_KEY,
	EUIBoxValueToDisplay_DOUBLE_JUMP_KEY,
	EUIBoxValueToDisplay_SLASH_KEY,

	EUIBoxValueToDisplay_TEXT_SIZE_FACTOR,
	EUIBoxValueToDisplay_CUR_KEYBOARD,

	EUIBoxValueToDisplay_CHARACTER_NAME,
	EUIBoxValueToDisplay_CHARACTER_ATTACK,
	EUIBoxValueToDisplay_CHARACTER_HEALTH,
	EUIBoxValueToDisplay_CHARACTER_HEALTH_MODIFIER_AMOUNT,
	EUIBoxValueToDisplay_CHARACTER_DEFENSE,
	EUIBoxValueToDisplay_CHARACTER_STUN,
	EUIBoxValueToDisplay_CHARACTER_ATTACK_OPTION_NAME,
	EUIBoxValueToDisplay_CHARACTER_ATTACK_OPTION_TYPE,
	EUIBoxValueToDisplay_CHARACTER_ATTACK_OPTION_DAMAGE,
	EUIBoxValueToDisplay_CHARACTER_ATTACK_OPTION_CUR_COOLDOWN,
	EUIBoxValueToDisplay_CHARACTER_ATTACK_OPTION_COOLDOWN_AMOUNT,
	EUIBoxValueToDisplay_CHARACTER_ATTACK_OPTION_SPECIAL_EFFECTS_AND_NOTES,
	EUIBoxValueToDisplay_CHARACTER_CUR_ATTACK_NAME,
	EUIBoxValueToDisplay_CHARACTER_MOVE_TYPE,
	EUIBoxValueToDisplay_MINI_GAME_DEBUG_LINE,
	EUIBoxValueToDisplay_COMBAT_ROUND_NUM,

	EUIBoxValueToDisplay_MAX
};

enum EUIPositionAlign
{
	EUIPositionAlign_INVALID = -1,
	EUIPositionAlign_CENTER,
	EUIPositionAlign_LEFT,
	EUIPositionAlign_TOP,
	EUIPositionAlign_RIGHT,
	EUIPositionAlign_BOTTOM,
	EUIPositionAlign_MAX
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
	EUIBoxValueToDisplay				mGameStatToDisplay			= EUIBoxValueToDisplay_INVALID;
	int									mCombatCharacterIndex		= -1;
	EMiniGameCombatCharacterType        mCombatCharacterType		= EMiniGameCombatCharacterType_CHARACTER;
	bool								mShowDuringAllCharacters	= false;
	int									mAttackNum					= -1;
	EMiniGameCombatAttackCategoryType	mAttackCategory				= EMiniGameCombatAttackCategoryType_INVALID;
	EDirection							mAttackDirection			= EDirection_INVALID;
	EUIBoxType							mType						= EUIBoxType_INVALID;
	std::vector <EMiniGameState>		mMiniGameStateWhenToShowList;

	UIBoxData() { ; }

	UIBoxData(EUIBoxValueToDisplay gameStatToDisplay, int combatCharacterIndex, bool showDuringAllCharacters, int attackNum, EMiniGameCombatAttackCategoryType attackCategory, 
			EUIBoxType mType, std::vector <EMiniGameState>& miniGameStateWhenToShowList);
};

struct UIPositionInfo
{
	int   mRotation = 0;

	Vect2            mPosition;
	EUIPositionAlign mPositionAlignH; // only affects x coord
	EUIPositionAlign mPositionAlignV; // only affects y coord

	int mMaxWidth;
	int mMaxHeight;
	Edges            mMargins;

	UIPositionInfo(EUIPositionAlign positionAlignH, EUIPositionAlign positionAlignV, Hitbox hitbox, Edges margins);

	UIPositionInfo(EUIPositionAlign positionAlignH, EUIPositionAlign positionAlignV, int maxWidth, int maxHeight, Edges margins);

	UIPositionInfo(int rotation, EUIPositionAlign positionAlignH, EUIPositionAlign positionAlignV, Hitbox hitbox, Edges margins);

	UIPositionInfo(int rotation, EUIPositionAlign positionAlignH, EUIPositionAlign positionAlignV, int maxWidth, int maxHeight, Edges margins);
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

struct FontSizeChart
{
	const static int mMinFontSize = 6;
	const static int mMaxFontSize = 150;

	
	// can't easily make static, since all values would have to be provided at start
	// font name maps to a map of font sizes paired with actual sizing details
	std::map<const char*, std::map<int, SDL_Point>> mFontChart;

	void createFontChart(const char*, SDL_Renderer* pRenderer);
};