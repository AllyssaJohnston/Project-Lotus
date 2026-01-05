#include "menuDataController.h"

MenuDataController::MenuDataController(MenuManager& menuManager, WorldData& worldData, MiniGameStateManagerData& miniGameStateManagerData,
	MiniGameWorldData& miniWorldData, ScreenObject& screen, SettingsManager& settingsManager, FontSizeChart& fontSizeChart, KeyboardData& keyboardData) : mMenuManager(menuManager), mWorldData(worldData), mMiniGameStateManagerData(miniGameStateManagerData),
	mMiniGameWorldData(miniWorldData), mScreen(screen), mSettingsManager(settingsManager), mFontSizeChart(fontSizeChart), mKeyboardData(keyboardData) { ; }


void MenuDataController::getUpdatedMenuBoxes(EGameState curState, bool forceUpdate)
{
	SDL_Renderer* pRenderer = mScreen.mpRenderer;

	bool updated = false;
	if (shouldUpdateTextBoxShowState(curState, forceUpdate))
	{
		updated = true;
		updateShowStateForCurPageUIBoxes();
	}
	if (mMenuManager.mpCurMenuPage->getCurTextBox() == nullptr)
	{
		mMenuManager.mpCurMenuPage->setDefaultSelectedBox();
	}
	if (mMenuManager.mpCurMenuPage->curTextBoxChange())
	{
		updated = true;
	}

	for (TextBox* pCurTextBox : mMenuManager.mpCurMenuPage->getCurTextBoxes())
	{
		std::string updatedMessage = pCurTextBox->mMessage;
		switch (pCurTextBox->mData.mType)
		{
		case EUIBoxType_GAME_STAT_BOX:
			updatedMessage = updateGameStatBoxCurTextBoxMessage(*pCurTextBox);
			break;
		case EUIBoxType_MINI_GAME_STAT_BOX:
			updatedMessage = updateMiniGameStatBoxCurTextBoxMessage(*pCurTextBox);
			break;
		case EUIBoxType_MINI_GAME_PLAYER_BOX:
		case EUIBoxType_MINI_GAME_CHARACTER_BOX:
			updatedMessage = updateCharacterStatBoxCurTextBoxMessage(*pCurTextBox);
			break;
		case EUIBoxType_MINI_GAME_PLAYER_ATTACK_BOX:
			updatedMessage = updateCharacterStatBoxCurTextBoxMessage(*pCurTextBox);
			updateUIBoxDisable(*pCurTextBox);
			break;
		default:
			break;
		}

		if ((pCurTextBox->mMessage != updatedMessage) or !pCurTextBox->mSetUp)
		{
			pCurTextBox->updateMessage(pRenderer, mFontSizeChart, updatedMessage);
			updated = true;
		}
	}

	for (HealthBox* pHealthBox : mMenuManager.mpCurMenuPage->mpHealthBoxes)
	{
		std::string updatedMessage = updateHealthStatBoxCurTextBoxMessage(*pHealthBox);

		if ((pHealthBox->mHealthText.mMessage != updatedMessage) or !pHealthBox->mHealthText.mSetUp)
		{
			float ratio = updateHealthStatBoxCurTextBoxRatio(*pHealthBox);
			pHealthBox->updateMessage(pRenderer, mFontSizeChart, updatedMessage, ratio);
			updated = true;
		}
		else
		{
			float ratio = updateHealthStatBoxCurTextBoxRatio(*pHealthBox);
			pHealthBox->updateRatio(ratio);
		}
	}

	if (updated)
	{
		mMenuManager.mpCurMenuPage->adjustBlocks();
	}
}

void MenuDataController::updateUIElements()
{
	const Level& curLevel = *mWorldData.mpWorlds[mWorldData.mCurWorldNumber]->mpLevels[mWorldData.mCurLevelNumber];

	int curProjectileBoxIndex = -1;
	for (int count = 0; count < mMenuManager.mpCurMenuPage->mpImageBoxes.size(); count++)
	{
		ImageBox& curImageBox = *mMenuManager.mpCurMenuPage->mpImageBoxes[count];
		switch (curImageBox.mData.mType)
		{
		case EUIBoxType_TAKE_DAMAGE_SCREEN:
			curImageBox.mShow = mWorldData.mPlayer.takingDamage();
			break;
		case EUIBoxType_PROJECTILE_UI:
			if (curProjectileBoxIndex == -1)
			{
				curProjectileBoxIndex = count;
			}
			break;
		case EUIBoxType_DOUBLE_JUMP_UI:
			curImageBox.mShow = curLevel.mDoubleJumpAllowed;
			break;
		case EUIBoxType_SLASH_UI:
			curImageBox.mShow = curLevel.mSlashAllowed;
			break;
		default:
			break;
		}
	}

	// draw available player projectiles
	if (curLevel.mThrowProjectileAllowed)
	{
		for (int count = 0; count < mWorldData.mProjectileLimit - mWorldData.getNumPlayerProjectiles(); count++)
		{
			ImageBox& curImageBox = *mMenuManager.mpCurMenuPage->mpImageBoxes[curProjectileBoxIndex];
			curImageBox.mShow = true;
			curProjectileBoxIndex++;
		}
		for (int count = mWorldData.mProjectileLimit - mWorldData.getNumPlayerProjectiles(); count < mWorldData.mMaxProjectileLimit; count++)
		{
			ImageBox& curImageBox = *mMenuManager.mpCurMenuPage->mpImageBoxes[curProjectileBoxIndex];
			curImageBox.mShow = false;
			curProjectileBoxIndex++;
		}
	}
	else
	{
		for (int count = 0; count < mWorldData.mMaxProjectileLimit; count++)
		{
			ImageBox& curImageBox = *mMenuManager.mpCurMenuPage->mpImageBoxes[curProjectileBoxIndex];
			curImageBox.mShow = false;
			curProjectileBoxIndex++;
		}
	}

}

bool MenuDataController::shouldUpdateTextBoxShowState(EGameState curState, bool forceUpdate)
{
	// Mini Game Option Boxes have a whenToShowList 
	bool didMiniGameStateChange = (mMiniGameStateManagerData.mLastFrameStateEnum != mMiniGameStateManagerData.mCurStateEnum || mMiniGameStateManagerData.mTicksSinceUndo <= 2);
	bool isMiniGame = (curState == EGameState_PLAY_MINI_GAME);

	return (isMiniGame and didMiniGameStateChange) or forceUpdate;
}

bool MenuDataController::shouldShowUIBox(const UIBoxData& data)
{
	CombatManager& combatManager = mMiniGameWorldData.getStage()->mCombatManager;
	CombatCharacter* pCombatCharacter = mMiniGameStateManagerData.mStateData.getCharacter();
	const std::vector<EMiniGameState>& miniGameStateWhenToShowList = data.mMiniGameStateWhenToShowList;
	switch (data.mType)
	{
	case EUIBoxType_MINI_GAME_CHARACTER_BOX:
		if (data.mCombatCharacterIndex != -1 && !data.mShowDuringAllCharacters && data.mCombatCharacterIndex != mMiniGameStateManagerData.mStateData.getCharacterIndex())
		{
			return false;
		}
		if (data.mCombatCharacterIndex != -1 && !combatManager.getFromAllCharacters(data.mCombatCharacterIndex)->isAlive() || data.mCombatCharacterIndex != -1 && !combatManager.getFromAllCharacters(data.mCombatCharacterIndex)->isStarted())
		{
			return false;
		}
		if (data.mCombatCharacterType != ECombatCharacterType_CHARACTER && data.mCombatCharacterType != mMiniGameStateManagerData.mStateData.mTargetCharacterType)
		{
			return false;
		}
		return std::find(miniGameStateWhenToShowList.begin(), miniGameStateWhenToShowList.end(), mMiniGameStateManagerData.mCurStateEnum) != miniGameStateWhenToShowList.end();
	case EUIBoxType_MINI_GAME_PLAYER_BOX:
		if (pCombatCharacter == nullptr or pCombatCharacter->mType != ECombatCharacterType_PLAYER)
		{
			return false;
		}
		return std::find(miniGameStateWhenToShowList.begin(), miniGameStateWhenToShowList.end(), mMiniGameStateManagerData.mCurStateEnum) != miniGameStateWhenToShowList.end();
	case EUIBoxType_MINI_GAME_PLAYER_ATTACK_BOX:
		if (data.mCombatCharacterIndex != -1 && !data.mShowDuringAllCharacters && data.mCombatCharacterIndex != mMiniGameStateManagerData.mStateData.getCharacterIndex())
		{
			return false;
		}
		if (std::find(miniGameStateWhenToShowList.begin(), miniGameStateWhenToShowList.end(), mMiniGameStateManagerData.mCurStateEnum) != miniGameStateWhenToShowList.end())
		{
			return data.mAttackCategory == mMiniGameStateManagerData.mStateData.mAttackCategory;
		}
		return false;
	case EUIBoxType_MINI_GAME_BOX:
	case EUIBoxType_MINI_GAME_DIRECTION_BOX:
		return std::find(miniGameStateWhenToShowList.begin(), miniGameStateWhenToShowList.end(), mMiniGameStateManagerData.mCurStateEnum) != miniGameStateWhenToShowList.end();
	default:
		break;
	}

	return true;
}

void MenuDataController::updateShowStateForCurPageUIBoxes()
{
	// update show state for all textboxes
	for (TextBox* pCurTextBox : mMenuManager.mpCurMenuPage->mpAllSelectableTextBoxes)
	{
		pCurTextBox->mShow = shouldShowUIBox(pCurTextBox->mData);
	}

	for (TextBox* pCurTextBox : mMenuManager.mpCurMenuPage->mpAllDisplayOnlyTextBoxes)
	{
		pCurTextBox->mShow = shouldShowUIBox(pCurTextBox->mData);
	}

	for (ImageBox* pImageBox : mMenuManager.mpCurMenuPage->mpImageBoxes)
	{
		pImageBox->mShow = shouldShowUIBox(pImageBox->mData);
	}

	for (ShapeBox* pShapeBox : mMenuManager.mpCurMenuPage->mpShapeBoxes)
	{
		pShapeBox->mShow = shouldShowUIBox(pShapeBox->mData);
	}

	for (HealthBox* pHealthBox : mMenuManager.mpCurMenuPage->mpHealthBoxes)
	{
		pHealthBox->mShow = shouldShowUIBox(pHealthBox->mData);
	}
}


std::string MenuDataController::updateGameStatBoxCurTextBoxMessage(const TextBox& textBox)
{
	switch (textBox.mData.mGameStatToDisplay)
	{
	case EUIBoxValueToDisplay_CUR_LEVEL_NUMBER:		return "CUR LEVEL: " + std::to_string(mWorldData.mCurWorldNumber) + " - " + std::to_string(mWorldData.mCurLevelNumber);
	case EUIBoxValueToDisplay_CUR_ENEMIES_LEFT:		return std::to_string(mWorldData.mpWorlds[mWorldData.mCurWorldNumber]->mpLevels[mWorldData.mCurLevelNumber]->mpActiveEnemies.size());
	case EUIBoxValueToDisplay_CUR_KEYS:				return std::to_string(mWorldData.mPlayer.mKeys);
	case EUIBoxValueToDisplay_CUR_TARGETS:			return std::to_string(mWorldData.mPlayer.mTargets);
	case EUIBoxValueToDisplay_CUR_COLLECTIBLES:		return std::to_string(mWorldData.mNumLotusCollectibles);
	case EUIBoxValueToDisplay_MOVEMENT_LEFT_KEY:	return createStringFromKeyboardList(mSettingsManager.leftKeys);
	case EUIBoxValueToDisplay_MOVEMENT_RIGHT_KEY:	return createStringFromKeyboardList(mSettingsManager.rightKeys);
	case EUIBoxValueToDisplay_MOVEMENT_UP_KEY:		return createStringFromKeyboardList(mSettingsManager.upKeys);
	case EUIBoxValueToDisplay_LEVEL_RESET_KEY:		return keyboardToStringMap.at(mSettingsManager.resetLevel);
	case EUIBoxValueToDisplay_CHECKPOINT_RESET_KEY: return keyboardToStringMap.at(mSettingsManager.resetCheckpoint);
	case EUIBoxValueToDisplay_PROJECTILE_HORIZONTAL_KEY:
		if (mWorldData.mpWorlds[mWorldData.mCurWorldNumber]->mpLevels[mWorldData.mCurLevelNumber]->mThrowProjectileAllowed)
		{
			return keyboardToStringMap.at(mSettingsManager.shootProjectileHorizontal);
		}
		return " ";
	case EUIBoxValueToDisplay_PROJECTILE_VERTICAL_KEY:
		if (mWorldData.mpWorlds[mWorldData.mCurWorldNumber]->mpLevels[mWorldData.mCurLevelNumber]->mThrowDownwardProjectileAllowed)
		{
			return keyboardToStringMap.at(mSettingsManager.shootProjectileVertical);
		}
		return " ";
	case EUIBoxValueToDisplay_DOUBLE_JUMP_KEY:
		if (mWorldData.mpWorlds[mWorldData.mCurWorldNumber]->mpLevels[mWorldData.mCurLevelNumber]->mDoubleJumpAllowed)
		{
			return createStringFromKeyboardList(mSettingsManager.upKeys);
		}
		return " ";
	case EUIBoxValueToDisplay_SLASH_KEY:
		if (mWorldData.mpWorlds[mWorldData.mCurWorldNumber]->mpLevels[mWorldData.mCurLevelNumber]->mSlashAllowed)
		{
			return keyboardToStringMap.at(mSettingsManager.slash);
		}
		return " ";
	case EUIBoxValueToDisplay_CUR_KEYBOARD: return mKeyboardData.mCurKeysString;
	default:
		SDL_assert(false);
		break;
	}
	return "error";
}

std::string MenuDataController::updateMiniGameStatBoxCurTextBoxMessage(const TextBox& textBox)
{
	switch (textBox.mData.mGameStatToDisplay)
	{
	case EUIBoxValueToDisplay_CUR_LEVEL_NUMBER:
		return "CUR LEVEL: " + std::to_string(mMiniGameWorldData.mCurWorldNumber) + " - " + std::to_string(mMiniGameWorldData.mCurLevelNumber) + " - " + std::to_string(mMiniGameWorldData.mCurStageNumber);
	case EUIBoxValueToDisplay_COMBAT_ROUND_NUM:
		return "ROUND: " + std::to_string(mMiniGameWorldData.getStage()->mCombatManager.getRoundNum());
	case EUIBoxValueToDisplay_MINI_GAME_DEBUG_LINE:
		return mMiniGameStateManagerData.mStateData.mDebugLine;
	default:
		SDL_assert(false);
		return "UNKNOWN TYPE";
	}
}

std::string MenuDataController::updateCharacterStatBoxCurTextBoxMessage(const TextBox& textBox)
{
	CombatManager& combatManager = mMiniGameWorldData.getStage()->mCombatManager;
	CombatCharacter* pCharacter = mMiniGameStateManagerData.mStateData.getCharacter(); // default to the cur character
	if (textBox.mData.mCombatCharacterIndex != -1) // requested a specific character
	{
		pCharacter = combatManager.getFromAllCharacters(textBox.mData.mCombatCharacterIndex);
	}
	if (pCharacter == nullptr)
	{
		return "null character";
	}

	switch (textBox.mData.mGameStatToDisplay)
	{
	case EUIBoxValueToDisplay_CHARACTER_NAME:					return pCharacter->mName;
	case EUIBoxValueToDisplay_CHARACTER_HEALTH:					return std::to_string(pCharacter->getCurHealth());
	case EUIBoxValueToDisplay_CHARACTER_HEALTH_MODIFIER_AMOUNT:	return std::to_string(pCharacter->getCurHealthModifier());
	case EUIBoxValueToDisplay_CHARACTER_DEFENSE:				return std::to_string(pCharacter->getCurDefense());
	case EUIBoxValueToDisplay_CHARACTER_ATTACK:					return std::to_string(pCharacter->getCurDamage());
	case EUIBoxValueToDisplay_CHARACTER_STUN:					return std::to_string(pCharacter->getStuns());
	case EUIBoxValueToDisplay_CHARACTER_ATTACK_OPTION_NAME:		return pCharacter->mCombatMovementManager.getAttacks()[textBox.mData.mAttackNum].mName;
	case EUIBoxValueToDisplay_CHARACTER_ATTACK_OPTION_TYPE:		return getAttackType(pCharacter->mCombatMovementManager.getAttacks()[textBox.mData.mAttackNum]);
	case EUIBoxValueToDisplay_CHARACTER_ATTACK_OPTION_CUR_COOLDOWN:
		if (pCharacter->mCombatMovementManager.getAttacks()[textBox.mData.mAttackNum].mCurCooldown > 0)
		{
			return getAttackCooldownAmount(pCharacter->mCombatMovementManager.getAttacks()[textBox.mData.mAttackNum]) + " TURN COOLDOWN. Currently on " + getAttackCurCooldown(pCharacter->mCombatMovementManager.getAttacks()[textBox.mData.mAttackNum]) + " turn cooldown";
		}
		else
		{
			return getAttackCooldownAmount(pCharacter->mCombatMovementManager.getAttacks()[textBox.mData.mAttackNum]) + " TURN COOLDOWN";
		}
	case EUIBoxValueToDisplay_CHARACTER_ATTACK_OPTION_COOLDOWN_AMOUNT:	return getAttackCooldownAmount(pCharacter->mCombatMovementManager.getAttacks()[textBox.mData.mAttackNum]) + " TURN COOLDOWN";
	case EUIBoxValueToDisplay_CHARACTER_ATTACK_OPTION_DAMAGE:			return getAttackDamage(pCharacter->mCombatMovementManager.getAttacks()[textBox.mData.mAttackNum], pCharacter->getBaseDamage());
	case EUIBoxValueToDisplay_CHARACTER_ATTACK_OPTION_SPECIAL_EFFECTS_AND_NOTES:	return getSpecialEffectAndNotes(pCharacter->mCombatMovementManager.getAttacks()[textBox.mData.mAttackNum]);

	case EUIBoxValueToDisplay_CHARACTER_CUR_ATTACK_NAME:	return mMiniGameStateManagerData.mStateData.mpCurAttack->mName;
	case EUIBoxValueToDisplay_CHARACTER_MOVE_TYPE:			return returnDescriptionOfGridPattern(pCharacter->mCombatMovementManager.getMoveType(), pCharacter->mCombatMovementManager.getMoveNum(), pCharacter->mCombatMovementManager.getMoveOut());
	case EUIBoxValueToDisplay_INVALID:	return textBox.mMessage;
	default:
		SDL_assert(false);
	}

	return "error";
}

std::string MenuDataController::updateHealthStatBoxCurTextBoxMessage(const HealthBox& healthBox)
{
	CombatManager& combatManager = mMiniGameWorldData.getStage()->mCombatManager;
	switch (healthBox.mData.mGameStatToDisplay)
	{
	case EUIBoxValueToDisplay_CHARACTER_HEALTH:
		return std::to_string(combatManager.getFromAllCharacters(healthBox.mData.mCombatCharacterIndex)->getCurHealth());
	case EUIBoxValueToDisplay_CHARACTER_DEFENSE:
		return std::to_string(combatManager.getFromAllCharacters(healthBox.mData.mCombatCharacterIndex)->getCurDefense());
	default:
		SDL_assert(false);
		break;
	}
	return "invalid";
}

void MenuDataController::updateUIBoxDisable(TextBox& textBox)
{
	CombatManager& combatManager = mMiniGameWorldData.getStage()->mCombatManager;
	if (textBox.mShow && textBox.mData.mType == EUIBoxType_MINI_GAME_PLAYER_ATTACK_BOX)
	{
		textBox.changeIsDisabled(!combatManager.getFromAllCharacters(textBox.mData.mCombatCharacterIndex)->mCombatMovementManager.getAttacks()[textBox.mData.mAttackNum].canUse());
	}
}

float MenuDataController::updateHealthStatBoxCurTextBoxRatio(const HealthBox& healthBox)
{
	CombatManager& combatManager = mMiniGameWorldData.getStage()->mCombatManager;
	switch (healthBox.mData.mGameStatToDisplay)
	{
	case EUIBoxValueToDisplay_CHARACTER_HEALTH:
		return combatManager.getFromAllCharacters(healthBox.mData.mCombatCharacterIndex)->getHealthRatio();
	case EUIBoxValueToDisplay_CHARACTER_DEFENSE:
		return combatManager.getFromAllCharacters(healthBox.mData.mCombatCharacterIndex)->getDefenseRatio();
	default:
		SDL_assert(false);
		break;
	}
	return -1.0f;
}
