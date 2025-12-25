#pragma once
#include "menuHelpers.h"



bool shouldShowUIBox(const UIBoxData& data, const MiniGameStateManagerData& managerData, const CombatManager& combatManager)
{
	CombatCharacter* pCombatCharacter = managerData.mStateData.getCharacter();
	const std::vector<EMiniGameState>& miniGameStateWhenToShowList = data.mMiniGameStateWhenToShowList;
	switch (data.mType)
	{
	case EUIBoxType_MINI_GAME_CHARACTER_BOX:
		if (data.mCombatCharacterIndex != -1 && !data.mShowDuringAllCharacters && data.mCombatCharacterIndex != managerData.mStateData.getCharacterIndex())
		{
			return false;
		}
		if (data.mCombatCharacterIndex != -1 && !combatManager.getFromAllCharacters(data.mCombatCharacterIndex)->isAlive() || data.mCombatCharacterIndex != -1 && !combatManager.getFromAllCharacters(data.mCombatCharacterIndex)->isStarted())
		{
			return false;
		}
		if (data.mCombatCharacterType != EMiniGameCombatCharacterType_CHARACTER && data.mCombatCharacterType != managerData.mStateData.mTargetCharacterType)
		{
			return false;
		}
		return std::find(miniGameStateWhenToShowList.begin(), miniGameStateWhenToShowList.end(), managerData.mCurStateEnum) != miniGameStateWhenToShowList.end();
	case EUIBoxType_MINI_GAME_PLAYER_BOX:
		if (pCombatCharacter == nullptr or pCombatCharacter->mType != EMiniGameCombatCharacterType_PLAYER)
		{
			return false;
		}
		return std::find(miniGameStateWhenToShowList.begin(), miniGameStateWhenToShowList.end(), managerData.mCurStateEnum) != miniGameStateWhenToShowList.end();
	case EUIBoxType_MINI_GAME_PLAYER_ATTACK_BOX:
		if (data.mCombatCharacterIndex != -1 && !data.mShowDuringAllCharacters && data.mCombatCharacterIndex != managerData.mStateData.getCharacterIndex())
		{
			return false;
		}
		if (std::find(miniGameStateWhenToShowList.begin(), miniGameStateWhenToShowList.end(), managerData.mCurStateEnum) != miniGameStateWhenToShowList.end())
		{
			return data.mAttackCategory == managerData.mStateData.mAttackCategory;
		}
		return false;
	case EUIBoxType_MINI_GAME_BOX:
	case EUIBoxType_MINI_GAME_DIRECTION_BOX:
		return std::find(miniGameStateWhenToShowList.begin(), miniGameStateWhenToShowList.end(), managerData.mCurStateEnum) != miniGameStateWhenToShowList.end();
	default:
		break;
	}

	return true;
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
		pCharacter = worldData.getStage()->mCombatManager.getFromAllCharacters(textBox.mData.mCombatCharacterIndex);
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
		return returnDescriptionOfMoveAttackType(pCharacter->mCombatMovementManager.getMoveType(), pCharacter->mCombatMovementManager.getMoveNum(), pCharacter->mCombatMovementManager.getMoveOut());
	case ECharacterStatBoxValueToDisplay_INVALID:
		return textBox.mMessage;
	default:
		SDL_assert(false);
	}

	return "error";
}

std::string updateHealthStatBoxCurTextBoxMessage(const HealthBox& healthBox, const MiniGameWorldData& worldData) 
{ 
	switch (healthBox.mData.mCharacterStatToDisplay)
	{
	case ECharacterStatBoxValueToDisplay_CHARACTER_HEALTH:
		return std::to_string(worldData.getStage()->mCombatManager.getFromAllCharacters(healthBox.mData.mCombatCharacterIndex)->getCurHealth());
	case ECharacterStatBoxValueToDisplay_CHARACTER_DEFENSE:
		return std::to_string(worldData.getStage()->mCombatManager.getFromAllCharacters(healthBox.mData.mCombatCharacterIndex)->getCurDefense());
	default:
		SDL_assert(false);
		break;
	}
	return "invalid";
	
}

float updateHealthStatBoxCurTextBoxRatio(const HealthBox& healthBox, const MiniGameWorldData& worldData)
{
	switch (healthBox.mData.mCharacterStatToDisplay)
	{
	case ECharacterStatBoxValueToDisplay_CHARACTER_HEALTH:
		return worldData.getStage()->mCombatManager.getFromAllCharacters(healthBox.mData.mCombatCharacterIndex)->getHealthRatio();
	case ECharacterStatBoxValueToDisplay_CHARACTER_DEFENSE:
		return worldData.getStage()->mCombatManager.getFromAllCharacters(healthBox.mData.mCombatCharacterIndex)->getDefenseRatio();
	default:
		SDL_assert(false);
		break;
	}
	return -1.0f;
}


void drawCircle(const SDL_Color& color, const Vect2& center, int radius, const ScreenObject& screen)
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

void printBlock(const ScreenObject& screen, const UIBlock& block) 
{
	SDL_Renderer* pRenderer = screen.mpRenderer;
	SDL_Color curBackgroundColor = block.mBackgroundColor;
	if (curBackgroundColor.a != 0)
	{
		SDL_SetRenderDrawColor(pRenderer, curBackgroundColor.r, curBackgroundColor.g, curBackgroundColor.b, curBackgroundColor.a);

		const Hitbox& box = block.mHitbox;
		SDL_FRect rect = { box.getTopLeft().getX() * screen.mGameScreenToGameLevelChunkRatio,
							box.getTopLeft().getY() * screen.mGameScreenToGameLevelChunkRatio,
							box.getWidth() * screen.mGameScreenToGameLevelChunkRatio,
							box.getHeight() * screen.mGameScreenToGameLevelChunkRatio };
		SDL_RenderFillRect(pRenderer, &rect);
	}
	pRenderer = nullptr;
}

void printTextBox(const ScreenObject& screen, const TextBox& textBox)
{
	SDL_Renderer* pRenderer = screen.mpRenderer;
	SDL_Color curTextBoxColor = textBox.getTextBoxColor();
	SDL_Color curOutlineColor = textBox.getIsHighlighted() ? textBox.mHighlightedOutlineColor : textBox.mOutlineColor;

	const Hitbox& hitbox = *textBox.mpCurHitbox;
	// outline box
	if (textBox.mOutlineWidth != 0 && curOutlineColor.a != 0)
	{
		SDL_SetRenderDrawColor(pRenderer, curOutlineColor.r, curOutlineColor.g, curOutlineColor.b, curOutlineColor.a);
		SDL_FRect rect = { hitbox.getTopLeft().getX() * screen.mGameScreenToGameLevelChunkRatio ,
							 hitbox.getTopLeft().getY() * screen.mGameScreenToGameLevelChunkRatio,
							 hitbox.getWidth() * screen.mGameScreenToGameLevelChunkRatio,
							 hitbox.getHeight() * screen.mGameScreenToGameLevelChunkRatio };
		SDL_RenderFillRect(pRenderer, &rect);
	}

	// background box
	if (curTextBoxColor.a != 0)
	{
		SDL_SetRenderDrawColor(pRenderer, curTextBoxColor.r, curTextBoxColor.g, curTextBoxColor.b, curTextBoxColor.a);
		SDL_FRect rect = { (hitbox.getTopLeft().getX() + textBox.mOutlineWidth) * screen.mGameScreenToGameLevelChunkRatio ,
							(hitbox.getTopLeft().getY() + textBox.mOutlineWidth) * screen.mGameScreenToGameLevelChunkRatio,
							(hitbox.getWidth() - 2 * textBox.mOutlineWidth) * screen.mGameScreenToGameLevelChunkRatio,
							(hitbox.getHeight() - 2 * textBox.mOutlineWidth) * screen.mGameScreenToGameLevelChunkRatio };
		SDL_RenderFillRect(pRenderer, &rect);
	}

	// text
	for (int i = 0; i < (*(textBox.mpCurTextures)).size(); i++)
	{
		const Hitbox& curLineBox = (*(textBox.mpCurLineHitboxes))[i];
		SDL_FRect destinationText = { curLineBox.getTopLeft().getX() * screen.mGameScreenToGameLevelChunkRatio ,
										curLineBox.getTopLeft().getY() * screen.mGameScreenToGameLevelChunkRatio,
										curLineBox.getWidth() * screen.mGameScreenToGameLevelChunkRatio,
										curLineBox.getHeight() * screen.mGameScreenToGameLevelChunkRatio };
		SDL_RenderTexture(pRenderer, (*(textBox.mpCurTextures))[i], NULL, &destinationText);
	}
	pRenderer = nullptr;
}

void printImageBox(const ScreenObject& screen, const ImageBox& imageBox)
{
	SDL_Renderer* pRenderer = screen.mpRenderer;
	const Hitbox& box = *imageBox.mpCurHitbox;
	SDL_FRect rect = { box.getTopLeft().getX() * screen.mGameScreenToGameLevelChunkRatio ,
						box.getTopLeft().getY() * screen.mGameScreenToGameLevelChunkRatio,
						box.getWidth() * screen.mGameScreenToGameLevelChunkRatio,
						box.getHeight() * screen.mGameScreenToGameLevelChunkRatio };
	int rotation = imageBox.mRotation;

	SDL_RenderTextureRotated(pRenderer, imageBox.mImageObject.getTexture(), NULL, &rect, rotation, NULL, SDL_FLIP_NONE);
}

void printShapeBox(const ScreenObject& screen, const ShapeBox& shapeBox)
{
	SDL_Renderer* pRenderer = screen.mpRenderer;
	if (shapeBox.mColor.a != 0)
	{
		SDL_FRect rect;
		const Hitbox& hitbox = *shapeBox.mpCurHitbox;
		SDL_SetRenderDrawColor(pRenderer, shapeBox.mColor.r, shapeBox.mColor.g, shapeBox.mColor.b, shapeBox.mColor.a);
		switch (shapeBox.mShapeType)
		{
		case EShapeBoxClass_CIRCLE:
			drawCircle(shapeBox.mColor, hitbox.getCenter(), hitbox.getWidth() / 2, screen);
			break;
		case EShapeBoxClass_RECT:
			rect = { hitbox.getTopLeft().getX() * screen.mGameScreenToGameLevelChunkRatio ,
						hitbox.getTopLeft().getY() * screen.mGameScreenToGameLevelChunkRatio,
						hitbox.getWidth() * screen.mGameScreenToGameLevelChunkRatio,
						hitbox.getHeight() * screen.mGameScreenToGameLevelChunkRatio };
			SDL_RenderFillRect(pRenderer, &rect);
			break;
		default:
			SDL_assert(false);
		}
	}
}

void printHealthBox(const ScreenObject& screen, const HealthBox& healthBox)
{
	printShapeBox(screen, healthBox.mBoundingBox);
	printShapeBox(screen, healthBox.mHealthLeftBox);
	printTextBox(screen, healthBox.mHealthText);
}