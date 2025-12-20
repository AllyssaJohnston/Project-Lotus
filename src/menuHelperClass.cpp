#pragma once
#include "menuHelperClass.h"

std::string getAttackType(const Attack& attack) { return returnDescriptionOfMoveAttackType(attack.mType); }

std::string getAttackDamage(const Attack& attack, int characterDamage) { return std::to_string((int)(attack.mDamagePercent * characterDamage)); }

std::string getSpecialEffect(const Attack& attack)
{
	std::string message;
	std::string requiresDirectionalInput;
	if (attack.mRequiresDirectionInput)
	{
		requiresDirectionalInput = "Requires directional input";
	}
	std::string specialEffects;
	for (SpecialEffect curSpecialEffect : attack.mSpecialEffects)
	{
		specialEffects += curSpecialEffect.mName + " ";
	}
	std::string distanceDependent;
	if (attack.mDamageDistanceDependent)
	{
		requiresDirectionalInput = "Damage is distance dependent";
	}
	message = requiresDirectionalInput + " " + specialEffects + " " + distanceDependent;
	// TODO trim start
	return message;
}


bool shouldShowTextBox(const TextBox& textBox, const MiniGameStateManagerData& data)
{
	CombatCharacter* pCombatCharacter = data.mStateData.getCharacter();
	const std::vector<EMiniGameState>& miniGameStateWhenToShowList = textBox.mData.mMiniGameStateWhenToShowList;
	switch (textBox.mData.mType)
	{
	case ETextBoxType_MINI_GAME_CHARACTER_BOX:
		if (textBox.mData.mCombatCharacterIndex != -1 && !textBox.mData.mShowDuringAllCharacters && textBox.mData.mCombatCharacterIndex != data.mStateData.getCharacterIndex()) 
		{
			return false;
		}
		return std::find(miniGameStateWhenToShowList.begin(), miniGameStateWhenToShowList.end(), data.mCurStateEnum) != miniGameStateWhenToShowList.end();
		break;
	case ETextBoxType_MINI_GAME_PLAYER_BOX:
		if (pCombatCharacter == nullptr or pCombatCharacter->mType != EMiniGameCombatCharacterType_PLAYER)
		{
			return false;
		}
		return std::find(miniGameStateWhenToShowList.begin(), miniGameStateWhenToShowList.end(), data.mCurStateEnum) != miniGameStateWhenToShowList.end();
		break;
	case ETextBoxType_MINI_GAME_BOX:
		return std::find(miniGameStateWhenToShowList.begin(), miniGameStateWhenToShowList.end(), data.mCurStateEnum) != miniGameStateWhenToShowList.end();
		break;
	default:
		break;
	}

	return true;
}

bool shouldShowTextBox(const ShapeBox& shapeBox, const MiniGameStateManagerData& data)
{
	const std::vector<EMiniGameState>& miniGameStateWhenToShowList = shapeBox.mDataStorage.mMiniGameStateWhenToShowList;
	switch (shapeBox.mDataStorage.mShowType)
	{
	case EShapeTypeShowType_MINI_GAME_CHARACTER_BOX:
		if (shapeBox.mDataStorage.mCombatCharacterIndex != -1 && !shapeBox.mDataStorage.mShowDuringAllCharacters && shapeBox.mDataStorage.mCombatCharacterIndex != data.mStateData.getCharacterIndex())
		{
			return false;
		}
		return std::find(miniGameStateWhenToShowList.begin(), miniGameStateWhenToShowList.end(), data.mCurStateEnum) != miniGameStateWhenToShowList.end();
		break;
	default:
		break;
	}

	return true;
}

void drawCircle(const SDL_Color& color, const Vect2& center, int radius, ScreenObject& screen)
{
	SDL_Renderer* pRenderer = screen.mpRenderer;

	float actualCenterX = center.getX() * screen.mGameScreenToGameLevelChunkRatio;
	float actualCenterY = center.getY() * screen.mGameScreenToGameLevelChunkRatio;
	float actualRadius = radius * screen.mGameScreenToGameLevelChunkRatio;


	for (int degrees = 0; degrees < 360; degrees++)
	{
		for (int curRadius = 0; curRadius < actualRadius; curRadius++)
		{
			float radians = (float)degToRad(degrees);
			float curX = actualCenterX + (cos(radians) * curRadius);
			float curY = actualCenterY + (sin(radians) * curRadius);
			float alpha = (actualRadius - curRadius) / actualRadius;
			SDL_SetRenderDrawColor(pRenderer, color.r, color.g, color.b, Uint8(alpha));
			SDL_RenderPoint(pRenderer, curX, curY);
		}
	}
}


std::string updateGameStatBoxCurTextBoxMessage(const TextBox& textBox, const std::string& curKeys, const WorldData& worldData, const SettingsManager& settingsManager)
{
	switch (textBox.mData.mGameStatToDisplay)
	{
	case EGameStatBoxValueToDisplay_CUR_LEVEL_NUMBER:
		return "CUR LEVEL: " + std::to_string(worldData.mCurWorldNumber) + " - " + std::to_string(worldData.mCurLevelNumber);
	case EGameStatBoxValueToDisplay_CUR_ENEMIES_LEFT:
		return std::to_string(worldData.mpWorlds[worldData.mCurWorldNumber]->mpLevels[worldData.mCurLevelNumber]->mpActiveEnemies.size());
	case EGameStatBoxValueToDisplay_CUR_KEYS:
		return std::to_string(worldData.mPlayer.mKeys);
	case EGameStatBoxValueToDisplay_CUR_TARGETS:
		return std::to_string(worldData.mPlayer.mTargets);
	case EGameStatBoxValueToDisplay_CUR_COLLECTIBLES:
		return std::to_string(worldData.mNumLotusCollectibles);
	case EGameStatBoxValueToDisplay_MOVEMENT_LEFT_KEY:
		return createStringFromKeyboardList(settingsManager.leftKeys);
	case EGameStatBoxValueToDisplay_MOVEMENT_RIGHT_KEY:
		return createStringFromKeyboardList(settingsManager.rightKeys);
	case EGameStatBoxValueToDisplay_MOVEMENT_UP_KEY:
		return createStringFromKeyboardList(settingsManager.upKeys);
	case EGameStatBoxValueToDisplay_LEVEL_RESET_KEY:
		return keyboardToStringMap.at(settingsManager.resetLevel);
	case EGameStatBoxValueToDisplay_CHECKPOINT_RESET_KEY:
		return keyboardToStringMap.at(settingsManager.resetCheckpoint);
	case EGameStatBoxValueToDisplay_PROJECTILE_HORIZONTAL_KEY:
		if (worldData.mpWorlds[worldData.mCurWorldNumber]->mpLevels[worldData.mCurLevelNumber]->mThrowProjectileAllowed)
		{
			return keyboardToStringMap.at(settingsManager.shootProjectileHorizontal);
		}
		return " ";
	case EGameStatBoxValueToDisplay_PROJECTILE_VERTICAL_KEY:
		if (worldData.mpWorlds[worldData.mCurWorldNumber]->mpLevels[worldData.mCurLevelNumber]->mThrowDownwardProjectileAllowed)
		{
			return keyboardToStringMap.at(settingsManager.shootProjectileVertical);
		}
		return " ";
	case EGameStatBoxValueToDisplay_DOUBLE_JUMP_KEY:
		if (worldData.mpWorlds[worldData.mCurWorldNumber]->mpLevels[worldData.mCurLevelNumber]->mDoubleJumpAllowed)
		{
			return createStringFromKeyboardList(settingsManager.upKeys);
		}
		return " ";
	case EGameStatBoxValueToDisplay_SLASH_KEY:
		if (worldData.mpWorlds[worldData.mCurWorldNumber]->mpLevels[worldData.mCurLevelNumber]->mSlashAllowed)
		{
			return keyboardToStringMap.at(settingsManager.slash);
		}
		return " ";
	case EGameStatBoxValueToDisplay_CUR_KEYBOARD:
		return curKeys;
	default:
		SDL_assert(false);
		break;
	}
	return "error";
}

std::string updateMiniGameStatBoxCurTextBoxMessage(const TextBox& textBox, const MiniGameStateManagerData& managerData, const MiniGameWorldData& worldData)
{
	switch (textBox.mData.mGameStatToDisplay)
	{
	case EGameStatBoxValueToDisplay_CUR_LEVEL_NUMBER:
		return "CUR LEVEL: " + std::to_string(worldData.mCurWorldNumber) + " - " + std::to_string(worldData.mCurLevelNumber) + " - " + std::to_string(worldData.mCurStageNumber);
	case EGameStatBoxValueToDisplay_MINI_GAME_DEBUG_LINE:
		return managerData.mStateData.mDebugLine;
	default:
		SDL_assert(false);
		return "UNKNOWN TYPE";
	}
}

std::string updateCharacterStatBoxCurTextBoxMessage(const TextBox& textBox, const MiniGameStateManagerData& managerData, const MiniGameWorldData& worldData)
{
	CombatCharacter* pCharacter = managerData.mStateData.getCharacter(); // default to the cur character
	if (textBox.mData.mCombatCharacterIndex != -1) // requested a specific character
	{
		pCharacter = worldData.getStage()->mCombatManager.mpAllCombatCharacters[textBox.mData.mCombatCharacterIndex];
	}
	if (pCharacter == nullptr) 
	{
		return "null character";
	}

	switch (textBox.mData.mCharacterStatToDisplay)
	{
	case ECharacterStatBoxValueToDisplay_CHARACTER_NAME:
		return pCharacter->mName;
	case ECharacterStatBoxValueToDisplay_CHARACTER_HEALTH:
		return std::to_string(pCharacter->getCurHealth());
	case ECharacterStatBoxValueToDisplay_CHARACTER_DEFENSE:
		return std::to_string(pCharacter->getCurDefense());
	case ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK:
		return std::to_string(pCharacter->getCurDamage());
	case ECharacterStatBoxValueToDisplay_CHARACTER_STUN:
		return std::to_string(pCharacter->getStuns());
	case ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_NAME:
		return pCharacter->mCombatMovementManager.getAttacks()[textBox.mData.mAttackNum].mName;
	case ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_TYPE:
		return getAttackType(pCharacter->mCombatMovementManager.getAttacks()[textBox.mData.mAttackNum]);
	case ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_DAMAGE:
		return getAttackDamage(pCharacter->mCombatMovementManager.getAttacks()[textBox.mData.mAttackNum], pCharacter->getBaseDamage());
	case ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_SPECIAL_EFFECTS_AND_NOTES:
		return getSpecialEffect(pCharacter->mCombatMovementManager.getAttacks()[textBox.mData.mAttackNum]);
	case ECharacterStatBoxValueToDisplay_CHARACTER_CUR_ATTACK_NAME:
		return managerData.mStateData.mpCurAttack->mName;
	case ECharacterStatBoxValueToDisplay_CHARACTER_MOVE_TYPE:
		return returnDescriptionOfMoveAttackType(pCharacter->mCombatMovementManager.getMoveType());
	case ECharacterStatBoxValueToDisplay_INVALID:
		return textBox.mMessage;
	default:
		SDL_assert(false);
	}

	return "error";
}

std::string updateHealthStatBoxCurTextBoxMessage(const HealthBox& healthBox, const MiniGameWorldData& worldData) { return std::to_string(worldData.getStage()->mCombatManager.mpAllCombatCharacters[healthBox.mCombatCharacterIndex]->getCurHealth()); }
