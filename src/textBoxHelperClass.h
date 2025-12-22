#pragma once
#include "helperClass.h"
#include "hitboxHelper.h"
#include "combatHelperClass.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <string>
#include <map>

enum ETextBoxFunction
{
	ETextBoxFunction_INVALID = -1,
	
	// main game
	ETextBoxFunction_PLAY_GAME_BOX,
	ETextBoxFunction_PLAY_MINI_GAME_BOX,
	ETextBoxFunction_GO_TO_MAIN_MENU_BOX,

	ETextBoxFunction_NO_FUNCTION,

	// For mini game
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

enum ETextBoxType
{
	ETextBoxType_INVALID = -1,
	ETextBoxType_TEXT_BOX,
	ETextBoxType_GAME_STAT_BOX,
	ETextBoxType_MINI_GAME_BOX,
	ETextBoxType_MINI_GAME_DIRECTION_BOX,
	ETextBoxType_MINI_GAME_STAT_BOX,
	ETextBoxType_MINI_GAME_CHARACTER_BOX,
	ETextBoxType_MINI_GAME_PLAYER_ATTACK_BOX,
	ETextBoxType_MINI_GAME_PLAYER_BOX,
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

struct TextBoxData 
{
	EGameStatBoxValueToDisplay			mGameStatToDisplay = EGameStatBoxValueToDisplay_INVALID;
	int									mCombatCharacterIndex = -1;
	EMiniGameCombatCharacterType        mCombatCharacterType = EMiniGameCombatCharacterType_CHARACTER;
	bool								mShowDuringAllCharacters = false;
	ECharacterStatBoxValueToDisplay		mCharacterStatToDisplay = ECharacterStatBoxValueToDisplay_INVALID;
	int									mAttackNum = -1;
	EMiniGameCombatAttackCategoryType	mAttackCategory = EMiniGameCombatAttackCategoryType_INVALID;
	EDirection							mAttackDirection = EDirection_INVALID;
	ETextBoxType						mType = ETextBoxType_INVALID;
	std::vector <EMiniGameState>		mMiniGameStateWhenToShowList;

	TextBoxData() { ; }

	TextBoxData(EGameStatBoxValueToDisplay gameStatToDisplay, int combatCharacterIndex, bool showDuringAllCharacters, ECharacterStatBoxValueToDisplay characterStatToDisplay, 
			int attackNum, EMiniGameCombatAttackCategoryType attackCategory, ETextBoxType mType, std::vector <EMiniGameState>& miniGameStateWhenToShowList);
};

struct TextBoxPositionInfo
{
	Vect2                 mPosition;
	ETextBoxPositionAlign mPositionAlign; // only affects x coord
	ETextBoxTextAlign     mTextAlign;

	int mMaxWidth;
	int mMaxHeight;
	Edges                 mMargins;

	TextBoxPositionInfo(Vect2 position, ETextBoxPositionAlign positionAlign, ETextBoxTextAlign textAlign, int maxWidth, int maxHeight, Edges margins);

	TextBoxPositionInfo(ETextBoxPositionAlign positionAlign, ETextBoxTextAlign textAlign, int maxWidth, int maxHeight, Edges margins);

	TextBoxPositionInfo(Vect2 position, ETextBoxPositionAlign positionAlign, ETextBoxTextAlign textAlign, int maxWidth, int maxHeight);
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
	ETextBoxPositionAlign mPositionAlign;

	int mMaxWidth;
	int mMaxHeight;
	Edges mMargins;

	ImageBoxPositionInfo(Vect2 position, int rotation, ETextBoxPositionAlign positionAlign, int maxWidth, int maxHeight, Edges margins);

	ImageBoxPositionInfo(int rotation, ETextBoxPositionAlign positionAlign, int maxWidth, int maxHeight, Edges margins);

	ImageBoxPositionInfo(Vect2 position, int rotation, ETextBoxPositionAlign positionAlign, int maxWidth, int maxHeight);
};

enum EShapeBoxClass
{
	EShapeBoxClass_INVALID = -1,
	EShapeBoxClass_CIRCLE,
	EShapeBoxClass_RECT,
	EShapeBoxClass_MAX
};

enum EShapeTypeShowType
{
	EShapeTypeShowType_INVALID = -1,
	EShapeTypeShowType_STANDARD,
	EShapeTypeShowType_MINI_GAME_BOX,
	EShapeTypeShowType_MINI_GAME_CHARACTER_BOX,
	EShapeTypeShowType_MINI_GAME_PLAYER_BOX,
	EShapeTypeShowType_MINI_GAME_PLAYER_ATTACK_BOX,
	EShapeTypeShowTypee_MAX
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

