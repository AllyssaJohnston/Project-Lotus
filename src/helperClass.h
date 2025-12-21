#pragma once
#include <string>
#include <vector>
#include <map>
#include <SDL3/SDL.h>


enum EKeyboardInput 
{
	EKeyboardInput_INVALID = -1,
	EKeyboardInput_LEFT,
	EKeyboardInput_RIGHT,
	EKeyboardInput_UP,
	EKeyboardInput_DOWN,
	EKeyboardInput_Q,
	EKeyboardInput_W,
	EKeyboardInput_E,
	EKeyboardInput_R,
	EKeyboardInput_T,
	EKeyboardInput_Y,
	EKeyboardInput_U,
	EKeyboardInput_I,
	EKeyboardInput_O,
	EKeyboardInput_P,
	EKeyboardInput_A,
	EKeyboardInput_S,
	EKeyboardInput_D,
	EKeyboardInput_F,
	EKeyboardInput_G,
	EKeyboardInput_H,
	EKeyboardInput_J,
	EKeyboardInput_K,
	EKeyboardInput_L,
	EKeyboardInput_Z,
	EKeyboardInput_X,
	EKeyboardInput_C,
	EKeyboardInput_V,
	EKeyboardInput_B,
	EKeyboardInput_N,
	EKeyboardInput_M,
	EKeyboardInput_1,
	EKeyboardInput_2,
	EKeyboardInput_3,
	EKeyboardInput_4,
	EKeyboardInput_5,
	EKeyboardInput_6,
	EKeyboardInput_7,
	EKeyboardInput_8,
	EKeyboardInput_9,
	EKeyboardInput_0,
	EKeyboardInput_SHIFT,
	EKeyboardInput_CTRL,
	EKeyboardInput_CAPS,
	EKeyboardInput_ENTER,
	EKeyboardInput_ESC,
	EKeyboardInput_SPACE_BAR,
	EKeyboardInput_MOUSE_LEFT,
	EKeyboardInput_MOUSE_MIDDLE,
	EKeyboardInput_MOUSE_RIGHT,
	EKeyboardInput_NONE,
	EKeyboardInput_MAX
};

struct KeyData
{
	int mKey = -1;
	int mRepeat = -1;

	KeyData() { ; }
	KeyData(int key, int repeat);
};

const static int numEventsToGrab = 20;

static const std::map<const SDL_Keycode, const EKeyboardInput> SDLKToKeyboardMap =
{
	{SDLK_LEFT,			EKeyboardInput_LEFT},	{SDLK_RIGHT,		EKeyboardInput_RIGHT},	{SDLK_UP,			EKeyboardInput_UP},
	{SDLK_DOWN,			EKeyboardInput_DOWN},	{SDLK_q,			EKeyboardInput_Q},		{SDLK_w,			EKeyboardInput_W},
	{SDLK_e,			EKeyboardInput_E},		{SDLK_r,			EKeyboardInput_R},		{SDLK_t,			EKeyboardInput_T},
	{SDLK_y,			EKeyboardInput_Y},		{SDLK_u,			EKeyboardInput_U},		{SDLK_i,			EKeyboardInput_I},
	{SDLK_o,			EKeyboardInput_O},		{SDLK_p,			EKeyboardInput_P},		{SDLK_a,			EKeyboardInput_A},
	{SDLK_s,			EKeyboardInput_S},		{SDLK_d,			EKeyboardInput_D},		{SDLK_f,			EKeyboardInput_F},
	{SDLK_g,			EKeyboardInput_G},		{SDLK_h,			EKeyboardInput_H},		{SDLK_j,			EKeyboardInput_J},
	{SDLK_k,			EKeyboardInput_K},		{SDLK_l,			EKeyboardInput_L},		{SDLK_z,			EKeyboardInput_Z},
	{SDLK_x,			EKeyboardInput_X},		{SDLK_c,			EKeyboardInput_C},		{SDLK_v,			EKeyboardInput_V},
	{SDLK_b,			EKeyboardInput_B},		{SDLK_n,			EKeyboardInput_N},		{SDLK_m,			EKeyboardInput_M},
	{SDLK_1,			EKeyboardInput_1},		{SDLK_2,			EKeyboardInput_2},		{SDLK_3,			EKeyboardInput_3},
	{SDLK_4,			EKeyboardInput_4},		{SDLK_5,			EKeyboardInput_5},		{SDLK_6,			EKeyboardInput_6},
	{SDLK_7,			EKeyboardInput_7},		{SDLK_8,			EKeyboardInput_8},		{SDLK_9,			EKeyboardInput_9},
	{SDLK_0,			EKeyboardInput_0},		{SDLK_LSHIFT,		EKeyboardInput_SHIFT},	{SDLK_CAPSLOCK,		EKeyboardInput_CAPS},
	{SDLK_LCTRL,		EKeyboardInput_CTRL},	{SDLK_RETURN,		EKeyboardInput_ENTER},	{SDLK_ESCAPE,		EKeyboardInput_ESC},
	{SDLK_SPACE,		EKeyboardInput_SPACE_BAR}
};

static const std::map<const EKeyboardInput, const std::string> keyboardToStringMap =
{
	{EKeyboardInput_LEFT,			"left"},		{EKeyboardInput_RIGHT,			"right"},		{EKeyboardInput_UP, 			"up"},
	{EKeyboardInput_DOWN, 			"down"},		{EKeyboardInput_Q, 				"Q"},			{EKeyboardInput_W, 				"W"},
	{EKeyboardInput_E, 				"E"},			{EKeyboardInput_R, 				"R"},			{EKeyboardInput_T, 				"T"},
	{EKeyboardInput_Y, 				"Y"},			{EKeyboardInput_U, 				"U"},			{EKeyboardInput_I, 				"I"},
	{EKeyboardInput_O, 				"O"},			{EKeyboardInput_P, 				"P"},			{EKeyboardInput_A, 				"A"},
	{EKeyboardInput_S, 				"S"},			{EKeyboardInput_D, 				"D"},			{EKeyboardInput_F, 				"F"},
	{EKeyboardInput_G, 				"G"},			{EKeyboardInput_H, 				"H"},			{EKeyboardInput_J, 				"J"},
	{EKeyboardInput_K, 				"K"},			{EKeyboardInput_L, 				"L"},			{EKeyboardInput_Z, 				"Z"},
	{EKeyboardInput_X, 				"X"},			{EKeyboardInput_C, 				"C"},			{EKeyboardInput_V, 				"V"},
	{EKeyboardInput_B, 				"B"},			{EKeyboardInput_N, 				"N"},			{EKeyboardInput_M, 				"M"},
	{EKeyboardInput_1, 				"1"},			{EKeyboardInput_2, 				"2"},			{EKeyboardInput_3,				"3"},
	{EKeyboardInput_4,				"4"},			{EKeyboardInput_5,				"5"},			{EKeyboardInput_6,				"6"},
	{EKeyboardInput_7,				"7"},			{EKeyboardInput_8,				"8"},			{EKeyboardInput_9,				"9"},
	{EKeyboardInput_0,				"0"},			{EKeyboardInput_SHIFT,			"shift"},		{EKeyboardInput_CAPS,			"caps"},
	{EKeyboardInput_CTRL,			"ctrl"},		{EKeyboardInput_ENTER,			"enter"},		{EKeyboardInput_ESC,			"esc"},
	{EKeyboardInput_SPACE_BAR,		"space"},		{EKeyboardInput_MOUSE_LEFT,		"mouse-left"},	{EKeyboardInput_MOUSE_MIDDLE,	"mouse-middle"},
	{EKeyboardInput_MOUSE_RIGHT,	"mouse-right"}
};


struct KeyboardData
{
	int				mNumKeys = (int)EKeyboardInput_MAX;
	SDL_Event       mEvents [numEventsToGrab];
	bool            mLastFrameKeyState[(int)EKeyboardInput_MAX];
	bool	        mKeyState[(int)EKeyboardInput_MAX];
	int 	        mKeyStateRepeat[(int)EKeyboardInput_MAX];
	std::string		mCurKeysString;
};

enum EDirection 
{
	EDirection_INVALID = -1,
	EDirection_LEFT,
	EDirection_RIGHT,
	EDirection_UP,
	EDirection_DOWN,
	EDirection_NONE,
	EDirection_LEFT_AND_RIGHT,
	EDirection_UP_AND_DOWN,
	EDirection_ALL,
	EDirection_MAX
};



// Types of Hitbox Edges
enum EEntityEdgeType
{
	EEntityEdgeType_INVALID = -1,
	EEntityEdgeType_NEUTRAL,
	EEntityEdgeType_NON_EXISTENT,
	EEntityEdgeType_HAZARDOUS,
	EEntityEdgeType_SPIKEY,
	EEntityEdgeType_BURNING,
	EEntityEdgeType_ELECTRIC,
	EEntityEdgeType_SLIPPERY,
	EEntityEdgeType_STICKY,
	EEntityEdgeType_ICY,
	EEntityEdgeType_MAGNETIC,
	EEntityEdgeType_BOUNCY,
	EEntityEdgeType_MOVEABLE,
	EEntityEdgeType_CRUSHABLE,
	EEntityEdgeType_CRUMBLING,
	EEntityEdgeType_TELEPORTER,
	EEntityEdgeType_LOCKED,
	EEntityEdgeType_WEIGHT_SENSITIVE,
	EEntityEdgeType_WALL_JUMPABLE,
	EEntityEdgeType_GRABBING,
	EEntityEdgeType_MAX
};


enum EGameState
{
	EGameState_INVALID = -1,
	EGameState_PLAY,
	EGameState_PLAY_MINI_GAME,
	EGameState_MENU,
	EGameState_GAME_LOAD,
	EGameState_MAX
};

enum EMiniGameState
{
	EMiniGameState_INVALID = -1,
	EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT,
	EMiniGameState_PLAYER_MOVE_CHARACTER,
	EMiniGameState_PLAYER_WAIT_FOR_ACTION_INPUT,
	EMiniGameState_PLAYER_WAIT_FOR_ATTACK_OPTION_INPUT,
	EMiniGameState_PLAYER_WAIT_FOR_ATTACK_DIRECTION_INPUT,
	EMiniGameState_PLAYER_WAIT_FOR_ATTACK_TILE_INPUT,
	EMiniGameState_PLAYER_COMPLETE_ACTION_ATTACK,
	EMiniGameState_PLAYER_COMPLETE_ACTION_DEFEND,
	EMiniGameState_PLAYER_COMPLETE_ACTION_HEAL,
	EMiniGameState_ENEMY_MOVE_CHARACTER,
	EMiniGameState_ENEMY_TAKE_ACTION,
	EMiniGameState_CHARACTER_STUNNED,
	EMiniGameState_BUFFER,
	EMiniGameState_EXIT,
	EMiniGameState_BUILD_NEXT_LEVEL,
	EMiniGameState_MAX
};

enum EEntityCharacterTypes
{
	EEntityCharacterTypes_INVALID = -1,
	EEntityCharacterTypes_C_ENTITY,
	EEntityCharacterTypes_C_CHARACTER,
	EEntityCharacterTypes_C_PLAYER,
	EEntityCharacterTypes_E_RAT,
	EEntityCharacterTypes_E_FAST_RAT,
	EEntityCharacterTypes_E_BOUNCY_RAT,
	EEntityCharacterTypes_E_HIDING_RAT,
	EEntityCharacterTypes_E_FROG,
	EEntityCharacterTypes_E_BOUNCY_FROG,
	EEntityCharacterTypes_E_FIRE_RAT,
	EEntityCharacterTypes_E_FIRE_BAT_V,
	EEntityCharacterTypes_E_FIRE_FROG,
	EEntityCharacterTypes_E_SNAIL,
	EEntityCharacterTypes_E_SPIKEY_SNAIL,
	EEntityCharacterTypes_E_SPIKEY_SNAIL_CHAMELEON,
	EEntityCharacterTypes_E_BAT_H,
	EEntityCharacterTypes_E_BAT_V,
	EEntityCharacterTypes_E_BAT_D,
	EEntityCharacterTypes_E_HIDING_SNAIL,
	EEntityCharacterTypes_E_GRABBING_SNAIL,
	EEntityCharacterTypes_E_WALKING_FROG,
	EEntityCharacterTypes_P_STANDARD,
	EEntityCharacterTypes_P_SOLID,
	EEntityCharacterTypes_P_STICKY,
	EEntityCharacterTypes_P_ICY,
	EEntityCharacterTypes_P_MAGNETIC,
	EEntityCharacterTypes_P_ELECTRIC,
	EEntityCharacterTypes_P_BOUNCY,
	EEntityCharacterTypes_P_WALL_JUMPABLE,
	EEntityCharacterTypes_P_SPIKE,
	EEntityCharacterTypes_P_LAVA,
	EEntityCharacterTypes_P_CRUMBLING,
	EEntityCharacterTypes_P_GATE,
	EEntityCharacterTypes_P_TARGET,
	EEntityCharacterTypes_P_TARGET_GATE,
	EEntityCharacterTypes_P_PRESSURE_OPERATED_GATE,
	EEntityCharacterTypes_P_CRATE,
	EEntityCharacterTypes_P_ARMORED_CRATE,
	EEntityCharacterTypes_P_MAGNETIC_CRATE,
	EEntityCharacterTypes_P_MOVING_PLATFORM,
	EEntityCharacterTypes_P_PRESSURE_PLATE,
	EEntityCharacterTypes_P_WIND_GUST,
	EEntityCharacterTypes_PJ_PLAYER_PROJECTILE,
	EEntityCharacterTypes_PJ_ENEMY_PROJECTILE,
	EEntityCharacterTypes_C_SAVE_POINT,
	EEntityCharacterTypes_C_KEY,
	EEntityCharacterTypes_C_END_OF_LEVEL,
	EEntityCharacterTypes_C_MINI_GAME_LEVEL,
	EEntityCharacterTypes_C_LOTUS_COLLECTIBLE,
	EEntityCharacterTypes_MAX
};

enum EEntityClassTypes
{
	EEntityClassTypes_INVALID = -1,
	EEntityClassTypes_ENTITY,
	EEntityClassTypes_PLAYER,
	EEntityClassTypes_ENEMY,
	EEntityClassTypes_PLATFORM,
	EEntityClassTypes_COLLECTIBLE,
	EEntityClassTypes_PROJECTILE,
	EEntityClassTypes_MAX
};

enum EEntitySubClassTypes
{
	EEntitySubClassTypes_INVALID = -1,
	EEntitySubClassTypes_NA,
	EEntitySubClassTypes_CRATE,
	EEntitySubClassTypes_AREA_EFFECT,
	EEntitySubClassTypes_MAX
};

enum EEnemyType
{
	EEnemyType_INVALID = -1,
	EEnemyType_STANDARD,
	EEnemyType_MULTISTAGED,
	EEnemyType_MAX
};

enum EEntityCharacteristicsTypes
{
	EEntityCharacteristicsTypes_INVALID = -1,
	EEntityCharacteristicsTypes_CRUSH_RESISTENT,
	EEntityCharacteristicsTypes_SPIKE_RESISTENT,
	EEntityCharacteristicsTypes_FIRE_RESISTENT,
	EEntityCharacteristicsTypes_ELECTRICITY_RESISTENT,
	EEntityCharacteristicsTypes_MAGNETIC,
	EEntityCharacteristicsTypes_SLIPPERY,
	EEntityCharacteristicsTypes_NONE,
	EEntityCharacteristicsTypes_INVINCIBLE,
	EEntityCharacteristicsTypes_MAX
};

enum EHowToDetermineWidthHeight
{
	EHowToDetermineWidthHeight_INVALID = -1,
	EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT,
	EHowToDetermineWidthHeight_GET_BEST_IMAGE_RATIO,
	EHowToDetermineWidthHeight_USE_IMAGE_RATIO_INPUT,
	EHowToDetermineWidthHeight_CHUNK_IMAGES,
	EHowToDetermineWidthHeight_SPLICE,
	EHowToDetermineWidthHeight_MAX
};

enum EWorldType
{
	EWorldType_INVALID = -1,
	EWorldType_EARTH,
	EWorldType_FIRE,
	EWorldType_WATER,
	EWorldType_AIR,
	EWorldType_DARK,
	EWorldType_MAX
};



std::vector<std::string> tokenize(const std::string input, const std::string delimiter);

std::vector<std::string> tokenizeByStringLength(const std::string input, int lineLength);

std::string createStringFromKeyboardList(const std::vector<EKeyboardInput>& list);

float degreesToImageRotationDegrees(int startingImageRotation, int degrees);

template <typename T>
void addToListIfUnique(std::vector<T*>& list, T* elementToAdd)
{
	for (T* element : list)
	{
		if (element == elementToAdd)
		{
			return;
		}
	}
	list.push_back(elementToAdd);
}

std::string directionToString(EDirection dir);

std::string floatDecimalToString(float num);