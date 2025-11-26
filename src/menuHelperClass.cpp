#pragma once
#include "menuHelperClass.h"

std::string getAttackName(Attack attack)
{
	return attack.mDescription;
}

std::string getAttackType(Attack attack)
{
	return returnDescriptionOfMoveAttackType(attack.mType);
}

std::string getAttackDamage(Attack attack, int characterDamage)
{
	return std::to_string(attack.mDamagePercent * characterDamage);
}

std::string getSpecialEffect(Attack attack)
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
	//TODO trim start
	return message;
}


bool shouldShowTextBox(TextBox* pTextBox, MiniGameStateManagerData& data)
{
	CombatCharacter* combatCharacter = data.mStateData.getCharacter();
	switch (pTextBox->mType)
	{
	case ETextBoxType_MINI_GAME_CHARACTER_BOX:
		if (pTextBox->mDataStorage.mCombatCharacterIndex != -1 && !pTextBox->mDataStorage.mShowDuringAllCharacters && pTextBox->mDataStorage.mCombatCharacterIndex != data.mStateData.getCharacterIndex()) 
		{
			return false;
		}
		return std::find(pTextBox->mDataStorage.mMiniGameStateWhenToShowList.begin(), pTextBox->mDataStorage.mMiniGameStateWhenToShowList.end(), data.mCurStateEnum) != pTextBox->mDataStorage.mMiniGameStateWhenToShowList.end();
		break;
	case ETextBoxType_MINI_GAME_PLAYER_BOX:
		if (combatCharacter == nullptr
			or combatCharacter->mType != EMiniGameCombatCharacterType_PLAYER)
		{
			return false;
		}
		return std::find(pTextBox->mDataStorage.mMiniGameStateWhenToShowList.begin(), pTextBox->mDataStorage.mMiniGameStateWhenToShowList.end(), data.mCurStateEnum) != pTextBox->mDataStorage.mMiniGameStateWhenToShowList.end();
		break;
	case ETextBoxType_MINI_GAME_BOX:
		return std::find(pTextBox->mDataStorage.mMiniGameStateWhenToShowList.begin(), pTextBox->mDataStorage.mMiniGameStateWhenToShowList.end(), data.mCurStateEnum) != pTextBox->mDataStorage.mMiniGameStateWhenToShowList.end();
		break;
	default:
		break;
	}

	return true;
}

bool shouldShowTextBox(ShapeBox* pShapeBox, MiniGameStateManagerData& data)
{
	CombatCharacter* combatCharacter = data.mStateData.getCharacter();
	switch (pShapeBox->mDataStorage.mShowType)
	{
	case EShapeTypeShowType_MINI_GAME_CHARACTER_BOX:
		if (pShapeBox->mDataStorage.mCombatCharacterIndex != -1 && !pShapeBox->mDataStorage.mShowDuringAllCharacters && pShapeBox->mDataStorage.mCombatCharacterIndex != data.mStateData.getCharacterIndex())
		{
			return false;
		}
		return std::find(pShapeBox->mDataStorage.mMiniGameStateWhenToShowList.begin(), pShapeBox->mDataStorage.mMiniGameStateWhenToShowList.end(), data.mCurStateEnum) != pShapeBox->mDataStorage.mMiniGameStateWhenToShowList.end();
		break;
	default:
		break;
	}

	return true;
}

void drawCircle(SDL_Color color, Vect2 center, int radius, ScreenObject& screen)
{
	SDL_Renderer* pRenderer = screen.mpRenderer;
	float gameScreenToGameLevelChunkRatio = screen.mGameScreenToGameLevelChunkRatio;

	float actualCenterX = center.getX() * gameScreenToGameLevelChunkRatio;
	float actualCenterY = center.getY() * gameScreenToGameLevelChunkRatio;
	float actualRadius = radius * gameScreenToGameLevelChunkRatio;


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


std::string updateGameStatBoxCurTextBoxMessage(TextBox* pTextBox, std::string& curKeys, WorldData& worldData, SettingsManager& settingsManager)
{
	switch (pTextBox->mDataStorage.mGameStatToDisplay)
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

std::string updateMiniGameStatBoxCurTextBoxMessage(TextBox* pTextBox, MiniGameStateManagerData& managerData, MiniGameWorldData& worldData)
{
	switch (pTextBox->mDataStorage.mGameStatToDisplay)
	{
	case EGameStatBoxValueToDisplay_CUR_LEVEL_NUMBER:
		return "CUR LEVEL: " + std::to_string(worldData.mCurMiniGameLevelNumber);
		break;
	default:
		SDL_assert(false);
		return "UNKNOWN TYPE";
		break;
	}
}

std::string updateCharacterStatBoxCurTextBoxMessage(TextBox* pTextBox, MiniGameStateManagerData& managerData, MiniGameWorldData& worldData)
{
	CombatCharacter* pCharacter = managerData.mStateData.getCharacter();
	if (pTextBox->mDataStorage.mCombatCharacterIndex != -1) {
		pCharacter = worldData.mpMiniGameLevels[worldData.mCurMiniGameLevelNumber]->mCombatManager.mpAllCombatCharacters[pTextBox->mDataStorage.mCombatCharacterIndex];
	}
	if (pCharacter == nullptr) 
	{
		return "null character";
	}

	switch (pTextBox->mDataStorage.mCharacterStatToDisplay)
	{
	case ECharacterStatBoxValueToDisplay_CHARACTER_NAME:
		return pCharacter->mName;
	case ECharacterStatBoxValueToDisplay_CHARACTER_HEALTH:
		return std::to_string(pCharacter->mCurHealth);
	case ECharacterStatBoxValueToDisplay_CHARACTER_DEFENSE:
		return std::to_string(pCharacter->mCurDefense);
	case ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK:
		return std::to_string(pCharacter->mCurAttackDamage);
	case ECharacterStatBoxValueToDisplay_CHARACTER_STUN:
		return std::to_string(pCharacter->mTurnsToPass);
	case ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_NAME:
		return getAttackName(pCharacter->mCombatMovementManager.getAttacks()[pTextBox->mDataStorage.mAttackNum]);
	case ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_TYPE:
		return getAttackType(pCharacter->mCombatMovementManager.getAttacks()[pTextBox->mDataStorage.mAttackNum]);
	case ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_DAMAGE:
		return getAttackDamage(pCharacter->mCombatMovementManager.getAttacks()[pTextBox->mDataStorage.mAttackNum], pCharacter->mCurAttackDamage);
	case ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_SPECIAL_EFFECTS_AND_NOTES:
		return getSpecialEffect(pCharacter->mCombatMovementManager.getAttacks()[pTextBox->mDataStorage.mAttackNum]);
	case ECharacterStatBoxValueToDisplay_CHARACTER_CUR_ATTACK_NAME:
		return managerData.mStateData.mpCurAttack->mName;
	case ECharacterStatBoxValueToDisplay_CHARACTER_MOVE_TYPE:
		return returnDescriptionOfMoveAttackType(pCharacter->mCombatMovementManager.getMoveType());
	case ECharacterStatBoxValueToDisplay_INVALID:
		return pTextBox->mDataStorage.mMessage;
	default:
		SDL_assert(false);
	}

	return "error";
}

std::string updateHealthStatBoxCurTextBoxMessage(HealthBox* pHealthBox, MiniGameWorldData& worldData)
		{ return std::to_string(worldData.mpMiniGameLevels[worldData.mCurMiniGameLevelNumber]->mCombatManager.mpAllCombatCharacters[pHealthBox->mCombatCharacterIndex]->mCurHealth); }
