#include <SDL3/SDL.h>
#include <cmath>
#include <iterator>
#include <iostream>

#include "gameStateHelper.h"

extern const int numEventsToGrab;

Tile* findTile(Grid* pGrid, TileCoords* pTileCoords)
{
	int row = pTileCoords->mRow;
	int col = pTileCoords->mCol;
	if (pGrid->isLegalCoords(row, col))
	{
		return pGrid->mpTiles[pGrid->getIndex(row, col)];
	}   
	return nullptr;
}

bool isPlayableTile(Tile* pGivenTile)
{
	switch (pGivenTile->mType)
	{
	case EMiniGameCombatTileType_IMPASSABLE:
		break;
	case EMiniGameCombatTileType_TELEPORTER:
		//TODO
		break;
	default:
		return true;
		break;
	}
	return false;
}

bool validAttackTile(MiniGameStateManagerData& stateManagerData, Tile* pGivenTile, CombatCharacter* pGivenCharacter)
{
	Attack& attack = stateManagerData.mpData->mCurAttack;
	std::vector <TileCoords> possibleAttackTileCoords;
	if (attack.mRequiresDirectionInput)
	{
		possibleAttackTileCoords = returnAttackTileCoordsBasedOnAttackAndDirection(pGivenCharacter->mCombatMovementManager.getCurTile(), attack, stateManagerData.mpData->mCurAttackDirection).mTileCoords;
	}
	else
	{
		possibleAttackTileCoords = returnAttackTileCoordsBasedOnAttack(pGivenCharacter->mCombatMovementManager.getCurTile(), attack).mTileCoords;
	}

	for (int count = 0; count < possibleAttackTileCoords.size(); count++)
	{
		Tile* pTile = findTile(stateManagerData.mpMiniGameWorldData->mpMiniGameLevels[stateManagerData.mpMiniGameWorldData->mCurMiniGameLevelNumber]->mpGrid, &(possibleAttackTileCoords[count]));
		if (pTile == pGivenTile)
		{
			return true;
		}
	}
	return false;
}

void attemptAttackMultipleTiles(MiniGameStateManagerData& stateManagerData, std::vector <Tile*> tilesToAttack, CombatCharacter* pGivenCharacter)
{
	for (int count = 0; count < (int)tilesToAttack.size(); count++)
	{
		if (validAttackTile(stateManagerData, tilesToAttack[count], pGivenCharacter))
		{
			Attack pAttack = stateManagerData.mpData->mCurAttack;
			CombatManager * pCombatManager = stateManagerData.mpMiniGameWorldData->mpMiniGameLevels[stateManagerData.mpMiniGameWorldData->mCurMiniGameLevelNumber]->mpCombatManager;
			pCombatManager->attack(pGivenCharacter, tilesToAttack[count], pAttack);
			continue;
			//attackCharacterChanges
		}
	}
}

std::vector <Tile*> returnListWithoutTilesWithCharacters(CombatManager* pCombatManager, CombatCharacter* pGivenCharacter, std::vector <Tile*> listOfTiles)
{
	for (int countTile = (int)listOfTiles.size() - 1; countTile > -1; countTile--)
	{
		Tile * pCurTile = listOfTiles[countTile];
		for (CombatCharacter* pCurCharacter : pCombatManager->mpCurCombatCharacters)
		{
			if		(pCurCharacter->mCombatMovementManager.getCurTile()->mRow == pCurTile->mRow 
				and  pCurCharacter->mCombatMovementManager.getCurTile()->mCol == pCurTile->mCol)
			{
				//std::vector<Tile*>::iterator  it = listOfTiles.begin();
				listOfTiles.erase(listOfTiles.begin() + countTile);
			}
		}
	}
	return listOfTiles;
}

std::vector <TileDistance> returnListOfTileDistances(std::vector <CombatCharacter*> pCurCombatCharacters, std::vector <Tile*> pMoveTiles, CombatCharacter* pCurEnemy)
{
	std::vector <TileDistance> tileDistances;

	for (CombatCharacter* pCurCharacter : pCurCombatCharacters)
	{
		if (pCurCharacter->mType == EMiniGameCombatCharacterType_PLAYER)
		{
			for (Tile* pCurTile : pMoveTiles)
			{
				int distanceRow = abs(pCurTile->mRow - pCurCharacter->mCombatMovementManager.getCurTile()->mRow);
				int distanceCol = abs(pCurTile->mCol - pCurCharacter->mCombatMovementManager.getCurTile()->mCol);
				float distance = (float)sqrt( pow(distanceRow, 2) + pow(distanceCol, 2));
				tileDistances.push_back(TileDistance(pCurTile, pCurEnemy, pCurCharacter, distance));
			}
		}
	}
	return tileDistances;
}


//MINI GAME STATE
MiniGameState::MiniGameState(MouseData* pMouseData, MiniGameStateData * pData) : mpMouseData(pMouseData), mpData(pData) {}

MiniGameState::~MiniGameState()
{
	mpMouseData = nullptr;
	mpData      = nullptr;
}

void MiniGameState::useMouseInput(EMiniGameState curStateEnum, ScreenObject& screenObject, Grid* pGrid) 
{
	if (curStateEnum == EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT or curStateEnum == EMiniGameState_PLAYER_TAKE_ACTION_ATTACK)
	{
		float x;
		float y;
		SDL_GetMouseState(&x, &y);
		Vect2 pos((int)(x / screenObject.mGameScreenToGameLevelChunkRatio), (int)(y / screenObject.mGameScreenToGameLevelChunkRatio));

		static std::vector <int> eventVect;
		for (int countEvent = 0; countEvent < mpMouseData->mNumMouseEvents; countEvent++) 
		{
			if (mpMouseData->mMouseEventSyms[countEvent] == true) 
			{
				eventVect.push_back(countEvent);
				if (countEvent == int(EMouseInput_LEFT))
				{
					selectTile(pGrid, pos);
				}
			}
		}
		eventVect.clear();
	}
};

void MiniGameState::setCharacter(CombatCharacter* pCharacter)
{
	mpData->mpCharacter = pCharacter;
}

void MiniGameState::highlightTile(Grid* pGrid, Vect2 pos)
{
	for (Tile* pTile : pGrid->mpTiles)
	{
		if (pTile->getMode() == EMiniGameCombatTileMode_HIGHLIGHTED)
		{
			pTile->setMode(EMiniGameCombatTileMode_NOT_SELECTED);
		}

		Tile* pTile = pGrid->getTileFromCoords(pos.getX(), pos.getY());

		if (pTile != nullptr)
		{
			if (pTile->getMode() == EMiniGameCombatTileMode_SELECTED)
			{

			}
			else
			{
				pTile->setMode(EMiniGameCombatTileMode_HIGHLIGHTED);
			}
		}
	}
}



//MINI GAME STATE MANAGER
MiniGameStateManager::MiniGameStateManager(MouseData* pMouseData, MiniGameWorldData& miniGameWorldData)
{
	mData = MiniGameStateManagerData();
	mData.mpMiniGameWorldData = &miniGameWorldData;
	MiniGameStateData* pMiniGameStateData = mData.mpData;
	mpStates.push_back(new MiniGamePlayerWaitForMoveInput(			pMouseData, pMiniGameStateData));
	mpStates.push_back(new MiniGamePlayerMoveCharacter(				pMouseData, pMiniGameStateData));
	mpStates.push_back(new MiniGamePlayerWaitForActionInput(		pMouseData, pMiniGameStateData));
	mpStates.push_back(new MiniGamePlayerWaitForAttackInput(		pMouseData, pMiniGameStateData));
	mpStates.push_back(new MiniGamePlayerWaitForAttackSubInput(		pMouseData, pMiniGameStateData));
	mpStates.push_back(new MiniGamePlayerCompleteDirectionalAttack(	pMouseData, pMiniGameStateData));
	mpStates.push_back(new MiniGamePlayerTakeActionAttack(			pMouseData, pMiniGameStateData));
	mpStates.push_back(new MiniGamePlayerTakeActionDefend(			pMouseData, pMiniGameStateData));
	mpStates.push_back(new MiniGameEnemyMoveCharacter(				pMouseData, pMiniGameStateData));
	mpStates.push_back(new MiniGameEnemyTakeAction(					pMouseData, pMiniGameStateData));
	mpStates.push_back(new MiniGameBuffer(							pMouseData, pMiniGameStateData));
	mData.mCurStateEnum = EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT;
	mpCurState			= mpStates[mData.mCurStateEnum];
}

MiniGameStateManager::~MiniGameStateManager()
{
	mpCurState = nullptr;
}

void MiniGameStateManager::preTick()
{
	mData.mLastFrameStateEnum = mData.mCurStateEnum;
}

void MiniGameStateManager:: tick()
{
	mpCurState->tick(mData);
	postTick();
}

void MiniGameStateManager::postTick()
{
	updateCurState(mData.mpData->mNextMiniGameState);
}

void MiniGameStateManager:: updateCurState(EMiniGameState newStateEnum)
{
	mData.mCurStateEnum = newStateEnum;
	mpCurState			= mpStates[newStateEnum];
}




//GAME STATE
GameState::GameState(){;}

GameState::~GameState()
{
	mpKeyboardData     = nullptr;
	mpMouseData        = nullptr;

	mpMenuManager      = nullptr;

	mpGameStateData    = nullptr;

	mpSettingsManager  = nullptr;
	mpStyleManager     = nullptr;
	mpScreen           = nullptr;
}

void GameState::getInput()
{
	
	for (int count = 0; count < mpKeyboardData->mNumEvents; count++)
	{
		mpKeyboardData->mLastFrameKeyState[count] = mpKeyboardData->mKeyState[count];
	}
	for (int count = 0; count < mpMouseData->mNumMouseEvents; count++)
	{
		mpMouseData->mLastFrameMouseEventSyms[count] = mpMouseData->mMouseEventSyms[count];
	}


	int numEvents = SDL_PeepEvents(mpKeyboardData->mEvents, numEventsToGrab, SDL_GETEVENT, SDL_EVENT_FIRST, SDL_EVENT_LAST);
	SDL_assert(numEvents <= numEventsToGrab);

	for (int count = 0; count < numEvents; count++) 
	{
		switch (mpKeyboardData->mEvents[count].type)
		{
		case SDL_EVENT_KEY_DOWN:
			switch (mpKeyboardData->mEvents[count].key.keysym.sym)
			{
			case SDLK_LEFT:
			case SDLK_a:
				if (mpKeyboardData->mKeyState[(int)EKeyboardInput_LEFT] != true)
				{
					mpKeyboardData->mKeyState[(int)EKeyboardInput_LEFT] = true;
					mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_LEFT] = 0;
				}
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				if (mpKeyboardData->mKeyState[(int)EKeyboardInput_RIGHT] != true)
				{
					mpKeyboardData->mKeyState[(int)EKeyboardInput_RIGHT] = true;
					mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_RIGHT] = 0;
				}
				break;
			case SDLK_UP:
			case SDLK_w:
			case SDLK_SPACE:
				if (mpKeyboardData->mKeyState[(int)EKeyboardInput_UP] == false)
				{
					mpKeyboardData->mKeyState[(int)EKeyboardInput_UP] = true;
					mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_UP] = 0;
				}
				break;
			case SDLK_DOWN:
			case SDLK_s:
				mpKeyboardData->mKeyState[(int)EKeyboardInput_DOWN] = true;
				mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_DOWN] = 0;
				break;
			case SDLK_j:
				mpKeyboardData->mKeyState[(int)EKeyboardInput_J] = true;
				mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_J] = 0;
				break;
			case SDLK_k:
				mpKeyboardData->mKeyState[(int)EKeyboardInput_K] = true;
				mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_K] = 0;
				break;
			case SDLK_l:
				mpKeyboardData->mKeyState[(int)EKeyboardInput_L] = true;
				mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_L] = 0;
				break;
			case SDLK_1:
				mpKeyboardData->mKeyState[(int)EKeyboardInput_1] = true;
				mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_1] = 0;
				break;
			case SDLK_2:
				mpKeyboardData->mKeyState[(int)EKeyboardInput_2] = true;
				mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_2] = 0;
				break;
			case SDLK_RETURN:
				mpKeyboardData->mKeyState[(int)EKeyboardInput_ENTER] = true;
				mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_ENTER] = 0;
				break;
			case SDLK_r:
				mpKeyboardData->mKeyState[(int)EKeyboardInput_R] = true;
				mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_R] = 0;
				break;
			case SDLK_t:
				mpKeyboardData->mKeyState[(int)EKeyboardInput_T] = true;
				mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_T] = 0;
				break;
			case SDLK_m:
				mpKeyboardData->mKeyState[(int)EKeyboardInput_M] = true;
				mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_M] = 0;
				break;
			case SDLK_n:
				mpKeyboardData->mKeyState[(int)EKeyboardInput_N] = true;
				mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_N] = 0;
				break;
			case SDLK_ESCAPE:
				mpKeyboardData->mKeyState[(int)EKeyboardInput_ESC] = true;
				mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_ESC] = 0;
				//pStateManager->mRunGame = false; //TODO
				break;
			default:
				break;
			}
			break;
		case SDL_EVENT_KEY_UP:
			switch (mpKeyboardData->mEvents[count].key.keysym.sym)
			{
			case SDLK_LEFT:
			case SDLK_a:
				mpKeyboardData->mKeyState[(int)EKeyboardInput_LEFT] = false;
				mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_LEFT] = 0;
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				mpKeyboardData->mKeyState[(int)EKeyboardInput_RIGHT] = false;
				mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_RIGHT] = 0;
				break;
			case SDLK_UP:
			case SDLK_w:
			case SDLK_SPACE:
				mpKeyboardData->mKeyState[(int)EKeyboardInput_UP] = false;
				mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_UP] = 0;
				break;
			case SDLK_DOWN:
			case SDLK_s:
				mpKeyboardData->mKeyState[(int)EKeyboardInput_DOWN] = false;
				mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_DOWN] = 0;
				break;
			case SDLK_j:
				mpKeyboardData->mKeyState[(int)EKeyboardInput_J] = false;
				mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_J] = 0;
				break;
			case SDLK_k:
				mpKeyboardData->mKeyState[(int)EKeyboardInput_K] = false;
				mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_K] = 0;
				break;
			case SDLK_l:
				mpKeyboardData->mKeyState[(int)EKeyboardInput_L] = false;
				mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_L] = 0;
				break;
			case SDLK_1:
				mpKeyboardData->mKeyState[(int)EKeyboardInput_1] = false;
				mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_1] = 0;
				break;
			case SDLK_2:
				mpKeyboardData->mKeyState[(int)EKeyboardInput_2] = false;
				mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_2] = 0;
				break;
			case SDLK_RETURN:
				mpKeyboardData->mKeyState[(int)EKeyboardInput_ENTER] = false;
				mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_ENTER] = 0;
				break;
			case SDLK_r:
				mpKeyboardData->mKeyState[(int)EKeyboardInput_R] = false;
				mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_R] = 0;
				break;
			case SDLK_t:
				mpKeyboardData->mKeyState[(int)EKeyboardInput_T] = false;
				mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_T] = 0;
				break;
			case SDLK_m:
				mpKeyboardData->mKeyState[(int)EKeyboardInput_M] = false;
				mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_M] = 0;
				//gameInstance.mSingleSteppingMode = !gameInstance.mSingleSteppingMode; //TODO
				break;
			case SDLK_n:
				mpKeyboardData->mKeyState[(int)EKeyboardInput_N] = false;
				mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_N] = 0;
				//gameInstance.mFrameStepInputRequest = true; //TODO
				break;
			default:
				break;
			}
			break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			switch (mpMouseData->mMouseEvents[count].button.button)
			{
			case SDL_BUTTON_LEFT:
				mpMouseData->mMouseEventSyms[(int)EMouseInput_LEFT] = true;
				break;
			case SDL_BUTTON_MIDDLE:
				mpMouseData->mMouseEventSyms[(int)EMouseInput_MIDDLE] = true;
				break;
			case SDL_BUTTON_RIGHT:
				mpMouseData->mMouseEventSyms[(int)EMouseInput_RIGHT] = true;
				break;
			default:
				break;
			}
			break;
		case SDL_EVENT_MOUSE_BUTTON_UP:
			switch (mpMouseData->mMouseEvents[count].button.button)
			{
			case SDL_BUTTON_LEFT:
				mpMouseData->mMouseEventSyms[(int)EMouseInput_LEFT] = false;
				break;
			case SDL_BUTTON_MIDDLE:
				mpMouseData->mMouseEventSyms[(int)EMouseInput_MIDDLE] = false;
				break;
			case SDL_BUTTON_RIGHT:
				mpMouseData->mMouseEventSyms[(int)EMouseInput_RIGHT] = false;
				break;
			default:
				break;
			}
			break;
		case SDL_EVENT_QUIT:
			mpKeyboardData->mKeyState[(int)EKeyboardInput_ESC] = true;
			mpKeyboardData->mKeyStateRepeat[(int)EKeyboardInput_ESC] = 0;
			//pStateManager->mRunGame = false; //TODO
			break;
		default:
			break;
		}
	}

	for (int count = 0; count < mpKeyboardData->mNumEvents; count++)
	{
		mpKeyboardData->mKeyStateRepeat[count] += 1;
	}
}

void GameState::useInput(GameStateManagerData& gameStateManagerData)
{
	for (int countEvent = 0; countEvent < mpKeyboardData->mNumEvents; countEvent++) 
	{
		if (mpKeyboardData->mKeyState[countEvent] == true)
		{

			if (countEvent == int(EKeyboardInput_ESC))
			{
				gameStateManagerData.mRunGame = false;
			}
		}
	}
}



//GAME STATE MANAGER
GameStateManager::GameStateManager(KeyboardData& keyboardData, MouseData& mouseData, WorldData& worldData, MenuManager& menuManager, 
	SettingsManager& settingsManager, CollisionManager& collisionManager, DamageManager& damageManager, SlashManager& slashManager,
	StyleManager& styleManager, MiniGameStateManager& miniGameStateManager) : 
	mMiniGameStateManager(miniGameStateManager), mWorldData(worldData)
{
	mpGameStateData = new GameStateData();
	mStates.push_back(new GameStatePlay(*mpGameStateData, keyboardData, mouseData, worldData, menuManager, settingsManager, collisionManager, 
		damageManager, slashManager, styleManager));
	mStates.push_back(new GameStatePlayMiniGame(*mpGameStateData, keyboardData, mouseData, miniGameStateManager, menuManager, 
		worldData.mScreen, settingsManager, styleManager));
	mStates.push_back(new GameStateMenu(*mpGameStateData, keyboardData, mouseData, menuManager, worldData.mScreen, settingsManager,
		styleManager));
	mData.mCurStateEnum       = EGameState_PLAY;
	mData.mLastFrameStateEnum = EGameState_INVALID;
	mpCurState = mStates[mData.mCurStateEnum];
}

GameStateManager::~GameStateManager()
{
	for (GameState* iter : mStates)
	{
		delete iter;
	}
	mStates.clear();
	mpGameStateData = nullptr;
	mpCurState		= nullptr;
}

void GameStateManager::preTick()
{
	mData.mLastFrameStateEnum = mData.mCurStateEnum;
	mpCurState->preTick(mData, mMiniGameStateManager, mWorldData);
}

void GameStateManager::tick()
{
	mpCurState->tick(mData, mMiniGameStateManager, mWorldData);
}

void GameStateManager::updateCurState(EGameState nextStateEnum)
{
	mData.mCurStateEnum = nextStateEnum;
	mpCurState = mStates[nextStateEnum];
}

void GameStateManager::postTick()
{
	mpCurState->postTick(mData, mMiniGameStateManager, mWorldData);
	if (mpGameStateData->mNextGameState != EGameState_INVALID)
	{
		updateCurState(mpGameStateData->mNextGameState);
		mpGameStateData->mNextGameState = EGameState_INVALID;
	}
}



//TYPES MINI GAME STATES
MiniGamePlayerWaitForMoveInput::MiniGamePlayerWaitForMoveInput(MouseData* pMouseData, MiniGameStateData * pData) : 
	MiniGameState(pMouseData, pData){;}

MiniGamePlayerWaitForMoveInput::~MiniGamePlayerWaitForMoveInput()
{
	MiniGameState::~MiniGameState();
}

void MiniGamePlayerWaitForMoveInput::selectTile(Grid* pGrid, Vect2 pos)
{
	Tile * pCurTile = pGrid->getTileFromCoords(pos.getX(), pos.getY());
	if (pCurTile != nullptr)
	{
		pCurTile->setMode(EMiniGameCombatTileMode_SELECTED);
		moveToTile(pCurTile, mpData->mpCharacter);
		return;
	}
}

void MiniGamePlayerWaitForMoveInput::moveToTile(Tile* pGivenTile, CombatCharacter* pGivenCharacter)
{
	if (checkIfTileInCharacterMoveRange(pGivenTile, pGivenCharacter))
	{
		if (isPlayableTile(pGivenTile))
		{
			postTick(pGivenTile); //+moveChange
		}
	}
}

void MiniGamePlayerWaitForMoveInput::postTick(Tile* pNextTile)
{
	mpData->mNextMiniGameState = EMiniGameState_PLAYER_MOVE_CHARACTER;
	mpData->mpTileToMoveTo = pNextTile;
	mpData->mpCharacter->mCombatMovementManager.setCurTile(pNextTile);
}



MiniGamePlayerMoveCharacter::MiniGamePlayerMoveCharacter(MouseData* pMouseData, MiniGameStateData * pData) : MiniGameState(pMouseData, pData){;}

MiniGamePlayerMoveCharacter::~MiniGamePlayerMoveCharacter()
{
	MiniGameState::~MiniGameState();
}

void MiniGamePlayerMoveCharacter::tick(MiniGameStateManagerData& stateManagerData)
{
	mpData->mpCharacter->move(mpData->mpTileToMoveTo);
	postTick();
}

void MiniGamePlayerMoveCharacter::postTick()
{
	mpData->mNextMiniGameState = EMiniGameState_PLAYER_WAIT_FOR_ACTION_INPUT;
	mpData->mpTileToMoveTo = nullptr;
}



MiniGamePlayerWaitForActionInput::MiniGamePlayerWaitForActionInput(MouseData* pMouseData, MiniGameStateData * pData) : 
	MiniGameState(pMouseData, pData){;}

MiniGamePlayerWaitForActionInput::~MiniGamePlayerWaitForActionInput()
{
	MiniGameState::~MiniGameState();
}

void MiniGamePlayerWaitForActionInput::postTick(MiniGameWorldData& worldData, EMiniGameState nextStateEnum)
{
	if (nextStateEnum == EMiniGameState_BUFFER)
	{
		CombatCharacter* pNextCharacter = worldData.mpMiniGameLevels[worldData.mCurMiniGameLevelNumber]->mpCombatManager->returnNextCharacter(mpData->mpCharacter);
		mpData->mpCharacter = pNextCharacter;
		if (mpData->mpCharacter->mType == EMiniGameCombatCharacterType_PLAYER)
		{
			mpData->mPostBufferGameState = EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT;
		}
		else if (mpData->mpCharacter->mType == EMiniGameCombatCharacterType_ENEMY)
		{
			mpData->mPostBufferGameState = EMiniGameState_ENEMY_MOVE_CHARACTER;
		}
		else
		{
			SDL_assert(false);
		}
	}
	mpData->mNextMiniGameState = nextStateEnum;
}



MiniGamePlayerWaitForAttackInput::MiniGamePlayerWaitForAttackInput(MouseData* pMouseData, MiniGameStateData * pData) : 
	MiniGameState(pMouseData, pData){;}

MiniGamePlayerWaitForAttackInput::~MiniGamePlayerWaitForAttackInput()
{
	MiniGameState::~MiniGameState();
}

void MiniGamePlayerWaitForAttackInput::postTick(const Attack& attack)
{
	if (attack.mRequiresDirectionInput)
	{
		mpData->mCurAttack = attack;
		mpData->mNextMiniGameState = EMiniGameState_PLAYER_WAIT_FOR_ATTACK_SUB_INPUT;
	}
	else
	{
		mpData->mCurAttack = attack;
		mpData->mNextMiniGameState = EMiniGameState_PLAYER_TAKE_ACTION_ATTACK;
	}
}



MiniGamePlayerWaitForAttackSubInput::MiniGamePlayerWaitForAttackSubInput(MouseData* pMouseData, MiniGameStateData * pData) : 
	MiniGameState(pMouseData, pData){;}

MiniGamePlayerWaitForAttackSubInput::~MiniGamePlayerWaitForAttackSubInput()
{
	MiniGameState::~MiniGameState();
}

void MiniGamePlayerWaitForAttackSubInput::postTick(EDirection curAttackDirection)
{
	mpData->mCurAttackDirection = curAttackDirection;
	mpData->mNextMiniGameState = EMiniGameState_PLAYER_COMPLETE_DIRECTIONAL_ATTACK;
}



MiniGamePlayerCompleteDirectionalAttack::MiniGamePlayerCompleteDirectionalAttack(MouseData* pMouseData, MiniGameStateData * pData) : 
	MiniGameState(pMouseData, pData){;}

MiniGamePlayerCompleteDirectionalAttack::~MiniGamePlayerCompleteDirectionalAttack()
{
	MiniGameState::~MiniGameState();
}

void MiniGamePlayerCompleteDirectionalAttack::tick(MiniGameStateManagerData& stateManagerData)
{
	if (mpData->mTicks < mpData->mTicksBeforeAction)
	{
		mpData->mTicks += 1;
	}
	else
	{
		attackTiles(stateManagerData);
	}
}

void MiniGamePlayerCompleteDirectionalAttack::attackTiles(MiniGameStateManagerData& stateManagerData)
{
	Grid* pGrid = stateManagerData.mpMiniGameWorldData->mpMiniGameLevels[stateManagerData.mpMiniGameWorldData->mCurMiniGameLevelNumber]->mpGrid;
	AttackAndListOfTileCoordsToCorrespondingTilesCoords attackTileCoords = returnAttackTileCoordsBasedOnAttackAndDirection(mpData->mpCharacter->mCombatMovementManager.getCurTile(), mpData->mCurAttack, mpData->mCurAttackDirection);
	std::vector <Tile* > tilesToAttack;
	for (int count = 0; count < attackTileCoords.mTileCoords.size(); count++)
	{
		Tile* curTile = pGrid->mpTiles[pGrid->getIndex(attackTileCoords.mTileCoords[count].mRow, attackTileCoords.mTileCoords[count].mCol)];
		tilesToAttack.push_back(curTile);
	}
	attemptAttackMultipleTiles(stateManagerData, tilesToAttack, mpData->mpCharacter);
	postTick(*stateManagerData.mpMiniGameWorldData);
}

//attackCharacterChanges
void MiniGamePlayerCompleteDirectionalAttack::postTick(MiniGameWorldData& worldData)
{
	CombatManager * pCombatManager = worldData.mpMiniGameLevels[worldData.mCurMiniGameLevelNumber]->mpCombatManager;

	pCombatManager->postTick();
	mpData->mpCharacter = pCombatManager->returnNextCharacter(mpData->mpCharacter);
	if		(mpData->mpCharacter->mType == EMiniGameCombatCharacterType_PLAYER)
	{
		mpData->mPostBufferGameState = EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT;
	}
	else if (mpData->mpCharacter->mType == EMiniGameCombatCharacterType_ENEMY)
	{
		mpData->mPostBufferGameState = EMiniGameState_ENEMY_MOVE_CHARACTER;
	}
	else
	{
		SDL_assert(false);
	}
	mpData->mTicks = 0;
	mpData->mCurAttackDirection = EDirection_NONE;
	mpData->mNextMiniGameState = EMiniGameState_BUFFER;
}



MiniGamePlayerTakeActionAttack::MiniGamePlayerTakeActionAttack(MouseData* pMouseData, MiniGameStateData * pData) : 
	MiniGameState(pMouseData, pData){;}

MiniGamePlayerTakeActionAttack::~MiniGamePlayerTakeActionAttack()
{
	MiniGameState::~MiniGameState();
}

void MiniGamePlayerTakeActionAttack::selectTile(MiniGameStateManagerData& stateManagerData, Grid* pGrid, Vect2 pos)
{
	Tile * pTile = pGrid->getTileFromCoords(pos.getX(), pos.getY());

	if (pTile != nullptr)
	{
		pTile->setMode(EMiniGameCombatTileMode_SELECTED);
		std::vector <Tile *> tilesToAttack;
		tilesToAttack.push_back(pTile);
		attemptAttackMultipleTiles(stateManagerData, tilesToAttack, mpData->mpCharacter);
		postTick(*stateManagerData.mpMiniGameWorldData);
		return;
	}
}

void MiniGamePlayerTakeActionAttack::postTick(MiniGameWorldData& worldData)
{
	CombatManager * pCombatManager = worldData.mpMiniGameLevels[worldData.mCurMiniGameLevelNumber]->mpCombatManager;

	pCombatManager->postTick();
	mpData->mpCharacter = pCombatManager->returnNextCharacter(mpData->mpCharacter);
	if		(mpData->mpCharacter->mType == EMiniGameCombatCharacterType_PLAYER)
	{
		mpData->mPostBufferGameState = EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT;
	}
	else if (mpData->mpCharacter->mType == EMiniGameCombatCharacterType_ENEMY)
	{
		mpData->mPostBufferGameState = EMiniGameState_ENEMY_MOVE_CHARACTER;
	}
	else
	{
		SDL_assert(false);
	}
	mpData->mCurAttackDirection = EDirection_NONE;
	mpData->mNextMiniGameState = EMiniGameState_BUFFER;
}



MiniGamePlayerTakeActionDefend::MiniGamePlayerTakeActionDefend(MouseData* pMouseData, MiniGameStateData * pData) : 
	MiniGameState(pMouseData, pData){;}

MiniGamePlayerTakeActionDefend::~MiniGamePlayerTakeActionDefend()
{
	MiniGameState::~MiniGameState();
}

void MiniGamePlayerTakeActionDefend::tick(MiniGameStateManagerData& stateManagerData)
{
	mpData->mpCharacter->defend();
	postTick(*stateManagerData.mpMiniGameWorldData);
}

//defense change
void MiniGamePlayerTakeActionDefend::postTick(MiniGameWorldData& worldData)
{
	CombatManager * pCombatManager = worldData.mpMiniGameLevels[worldData.mCurMiniGameLevelNumber]->mpCombatManager;

	pCombatManager->postTick();
	mpData->mpCharacter = pCombatManager->returnNextCharacter(mpData->mpCharacter);
	if		(mpData->mpCharacter->mType == EMiniGameCombatCharacterType_PLAYER)
	{
		mpData->mPostBufferGameState = EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT;
	}
	else if (mpData->mpCharacter->mType == EMiniGameCombatCharacterType_ENEMY)
	{
		mpData->mPostBufferGameState = EMiniGameState_ENEMY_MOVE_CHARACTER;
	}
	else
	{
		SDL_assert(false);
	}
}



MiniGameEnemyMoveCharacter::MiniGameEnemyMoveCharacter(MouseData* pMouseData, MiniGameStateData * pData) : MiniGameState(pMouseData, pData){;}

MiniGameEnemyMoveCharacter::~MiniGameEnemyMoveCharacter()
{
	MiniGameState::~MiniGameState();
}

void MiniGameEnemyMoveCharacter::tick(MiniGameStateManagerData& stateManagerData)
{
	if (mpData->mTicks == 0 or mpData->mTicks < 0)
	{
		decideTileToMoveTo(stateManagerData);
	}

	if (mpData->mTicks < mpData->mTicksBeforeAction)
	{
		mpData->mTicks += 1;
	}
	else
	{
		mpData->mpCharacter->move(mpData->mpTileToMoveTo);
		mpData->mTicks = -1;
		postTick();
	}
}

void MiniGameEnemyMoveCharacter::decideTileToMoveTo(MiniGameStateManagerData& stateManagerData)
{
	MiniGameLevel*	pCurLevel		= stateManagerData.mpMiniGameWorldData->mpMiniGameLevels[stateManagerData.mpMiniGameWorldData->mCurMiniGameLevelNumber];
	Grid*			pGrid			= pCurLevel->mpGrid;
	CombatManager * pCombatManager	= pCurLevel->mpCombatManager;
	mpData->mTicks = 0;

	std::vector <Tile*> allPossibleMoveTiles;

	for (TileCoords curTileCoords : mpData->mpCharacter->mCombatMovementManager.getMoveTiles())
	{
		int row = curTileCoords.mRow;
		int col = curTileCoords.mCol;
		if (pGrid->isLegalCoords(row, col))
		{
			Tile *pCurTile = pGrid->mpTiles[pGrid->getIndex(row, col)];
			if (isPlayableTile(pCurTile))
			{
				allPossibleMoveTiles.push_back(pCurTile);
			}
		}
	}

	int maxNumberOfCharactersCanAttack = 0;
	Tile* pBestTileToMoveTo = nullptr;

	for (Tile* pCurTile : allPossibleMoveTiles)
	{
		std::vector <AttackTile> allPossibleAttacksFromCurTile = returnAttackTileCoordsWithPlayersOnThem(*stateManagerData.mpMiniGameWorldData, pCurTile, mpData->mpCharacter);
		if (allPossibleAttacksFromCurTile.size() > maxNumberOfCharactersCanAttack)
		{
			pBestTileToMoveTo = pCurTile;
		}
	}
	if (pBestTileToMoveTo != nullptr)
	{
		mpData->mpTileToMoveTo = pBestTileToMoveTo;
		return;
	}

	//No tiles I can attack from
	allPossibleMoveTiles = returnListWithoutTilesWithCharacters(pCombatManager, mpData->mpCharacter, allPossibleMoveTiles);
	std::vector <TileDistance> tileDistances = returnListOfTileDistances(pCombatManager->mpCurCombatCharacters, allPossibleMoveTiles, mpData->mpCharacter);
	int minDistanceFromPlayer = std::numeric_limits<int>::max();
	Tile* pCurTile = mpData->mpCharacter->mCombatMovementManager.getCurTile();

	for (TileDistance curTileDistance : tileDistances)
	{
		if (minDistanceFromPlayer > curTileDistance.mDistance and curTileDistance.mpTile != mpData->mpCharacter->mCombatMovementManager.getCurTile())
		{
			minDistanceFromPlayer = (int)curTileDistance.mDistance;
			pCurTile = curTileDistance.mpTile;
		}
	}
	mpData->mpTileToMoveTo = pCurTile;
}

void MiniGameEnemyMoveCharacter::postTick()
{
	mpData->mNextMiniGameState	= EMiniGameState_ENEMY_TAKE_ACTION;
	mpData->mpTileToMoveTo		= nullptr;
}



MiniGameEnemyTakeAction:: MiniGameEnemyTakeAction(MouseData* pMouseData, MiniGameStateData * pData) : MiniGameState(pMouseData, pData){;}

MiniGameEnemyTakeAction::~MiniGameEnemyTakeAction()
{
	MiniGameState::~MiniGameState();
}

void MiniGameEnemyTakeAction::tick(MiniGameStateManagerData& stateManagerData)
{
	if (mpData->mTicks == int(mpData->mTicksBeforeAction / 2))
	{
		shouldAttack(*stateManagerData.mpMiniGameWorldData);
	}

	if (mpData->mTicks < mpData->mTicksBeforeAction)
	{
		mpData->mTicks += 1;
	}
	else
	{
		if (mpData->mGoingToAttack)
		{
			performAttack(stateManagerData);
		}
		else if (shouldDefend(*stateManagerData.mpMiniGameWorldData))
		{
			postTick(*stateManagerData.mpMiniGameWorldData);
		}
		else
		{
			postTick(*stateManagerData.mpMiniGameWorldData);
		}
	}
}

bool MiniGameEnemyTakeAction::shouldAttack(MiniGameWorldData& worldData)
{
	Tile * refTile = mpData->mpCharacter->mCombatMovementManager.getCurTile();
	std::vector <AttackTile> attackTilesWithCharacters = returnAttackTileCoordsWithPlayersOnThem(worldData, refTile, mpData->mpCharacter);
	Grid* pGrid = worldData.mpMiniGameLevels[worldData.mCurMiniGameLevelNumber]->mpGrid;

	AttackTile * pCurBestAttackTile = nullptr;
	AttackAndListOfTileCoordsToCorrespondingTilesCoords curBestAttackTiles;
	float maxDamageOutput = 0;


	for (AttackTile curAttackTile : attackTilesWithCharacters)
	{
		if (curAttackTile.mAttack.mRequiresDirectionInput)
		{
			EDirection attackDirection = getDirectionBetweenTiles(mpData->mpCharacter->mCombatMovementManager.getCurTile(), curAttackTile.mpTile);
			AttackAndListOfTileCoordsToCorrespondingTilesCoords allAttackTiles = returnAttackTileCoordsBasedOnAttackAndDirection(mpData->mpCharacter->mCombatMovementManager.getCurTile(), curAttackTile.mAttack, attackDirection);
			float damageOutput = allAttackTiles.mTileCoords.size() * allAttackTiles.mAttack.mDamagePercent;
			if (damageOutput > maxDamageOutput)
			{
				maxDamageOutput = damageOutput;
				curBestAttackTiles.mTileCoords.clear();
				curBestAttackTiles = allAttackTiles;
				pCurBestAttackTile = nullptr;
			}

		}
		else
		{
			float damageOutput = curAttackTile.mAttack.mDamagePercent;
			if (damageOutput > maxDamageOutput)
			{
				maxDamageOutput = damageOutput;
				pCurBestAttackTile = &curAttackTile;
				curBestAttackTiles.mTileCoords.clear();
			}
		}

		if (curBestAttackTiles.mTileCoords.size() == 0 and pCurBestAttackTile == nullptr)
		{
			return false;
		}
		else
		{
			if (curBestAttackTiles.mTileCoords.size() == 0)
			{
				mpData->mpTilesToAttack.push_back(pCurBestAttackTile->mpTile);
				mpData->mCurAttack = pCurBestAttackTile->mAttack;
			}
			else
			{
				for (TileCoords coords: curBestAttackTiles.mTileCoords)
				{
					Tile* pCurTile = pGrid->mpTiles[pGrid->getIndex(coords.mRow, coords.mCol)];
					mpData->mpTilesToAttack.push_back(pCurTile);
					mpData->mCurAttack = curBestAttackTiles.mAttack;
				}
			}

			if (mpData->mCurAttack.mRequiresDirectionInput)
			{
				mpData->mCurAttackDirection = getDirectionBetweenTiles(mpData->mpCharacter->mCombatMovementManager.getCurTile(), mpData->mpTilesToAttack[0]);
			}
		}
	}
	mpData->mGoingToAttack = true;
	return true;
}

bool MiniGameEnemyTakeAction::shouldDefend(MiniGameWorldData& worldData)
{
	Grid*			pGrid			= worldData.mpMiniGameLevels[worldData.mCurMiniGameLevelNumber]->mpGrid;
	CombatManager*	pCombatManager	= worldData.mpMiniGameLevels[worldData.mCurMiniGameLevelNumber]->mpCombatManager;
	Tile*			pCurTile		= mpData->mpCharacter->mCombatMovementManager.getCurTile();

	for (CombatCharacter* curCombatCharacter : pCombatManager->mpCurCombatCharacters)
	{
		std::vector <AttackTile> attackTileListWithCharacters = returnAttackTileCoordsWithPlayersOnThem(worldData, pCurTile, mpData->mpCharacter);

		for (AttackTile attackTile : attackTileListWithCharacters)
		{
			Tile* pCurTileWithCharacter = attackTile.mpTile;
			if (pGrid->isLegalCoords(pCurTileWithCharacter->mRow, pCurTileWithCharacter->mCol))
			{
				if (pCurTileWithCharacter->mRow == pCurTile->mRow and pCurTileWithCharacter->mCol and pCurTile->mCol)
				{
					return true;
				}
			}
		}
	}
	return false;
}

void MiniGameEnemyTakeAction::performAttack(MiniGameStateManagerData& stateManagerData)
{
	attemptAttackMultipleTiles(stateManagerData, mpData->mpTilesToAttack, mpData->mpCharacter);
	postTick(*stateManagerData.mpMiniGameWorldData);
	//add attackedCharacterChangesList
}

void MiniGameEnemyTakeAction::postTick(MiniGameWorldData& worldData)
{
	CombatManager* pCombatManager = worldData.mpMiniGameLevels[worldData.mCurMiniGameLevelNumber]->mpCombatManager;
	worldData.mpMiniGameLevels[worldData.mCurMiniGameLevelNumber]->mpCombatManager->postTick();
	mpData->mpCharacter = pCombatManager->returnNextCharacter(mpData->mpCharacter);
	mpData->mNextMiniGameState = EMiniGameState_BUFFER;

	switch (mpData->mpCharacter->mType)
	{
	case EMiniGameCombatCharacterType_PLAYER:
		mpData->mPostBufferGameState = EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT;
		break;
	case EMiniGameCombatCharacterType_ENEMY:
		mpData->mPostBufferGameState = EMiniGameState_ENEMY_MOVE_CHARACTER;
		break;
	default:
		SDL_assert(false);
		break;
	}
	mpData->mGoingToAttack = false;
	mpData->mpTilesToAttack.clear();
	//mpData->mCurAttack = NULL;
	mpData->mCurAttackDirection = EDirection_NONE;
	mpData->mTicks = 0;
}



MiniGameBuffer:: MiniGameBuffer(MouseData* pMouseData, MiniGameStateData * pData) : MiniGameState(pMouseData, pData){;}

MiniGameBuffer::~MiniGameBuffer()
{
	MiniGameState::~MiniGameState();
}

void MiniGameBuffer::tick(MiniGameStateManagerData& stateManagerData)
{
	if (mpData->mTickYet == false)
	{
		mpData->mTickYet = true;
	}
	else
	{
		postTick();
	}
}

void MiniGameBuffer::postTick()
{
	mpData->mNextMiniGameState = mpData->mPostBufferGameState;
	mpData->mPostBufferGameState = EMiniGameState_INVALID;
	mpData->mTickYet = false;
}



//TYPES GAME STATES
GameStatePlay::GameStatePlay(GameStateData& gameStateData, KeyboardData& keyboardData, MouseData& mouseData, 
	WorldData& worldData, MenuManager& menuManager, SettingsManager& settingsManager, CollisionManager& collisionManager, 
	DamageManager& damageManager, SlashManager& slashManager, StyleManager& styleManager): mWorldData(worldData), mCollisionManager(collisionManager), 
	mDamageManager(damageManager), mSlashManager(slashManager)
{
	mpKeyboardData		= &keyboardData;
	mpMouseData			= &mouseData;
	mpMenuManager		= &menuManager;
	mpSettingsManager	= &settingsManager;
	mpStyleManager		= &styleManager;
	mpScreen			= &worldData.mScreen;
}

GameStatePlay::~GameStatePlay()
{
	GameState::~GameState();
}

void GameStatePlay::tick(GameStateManagerData& gameStateManagerData, MiniGameStateManager& miniGameStateManager,
	WorldData& worldData)
{
	mWorldData.updateCurLevelChunk();
	Level* curLevel = mWorldData.mpWorlds[mWorldData.mCurWorldNumber]->mpLevels[mWorldData.mCurLevelNumber];

	Player & player = mWorldData.mPlayer;
	LevelChunk& curLevelChunk = mWorldData.mCurLevelChunk;
	SDL_PumpEvents();

	getInput();

	if (mpSettingsManager->mSingleSteppingMode == false or (mpSettingsManager->mSingleSteppingMode == true 
		and mpSettingsManager->mFrameStepInputRequest == true))
	{	// ie do not tick if we have switched single step mode and there is not input request this frame.
		useMouseCursor();
		useInput(gameStateManagerData);
		mWorldData.entityPreTickUpdateMovement(mSlashManager);

		//COLLISIONS
		mWorldData.entityCollisions(mCollisionManager, mDamageManager, mSlashManager, *mpKeyboardData);

		if (mpSettingsManager->mFrameStepInputRequest)
		{
			// Everytime we skip we reset the variable.
			mpSettingsManager->mFrameStepInputRequest = false;
		}
	}

	//RENDERING
	render(gameStateManagerData, miniGameStateManager.mData, worldData);

	mWorldData.entityPostTick();
	mCollisionManager.postTick();
	mWorldData.clearDeadProjectiles();
	mWorldData.updatePermanentCollectibles();

	mWorldData.updateBackgroundEffects();

}

void GameStatePlay::useInput(GameStateManagerData& gameStateManagerData)
{
	GameState::useInput(gameStateManagerData);
	static std::vector < KeyData > eventVect;
	for (int countEvent = 0; countEvent < mpKeyboardData->mNumEvents; countEvent++) 
	{
		if (mpKeyboardData->mKeyState[countEvent] == true)
		{
			KeyData key;
			key.mKey = countEvent;
			key.mRepeat = mpKeyboardData->mKeyStateRepeat[countEvent];			

			eventVect.push_back(key);

			const EKeyboardInput resetLevelKey					= mpSettingsManager->resetLevel;
			const EKeyboardInput resetCheckpointKey				= mpSettingsManager->resetCheckpoint;
			const EKeyboardInput throwProjectileHorizontalKey	= mpSettingsManager->shootProjectileHorizontal;
			const EKeyboardInput throwProjectileVerticalKey		= mpSettingsManager->shootProjectileVertical;
			const EKeyboardInput slashKey					    = mpSettingsManager->slash;     

			if (countEvent == int(resetLevelKey))
			{
				mWorldData.resetStats();
			}
			else if (countEvent == int(resetCheckpointKey))
			{
				mWorldData.resetToCheckpoint();
			}
			else if (countEvent == int(throwProjectileHorizontalKey) and mpKeyboardData->mLastFrameKeyState[countEvent] == false)
			{
				mWorldData.playerShootProjectile(EEntityMovementPath_HORIZONTAL);
			}
			else if (countEvent == int(throwProjectileVerticalKey) and mpKeyboardData->mLastFrameKeyState[countEvent] == false)
			{
				mWorldData.playerShootProjectile(EEntityMovementPath_VERTICAL);
			}
			else if (countEvent == int(slashKey) and mpKeyboardData->mLastFrameKeyState[countEvent] == false)
			{
				mWorldData.playerSwordSlash(mSlashManager);
			}
			else if (countEvent == int(EKeyboardInput_1))
			{
				mpMenuManager->setCurMenuPage(mpMenuManager->mpMenuPages[int(EMenuPageType_MAIN_GAME_MENU)]);
				mpGameStateData->mNextGameState = EGameState_PLAY;
			}
			else if (countEvent == int(EKeyboardInput_2))
			{
				mpMenuManager->setCurMenuPage(mpMenuManager->mpMenuPages[int(EMenuPageType_MAIN_MENU)]);
				mpGameStateData->mNextGameState = EGameState_MENU;
			}
			else if (countEvent == int(EKeyboardInput_ENTER))
			{
			}
		}
	}

	mWorldData.mPlayer.useInput(eventVect);
	eventVect.clear();
}

void GameStatePlay::useMouseCursor()
{
	float x;
	float y;
	SDL_GetMouseState(&x, &y);

	Vect2 pos((int)x, (int)y);
	Vect2 adjustedPos( (int)(x / mpScreen->mGameScreenToGameLevelChunkRatio), (int)(y / mpScreen->mGameScreenToGameLevelChunkRatio));

	TextBox* pTextBox = mpMenuManager->returnMouseTextBox(pos, *mpScreen);
	if (pTextBox != nullptr)
	{
		mpMenuManager->mpCurMenuPage->setCurTextBox(pTextBox);
	}
}

void GameStatePlay::render(GameStateManagerData& gameStateManagerData, MiniGameStateManagerData& miniGameStateManagerData,
	WorldData& worldData)
{
	Level* pCurLevel = mWorldData.mpWorlds[mWorldData.mCurWorldNumber]->mpLevels[mWorldData.mCurLevelNumber];
	SDL_SetRenderDrawColor(mWorldData.mScreen.mpRenderer, 0, 0, 0, 1);
	SDL_FRect screenRect {0.0, 0.0, (float)mWorldData.mScreen.mGameScreenWidth, (float)mWorldData.mScreen.mGameScreenHeight};
	SDL_RenderFillRect(mWorldData.mScreen.mpRenderer, &screenRect);
	mWorldData.renderBackgrounds();
	mWorldData.renderBackgroundEffects();


	//platforms
	//standard platforms
	for (Platform* pPlatform : pCurLevel->mpPlatforms)
	{
		if		(pPlatform->mPrintViaChunk	== true)
		{
			mWorldData.renderEntityViaChunk(pPlatform);
		}
		else if (pPlatform->mSplice			== true)
		{
			mWorldData.renderEntityViaSplice(pPlatform);
		}
		else
		{
			mWorldData.renderEntity(pPlatform);
		}
	}
	//non static
	for (Platform* pPlatform : pCurLevel->mpActiveNonStaticPlatforms)
	{
		if		(pPlatform->mPrintViaChunk == true)
		{
			mWorldData.renderEntityViaChunk(pPlatform);
		}
		else if (pPlatform->mSplice == true)
		{
			mWorldData.renderEntityViaSplice(pPlatform);
		}
		else
		{
			mWorldData.renderEntity(pPlatform);
		}
	}
	//area effect
	for (AreaEffectPlatform* pPlatform : pCurLevel->mpAreaEffectPlatforms)
	{
		if (pPlatform->mIsVisible)
		{
			AnimationManager& animationManager = pPlatform->mAreaEffectAnimationManager;
			ImageObject*	pCurImageObject = pPlatform->mAreaEffectAnimationManager.getCurImage();
			EDirection		imageDirection	= pPlatform->getMovementManager().getCurFacingDirection();
			EImageOffset	imageOffsetType = EImageOffset_MIDDLE;
			Hitbox&			hitbox = pPlatform->mAreaEffectHitbox;

			if      (pPlatform->mPrintViaChunk == true)
			{
				mWorldData.renderEntityViaChunk(pPlatform);
				mWorldData.renderEntityViaChunk(pCurImageObject, hitbox);
			}
			else if (pPlatform->mSplice == true)
			{
				mWorldData.renderEntityViaSplice(pPlatform);
				mWorldData.renderEntityViaSplice(animationManager, hitbox);
			}
			else
			{
				mWorldData.renderEntity(pPlatform);
				mWorldData.renderEntity(pCurImageObject, imageOffsetType, imageDirection, hitbox);
			}
		}
	}

	//collectibles
	for (Collectible* pCollectible : pCurLevel->mpActiveCollectibles)
	{
		mWorldData.renderEntity(pCollectible);
	}

	//enemies
	for (Enemy* pEnemy : pCurLevel->mpActiveEnemies)
	{
		mWorldData.renderEntity(pEnemy);
		//mWorldData.renderEntityWithHitbox(pEnemy);
	}

	//projectiles
	for (Projectile* pProjectile : mWorldData.mpProjectiles)
	{
		mWorldData.renderEntity(pProjectile);
	}
	
	//slash
	if (mSlashManager.mCurSlash)
	{
		Vect2 topLeft = mSlashManager.mCenterOfRotation - Vect2(mSlashManager.mHitbox.getWidth() / 2, mSlashManager.mHitbox.getHeight() / 2);
		Hitbox slashImageHitbox = Hitbox(topLeft, mSlashManager.mAnimationManager.getCurImage()->getIdealImageWidth(), mSlashManager.mAnimationManager.getCurImage()->getIdealImageHeight());
		EImageOffset offsetType = EImageOffset_LEFT_X_MIDDLE_Y;
		bool rotating = true;
		//sword
		mWorldData.renderEntityWithHitbox(mSlashManager.mAnimationManager.getCurImage(), mSlashManager.mSlashImageHitboxTexture, offsetType, mSlashManager.mCurSlashDirection, slashImageHitbox, rotating, degreesToImageRotationDegrees(mSlashManager.mImageRotation, mSlashManager.mCurRotation));
		//slash hitbox
		offsetType = EImageOffset_PRINT_TOP_LEFT;
		rotating = false;
		float rotation = 0;
		Vect2 hitboxOffset = Vect2(0, slashImageHitbox.getHeight() / 2);
		mWorldData.renderTexture(mSlashManager.mHitboxTexture, mSlashManager.mHitbox, offsetType, hitboxOffset, mSlashManager.mCurSlashDirection, rotating, 0.0);
	}
	

	//PLAYER
	mWorldData.renderEntity(&mWorldData.mPlayer);
	mpMenuManager->updateUIElements(mWorldData.mScreen, mWorldData);
	mpMenuManager->setCurMenuPage(mpMenuManager->mpMenuPages[int(EMenuPageType_MAIN_GAME_MENU)]);
	mpMenuManager->renderMenus(mWorldData.mScreen, gameStateManagerData, miniGameStateManagerData, mWorldData, *mpSettingsManager);
}


GameStatePlayMiniGame::GameStatePlayMiniGame(GameStateData& gameStateData, KeyboardData& keyboardData, MouseData& mouseData, 
	MiniGameStateManager& miniGameStateManager, MenuManager& menuManager, ScreenObject& screen, SettingsManager& settingsManager,
	StyleManager& styleManager) : mMiniGameStateManager(miniGameStateManager)
{
	mpKeyboardData		= &keyboardData;
	mpMouseData			= &mouseData;
	mpMenuManager		= &menuManager;
	mpSettingsManager	= &settingsManager;
	mpStyleManager		= &styleManager;
	mpScreen			= &screen;
}

GameStatePlayMiniGame::~GameStatePlayMiniGame()
{
	GameState::~GameState();
}

void GameStatePlayMiniGame::tick(GameStateManagerData& gameStateManagerData, MiniGameStateManager& miniGameStateManager,
	WorldData& worldData)
{
	MiniGameLevel* pCurLevel = mMiniGameStateManager.mData.mpMiniGameWorldData->mpMiniGameLevels[mMiniGameStateManager.mData.mpMiniGameWorldData->mCurMiniGameLevelNumber];

	if (pCurLevel->mpCombatManager->mpCurCombatCharacters.size() == 0)
	{
		pCurLevel->mpCombatManager->createCurCharacterList();
		mMiniGameStateManager.mData.mpData->mpCharacter = pCurLevel->mpCombatManager->mpCurCombatCharacters[0];
	}

	getInput();
	useMouseCursor();
	mMiniGameStateManager.mpCurState->useMouseInput(mMiniGameStateManager.mData.mCurStateEnum, *mpScreen, pCurLevel->mpGrid);

	mMiniGameStateManager.preTick();
	mMiniGameStateManager.tick();

	render(gameStateManagerData, mMiniGameStateManager.mData, worldData);
}

void GameStatePlayMiniGame::useMouseCursor()
{
	float x;
	float y;
	SDL_GetMouseState(&x, &y);

	Vect2 pos((int)x, (int)y);
	Vect2 adjustedPos( int(x / mpScreen->mGameScreenToGameLevelChunkRatio), int(y / mpScreen->mGameScreenToGameLevelChunkRatio));

	mMiniGameStateManager.mpCurState->highlightTile(mMiniGameStateManager.mData.mpMiniGameWorldData->mpMiniGameLevels[mMiniGameStateManager.mData.mpMiniGameWorldData->mCurMiniGameLevelNumber]->mpGrid, adjustedPos);
	TextBox* pTextBox = mpMenuManager->returnMouseTextBox(pos, *mpScreen);
	if (pTextBox != nullptr)
	{
		mpMenuManager->mpCurMenuPage->setCurTextBox(pTextBox);
	}
}

void GameStatePlayMiniGame::render(GameStateManagerData& gameStateManagerData, MiniGameStateManagerData& miniGameStateManagerData,
	WorldData& worldData)
{
	SDL_SetRenderDrawColor(mpScreen->mpRenderer, 0, 0, 0, 1);
	SDL_FRect screenRect {0.0, 0.0, float(mpScreen->mGameScreenWidth), float(mpScreen->mGameScreenHeight)};
	SDL_RenderFillRect(mpScreen->mpRenderer, &screenRect);
	mMiniGameStateManager.mData.printBoard(*mpScreen, *mpStyleManager);
	mpMenuManager->renderMenus(*mpScreen, gameStateManagerData, mMiniGameStateManager.mData, worldData, *mpSettingsManager);
}




GameStateMenu::GameStateMenu(GameStateData& gameStateData, KeyboardData& keyboardData, MouseData& mouseData, 
		MenuManager& menuManager, ScreenObject& screen, SettingsManager& settingsManager, StyleManager& styleManager)
{
	mpKeyboardData		= &keyboardData;
	mpMouseData			= &mouseData;
	mpMenuManager		= &menuManager;
	mpSettingsManager	= &settingsManager;
	mpStyleManager		= &styleManager;
	mpScreen			= &screen;
}

GameStateMenu::~GameStateMenu()
{
	GameState::~GameState();
}

void GameStateMenu::tick(GameStateManagerData& gameStateManagerData, MiniGameStateManager& miniGameStateManager,
	WorldData& worldData)
{
	mTicksSinceInput += 1;
	// Create the resources for rendering.

	SDL_PumpEvents();
	mpMenuManager->preTick();
	//PRE TICK
	getInput();
	useMouseCursor();
	useInput(gameStateManagerData);
	takeMenuAction(miniGameStateManager);

	//RENDERING
	render(gameStateManagerData, miniGameStateManager.mData, worldData);

	return;
}

void GameStateMenu::useInput(GameStateManagerData& gameStateManagerData)
{		
	GameState::useInput(gameStateManagerData);
	static std::vector <int> eventVect;
	for (int countEvent = 0; countEvent < mpKeyboardData->mNumEvents; countEvent++)
	{
		if (mpKeyboardData->mKeyState[countEvent] == true)
		{
			eventVect.push_back(countEvent);
		}
	}
	for (int countEvent = 0; countEvent < mpMouseData->mNumMouseEvents; countEvent++)
	{
		if (mpMouseData->mMouseEventSyms[countEvent] == true)
		{
			eventVect.push_back(countEvent);
		}
	}

	float x;
	float y;
	SDL_GetMouseState(&x, &y);
	Vect2 pos((int)x, (int)y);

	for (int curEventEnum : eventVect)
	{
		switch (curEventEnum)
		{
		case EKeyboardInput_1:
			mpMenuManager->setCurMenuPage(mpMenuManager->mpMenuPages[int(EMenuPageType_MAIN_GAME_MENU)]);
			mpGameStateData->mNextGameState = EGameState_PLAY;
			break;
		case EKeyboardInput_2:
			mpMenuManager->setCurMenuPage(mpMenuManager->mpMenuPages[int(EMenuPageType_MAIN_MENU)]);
			mpGameStateData->mNextGameState = EGameState_MENU;
			break;
		case EKeyboardInput_UP:
			if (mTicksSinceInput > mTicksBeforeUseInput)
			{
				mpMenuManager->mpCurMenuPage->setCurTextBoxIfValid(mpMenuManager->getCurTextBoxIndex() - 1);
				mTicksSinceInput = 0;
			}
			break;

		case EKeyboardInput_DOWN:
			if (mTicksSinceInput > mTicksBeforeUseInput)
			{
				mpMenuManager->mpCurMenuPage->setCurTextBoxIfValid(mpMenuManager->getCurTextBoxIndex() + 1);
				mTicksSinceInput = 0;
			}
			break;

		case EKeyboardInput_ENTER:
			if (mTicksSinceInput > mTicksBeforeUseInput)
			{
				mpMenuManager->mpCurMenuPage->setCurSelectedTextBox(mpMenuManager->mpCurMenuPage->getCurTextBox());
				mTicksSinceInput = 0;
			}
			break;
		case EMouseInput_LEFT:
			if (mTicksSinceInput > mTicksBeforeUseInput)
			{
				TextBox* pTextBox = mpMenuManager->returnMouseTextBox(pos, *mpScreen);
				if (pTextBox)
				{
					mpMenuManager->mpCurMenuPage->setCurSelectedTextBox(pTextBox);
					mpMenuManager->mpCurMenuPage->setCurTextBox(pTextBox);
					mTicksSinceInput = 0;
				}
			}
			break;
		}
			
	}
	eventVect.clear();
}

void GameStateMenu::useMouseCursor()
{
	float x;
	float y;
	SDL_GetMouseState(&x, &y);

	Vect2 pos((int)x, (int)y);
	Vect2 adjustedPos( int(x / mpScreen->mGameScreenToGameLevelChunkRatio), int(y / mpScreen->mGameScreenToGameLevelChunkRatio));

	TextBox* pTextBox = mpMenuManager->returnMouseTextBox(pos, *mpScreen);
	if (pTextBox != nullptr)
	{
		mpMenuManager->mpCurMenuPage->setCurTextBox(pTextBox);
	}
}

void GameStateMenu::render(GameStateManagerData& gameStateManagerData, MiniGameStateManagerData& miniGameStateManagerData,
	WorldData& worldData)
{
	mpMenuManager->renderMenus(*mpScreen, gameStateManagerData, miniGameStateManagerData, worldData, *mpSettingsManager);
}

void GameStateMenu::takeMenuAction(MiniGameStateManager& miniStateManager)
{
	TextBox* curSelectedTextBox = mpMenuManager->mpCurMenuPage->getCurSelectedTextBox();
	MiniGameState* pCurState = miniStateManager.mpCurState;
	if (curSelectedTextBox == nullptr)
	{
		return;
	}


	switch (curSelectedTextBox->mFunction)
	{
	case ETextBoxFunction_PLAY_GAME_BOX:
		mpMenuManager->setCurMenuPage(mpMenuManager->mpMenuPages[int(EMenuPageType_MAIN_GAME_MENU)]);
		mpGameStateData->mNextGameState = EGameState_PLAY;
		break;
	case ETextBoxFunction_GO_TO_MAIN_MENU_BOX:
		mpMenuManager->setCurMenuPage(mpMenuManager->mpMenuPages[int(EMenuPageType_MAIN_MENU)]);
		mpGameStateData->mNextGameState = EGameState_MENU;
		break;
	case ETextBoxFunction_PLAY_MINI_GAME_BOX:
		mpMenuManager->setCurMenuPage(mpMenuManager->mpMenuPages[int(EMenuPageType_MINI_GAME_MENU)]);
		mpGameStateData->mNextGameState = EGameState_PLAY_MINI_GAME;
		break;
	case ETextBoxFunction_ATTACK_CUR_COMBAT_CHARACTER_BOX:
		if (miniStateManager.mData.mCurStateEnum == EMiniGameState_PLAYER_WAIT_FOR_ACTION_INPUT)
		{
			MiniGamePlayerWaitForActionInput* pSpecificCurState = (MiniGamePlayerWaitForActionInput*)pCurState;
			pSpecificCurState->postTick(*miniStateManager.mData.mpMiniGameWorldData, EMiniGameState_PLAYER_WAIT_FOR_ATTACK_INPUT);
		}
		break;
	case ETextBoxFunction_DEFEND_CUR_COMBAT_CHARACTER_BOX:
		if (miniStateManager.mData.mCurStateEnum == EMiniGameState_PLAYER_WAIT_FOR_ACTION_INPUT)
		{
			MiniGamePlayerWaitForActionInput* pSpecificCurState = (MiniGamePlayerWaitForActionInput*)pCurState;
			pSpecificCurState->postTick(*miniStateManager.mData.mpMiniGameWorldData, EMiniGameState_PLAYER_TAKE_ACTION_DEFEND);
		}
		break;
	case ETextBoxFunction_PASS_CUR_COMBAT_CHARACTER_TURN_BOX:
		if (miniStateManager.mData.mCurStateEnum == EMiniGameState_PLAYER_WAIT_FOR_ACTION_INPUT)
		{
			MiniGamePlayerWaitForActionInput* pSpecificCurState = (MiniGamePlayerWaitForActionInput*)pCurState;
			pSpecificCurState->postTick(*miniStateManager.mData.mpMiniGameWorldData, EMiniGameState_BUFFER);
		}
		break;
	case ETextBoxFunction_ATTACK_STYLE_1_BOX:
		if (miniStateManager.mData.mCurStateEnum == EMiniGameState_PLAYER_WAIT_FOR_ATTACK_INPUT)
		{
			MiniGamePlayerWaitForAttackInput * pSpecificCurState = (MiniGamePlayerWaitForAttackInput*)pCurState;
			pSpecificCurState->postTick(miniStateManager.mData.mpData->mpCharacter->mCombatMovementManager.getAttacks()[0]);
		}
		break;
	case ETextBoxFunction_ATTACK_STYLE_2_BOX:
		if (miniStateManager.mData.mCurStateEnum == EMiniGameState_PLAYER_WAIT_FOR_ATTACK_INPUT)
		{
			MiniGamePlayerWaitForAttackInput * pSpecificCurState = (MiniGamePlayerWaitForAttackInput*)pCurState;
			pSpecificCurState->postTick(miniStateManager.mData.mpData->mpCharacter->mCombatMovementManager.getAttacks()[1]);
		}
		break;
	case ETextBoxFunction_ATTACK_DIRECTION_LEFT_BOX:
		if (miniStateManager.mData.mCurStateEnum == EMiniGameState_PLAYER_WAIT_FOR_ATTACK_SUB_INPUT)
		{
			MiniGamePlayerWaitForAttackSubInput* pSpecificCurState = (MiniGamePlayerWaitForAttackSubInput*)pCurState;
			pSpecificCurState->postTick(EDirection_LEFT);
		}
		break;
	case ETextBoxFunction_ATTACK_DIRECTION_RIGHT_BOX:
		if (miniStateManager.mData.mCurStateEnum == EMiniGameState_PLAYER_WAIT_FOR_ATTACK_SUB_INPUT)
		{
			MiniGamePlayerWaitForAttackSubInput* pSpecificCurState = (MiniGamePlayerWaitForAttackSubInput*)pCurState;
			pSpecificCurState->postTick(EDirection_RIGHT);
		}
		break;
	case ETextBoxFunction_ATTACK_DIRECTION_UP_BOX:
		if (miniStateManager.mData.mCurStateEnum == EMiniGameState_PLAYER_WAIT_FOR_ATTACK_SUB_INPUT)
		{
			MiniGamePlayerWaitForAttackSubInput* pSpecificCurState = (MiniGamePlayerWaitForAttackSubInput*)pCurState;
			pSpecificCurState->postTick(EDirection_UP);
		}
		break;
	case ETextBoxFunction_ATTACK_DIRECTION_DOWN_BOX:
		if (miniStateManager.mData.mCurStateEnum == EMiniGameState_PLAYER_WAIT_FOR_ATTACK_SUB_INPUT)
		{
			MiniGamePlayerWaitForAttackSubInput* pSpecificCurState = (MiniGamePlayerWaitForAttackSubInput*)pCurState;
			pSpecificCurState->postTick(EDirection_DOWN);
		}
		break;
	}
}


