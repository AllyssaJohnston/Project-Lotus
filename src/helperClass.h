#pragma once
#include <string>
#include <vector>
#include <SDL3/SDL.h>


enum EKeyboardInput 
{
	EKeyboardInput_INVALID = -1,
	EKeyboardInput_LEFT,
	EKeyboardInput_RIGHT,
	EKeyboardInput_UP,
	EKeyboardInput_DOWN,
	EKeyboardInput_SPACE_BAR,
	EKeyboardInput_A,
	EKeyboardInput_D,
	EKeyboardInput_W,
	EKeyboardInput_J,
	EKeyboardInput_K,
	EKeyboardInput_L,
	EKeyboardInput_M,
	EKeyboardInput_N,
	EKeyboardInput_R,
	EKeyboardInput_T,
	EKeyboardInput_1,
	EKeyboardInput_2,
	EKeyboardInput_ENTER,
	EKeyboardInput_ESC,
	EKeyboardInput_NONE,
	EKeyboardInput_MAX
};

enum EMouseInput
{
	EMouseInput_INVALID = -1,
	EMouseInput_LEFT,
	EMouseInput_MIDDLE,
	EMouseInput_RIGHT,
	EMouseInput_MAX
};

const static int numEventsToGrab = 10;

struct KeyboardData
{
	const static int            numEventsToGrab = 10;
	SDL_Event                   mEvents [numEventsToGrab];
	const static int            mNumEvents = 19;
	bool                        mLastFrameKeyState[mNumEvents];
	bool	                    mKeyState[mNumEvents];
	int 	                    mKeyStateRepeat[mNumEvents];
};

struct MouseData
{
	SDL_Event                   mMouseEvents[numEventsToGrab];
	const static int            mNumMouseEvents = 3;
	bool                        mLastFrameMouseEventSyms[mNumMouseEvents];
	bool                        mMouseEventSyms[mNumMouseEvents];
	int 	                    mMouseStateRepeat[mNumMouseEvents];
};

enum EDirection 
{
	EDirection_INVALID = -1,
	EDirection_LEFT,
	EDirection_RIGHT,
	EDirection_UP,
	EDirection_DOWN,
	EDirection_NONE,
	EDirection_MAX
};



//Types of Hitbox Edges
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
	EMiniGameState_PLAYER_WAIT_FOR_ATTACK_INPUT,
	EMiniGameState_PLAYER_WAIT_FOR_ATTACK_SUB_INPUT,
	EMiniGameState_PLAYER_COMPLETE_DIRECTIONAL_ATTACK,
	EMiniGameState_PLAYER_TAKE_ACTION_ATTACK,
	EMiniGameState_PLAYER_TAKE_ACTION_DEFEND,
	EMiniGameState_ENEMY_MOVE_CHARACTER,
	EMiniGameState_ENEMY_TAKE_ACTION,
	EMiniGameState_BUFFER,
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

std::string convertKeyboardEnumToString(EKeyboardInput key);

std::vector<std::string> tokenize(std::string input, std::string delimiter);

std::vector<std::string> tokenizeByStringLength(std::string input, int lineLength);

std::string createStringFromKeyboardList(std::vector <EKeyboardInput> list);

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