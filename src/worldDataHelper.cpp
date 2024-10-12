#include "worldDataHelper.h"

#include <corecrt_math_defines.h>
#define degToRad(angleDegrees) ((float)(angleDegrees) * M_PI / 180.0f)
#define radToDeg(angleRadians) ((float)(angleRadians) * 180.0f / M_PI)


WorldData::WorldData(ScreenObject& screenObject) : mScreen(screenObject){;}

WorldData::~WorldData()
{
	for (World* world : mpWorlds)
	{
		for (Level* level : world->mpLevels)
		{
			delete level;
		}
		delete world;
	}

	for (Projectile* projectile : mpProjectiles)
	{
		delete projectile;
	}
}


void WorldData::entityPreTickUpdateMovement(SlashManager& slashManager)
{
	//PLAYER
	Level* curLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];

	mPlayer.preTick();
	mPlayer.tick();

	curLevel->removeInactiveEntitiesFromActiveEntitiesVector();

	//REGULAR PLATFORMS
	for (Platform* curPlatform : curLevel->mpPlatforms)
	{
		curPlatform->preTick();
	}

	//NON STATIC PLATFORMS
	for (Platform* curPlatform : curLevel->mpActiveNonStaticPlatforms)
	{
		curPlatform->preTick();
		curPlatform->tick();
	}

	//ENEMIES
	for (Enemy* curEnemy : curLevel->mpActiveEnemies)
	{
		curEnemy->preTick();
		if (curEnemy->shouldShootProjectile())
		{
			enemyShootProjectile(curEnemy);
		}
		curEnemy->tick();
	}

	//PROJECTILES
	for (Projectile* curProjectile : mpProjectiles)
	{
		curProjectile->preTick();
		curProjectile->tick();
	}

	slashManager.tick();
}

void WorldData::createLevelChunk()
{
	int levelChunkX1 = 0;
	int levelChunkY1 = (mScreen.mGameLevelChunkHeight / 2); // half way through screen

	mCurLevelChunk = (CoordsX1Y1WidthHeight(levelChunkX1, levelChunkY1, mScreen.mGameLevelChunkWidth, mScreen.mGameLevelChunkHeight));
}

void WorldData::updateCurLevelChunk()
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	MovementManager& movementManager = mPlayer.getMovementManager();
	int playerX = movementManager.getHitbox().getTopLeft().getX();
	int playerY = movementManager.getHitbox().getTopLeft().getY();
	int levelChunkCenterX = mCurLevelChunk.getHitbox().getTopLeft().getX() + (mCurLevelChunk.getHitbox().getWidth()   / 2);
	int levelChunkCenterY = mCurLevelChunk.getHitbox().getTopLeft().getY() + (mCurLevelChunk.getHitbox().getHeight()  / 2);

	int distanceFromPlayer = getDistanceBetweenPoints(movementManager.getHitbox().getTopLeft(), mCurLevelChunk.getHitbox().getTopLeft());

	//move towards player
	levelChunkCenterX -= int((levelChunkCenterX - playerX) / 4);
	levelChunkCenterY -= int((levelChunkCenterY - playerY) / 4);


	int levelChunkX1 = levelChunkCenterX - (mCurLevelChunk.getHitbox().getWidth()  / 2);
	int levelChunkY1 = levelChunkCenterY - (mCurLevelChunk.getHitbox().getHeight() / 2);


	if (levelChunkX1 < 0)
	{
		levelChunkX1 = 0;
	}
	if (levelChunkX1 + mScreen.mGameLevelChunkWidth > pCurLevel->mLevelX2)
	{
		levelChunkX1 = pCurLevel->mLevelX2 - mScreen.mGameLevelChunkWidth;
	}
	if (levelChunkY1 < 0)
	{
		levelChunkY1 = 0;
	}
	if (levelChunkY1 + mScreen.mGameLevelChunkHeight > pCurLevel->mLevelY2)
	{
		levelChunkY1 = pCurLevel->mLevelY2 - mScreen.mGameLevelChunkHeight;
	}

	mCurLevelChunk.updateCoords(Vect2(levelChunkX1, levelChunkY1));
}

void WorldData::updateBackgroundEffects()
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	for (CircleEffect* pCurBackgroundEffect : pCurLevel->mpBackgroundEffects)
	{	
		pCurBackgroundEffect->move();
	}
}

void WorldData::entityCollisions(CollisionManager& collisionManager, DamageManager& damageManager, SlashManager& slashManager, KeyboardData& keyboardData)
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];

	updatePlayerCollisions(collisionManager, damageManager);
	if (keyboardData.mKeyState[int(EKeyboardInput_UP)] == true and keyboardData.mLastFrameKeyState[int(EKeyboardInput_UP)] == false)
	{
		if (canPlayerWallJump())
		{
			mPlayer.getMovementManager().startWallJump();
		}
	}
	updateCurLevelChunk();

	//NON STATIC PLATFORMS
	for (Platform* curPlatform : pCurLevel->mpActiveNonStaticPlatforms)
	{
		collideWithWorld(curPlatform);
		collideWithPlatforms(		  collisionManager, damageManager, curPlatform);
		collideWithNonStaticPlatforms(collisionManager, damageManager, curPlatform);
		collideWithEnemies(			  collisionManager, damageManager, curPlatform);
	}

	//ENEMIES
	for (Enemy* curEnemy : pCurLevel->mpActiveEnemies)
	{	
		collideWithWorld(curEnemy);
		collideWithPlatforms(			collisionManager, damageManager, curEnemy);
		checkIfOnEdgeOfPlatform(		collisionManager,				 curEnemy);
		collideWithNonStaticPlatforms(	collisionManager, damageManager, curEnemy);
		collideWithEnemies(				collisionManager, damageManager, curEnemy);
	}

	//PROJECTILES
	for (Projectile* curProjectile : mpProjectiles)
	{
		collideWithWorld(curProjectile);
		collideWithPlatforms(			collisionManager, damageManager, curProjectile);
		collideWithNonStaticPlatforms(	collisionManager, damageManager, curProjectile);
		collideWithEnemies(				collisionManager, damageManager, curProjectile);
	}

	//SLASH COLLISIONS
	if (slashManager.mCurSlash)
	{
		std::vector<EntityDistance> struckEntities;
		EntityDistance closestBlockingEntity;
		std::vector<Entity*> entities = pCurLevel->getAllActiveEntities();

		int hitboxX1 = std::min(mPlayer.getMovementManager().getHitbox().getTopLeft().getX(),     slashManager.mHitbox.getTopLeft().getX());
		int hitboxX2 = std::max(mPlayer.getMovementManager().getHitbox().getBottomRight().getX(), slashManager.mHitbox.getBottomRight().getX());
		int hitboxY1 = slashManager.mHitbox.getTopLeft().getY();
		int hitboxY2 = slashManager.mHitbox.getBottomRight().getY();
		Hitbox approxSlashHitbox = Hitbox(hitboxX1, hitboxX2, hitboxY1, hitboxY2);

		for (Entity* pEntity : entities)
		{	
			if (pEntity->getAmAlive() and pEntity->getMovementManager().getHitbox().overlap(approxSlashHitbox))
			{
				if (pEntity->mVulnerableToProjectiles)
				{
					struckEntities.push_back(EntityDistance(pEntity, &mPlayer, approxSlashHitbox, slashManager.mCurRotation));
				}
				else
				{
					EntityDistance curBlockingEntity = EntityDistance(pEntity, &mPlayer, approxSlashHitbox, slashManager.mCurRotation);
					if (curBlockingEntity.mDistance < closestBlockingEntity.mDistance)
					{
						closestBlockingEntity = curBlockingEntity;
					}
				}
			}
		}

		for (EntityDistance& curEntityDistance : struckEntities)
		{
			if (curEntityDistance.mDistance < closestBlockingEntity.mDistance)
			{
				//entity closer, will be hit
				bool instantDeath = true;
				killedCharacter(curEntityDistance.mpEntity, instantDeath);
			}
		}
	}
}

void WorldData::updatePlayerCollisions(CollisionManager& collisionManager, DamageManager& damageManager)
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	MovementManager & movementManager = mPlayer.getMovementManager();

	//WINDOW
	bool instantDeath = true;
	if (movementManager.getHitbox().getTopLeft().getX() <= 0)
	{
		movementManager.getHitbox().setTopLeftX(1);
	}
	else if (movementManager.getHitbox().getBottomRight().getX() >= pCurLevel->mLevelX2)
	{
		//CHANGE LEVEL
		movementManager.getHitbox().setTopLeftX(pCurLevel->mLevelX2 - movementManager.getHitbox().getWidth() - 1);
	}
	if (movementManager.getHitbox().getTopLeft().getY() <= 0)
	{
		movementManager.getHitbox().setTopLeftY(1);
	}
	else if (movementManager.getHitbox().getBottomRight().getY() >= pCurLevel->mLevelY2)
	{
		killedCharacter(&mPlayer, instantDeath);
	}

	movementManager.setOnGroundFalse();
	bool check = true;
	
	//PLATFORMS
	std::vector <Entity*> platformEntities;
	pCurLevel->mStaticEntities.getEntitiesInHitbox(platformEntities, movementManager.getHitbox());
	pCurLevel->mDynamicEntities.getEntitiesInHitbox(platformEntities, movementManager.getHitbox(), EEntityClassTypes_PLATFORM);

	for (Entity* pEntity : platformEntities)
	{
		Platform* pCurPlatform = (Platform*)pEntity;
		Hitbox& curPlatformHitbox = pCurPlatform->getMovementManager().getHitbox();
		if (pCurPlatform->mIsVisible == true)
		{
			bool fallenOnIt = false;
			bool doSeparate = false;
			EBoxSide playerSeparationPath   = curPlatformHitbox.separate(movementManager.getHitbox(), doSeparate);
			EBoxSide platformSeparationPath = movementManager.getHitbox().separate(curPlatformHitbox, doSeparate);

			if (platformSeparationPath == EBoxSide_TOP and (movementManager.getCurDirectionY() == EDirection_NONE or movementManager.getCurDirectionY() == EDirection_DOWN))
			{
				fallenOnIt = true;
			}

			EEntityEdgeType playerEdgeType   = movementManager.getEdgeType(playerSeparationPath);
			EEntityEdgeType platformEdgeType = pCurPlatform->getMovementManager().getEdgeType(platformSeparationPath);
			if		(platformEdgeType == EEntityEdgeType_NON_EXISTENT)
			{
				//PASS
			}
			else if (platformEdgeType == EEntityEdgeType_MOVEABLE)
			{
				collisionManager.mCollisionsToSeparate.push_back(Collision(&mPlayer, pCurPlatform));
				int xOverlap = rangeOverlapDistance(movementManager.getHitbox().getTopLeft().getX(), movementManager.getHitbox().getBottomRight().getX(), pCurPlatform->getMovementManager().getHitbox().getTopLeft().getX(), pCurPlatform->getMovementManager().getHitbox().getBottomRight().getX());
				if ((xOverlap > int(movementManager.getMovementVect2().getX() + movementManager.getMovementEffect()) / 2))
				{
					collisionManager.addCrateContact(pCurPlatform);
					int curMovement = movementManager.getMovementVect2().getX() + movementManager.getMovementEffect();
					if (movementManager.getCurDirection() == EDirection_LEFT)
					{
						curMovement *= -1;
					}
					collisionManager.setCrateContactMovementIncrement(curMovement);
				}
				doSeparate = true;
				platformSeparationPath = movementManager.getHitbox().separate(pCurPlatform->getMovementManager().getHitbox(), doSeparate);
			}
			else
			{
				doSeparate = true;
				platformSeparationPath = movementManager.getHitbox().separate(curPlatformHitbox, doSeparate);

				if (damageManager.willKillCharacter(&mPlayer, platformEdgeType))
				{
					killedCharacter(&mPlayer, !instantDeath);
				}

				damageManager.spreadEdges(pCurPlatform, &mPlayer);

				//check for special cases
				if (platformEdgeType == EEntityEdgeType_TELEPORTER)
				{
					//TODO TELEPORT
					return;
				}

			}

			if (pCurPlatform->getCharacterType() == EEntityCharacterTypes_P_GATE and pCurPlatform->mIsVisible == true)
			{
				if (mPlayer.mKeys > 0)
				{
					pCurPlatform->hide();
					mPlayer.updateKeys(-1);
				}
			}
			else if (pCurPlatform->getCharacterType() == EEntityCharacterTypes_P_TARGET_GATE and pCurPlatform->mIsVisible == true)
			{
				if (mPlayer.mTargets > 0)
				{
					pCurPlatform->hide();
					mPlayer.updateTargets(-1);
				}
			}

			if (fallenOnIt == true)
			{
				//curPlatform.fallenOn();
				movementManager.setOnGroundTrue(pCurPlatform->getMovementEffect(), pCurPlatform->getCurCharacteristics(), pCurPlatform->getMovementManager().getHitboxEdges().mTop);
				movementManager.getMovementStates()[EMovementStateIndex_JUMPING]->landed();
				((JumpingState*)movementManager.getMovementStates()[EMovementStateIndex_JUMPING])->endJump();

				playerEdgeType   = movementManager.getHitboxEdges().mBottom;
				platformEdgeType = pCurPlatform->getMovementManager().getHitboxEdges().mTop;

				if (damageManager.willKillCharacter(pCurPlatform, playerEdgeType))
				{
					killedCharacter(pCurPlatform, !instantDeath);
				}
				if (platformEdgeType == EEntityEdgeType_BOUNCY)
				{
					movementManager.collideWithBouncy();
				} 
				else if (platformEdgeType == EEntityEdgeType_CRUMBLING)
				{
					pCurPlatform->startCrumble();
				}
				else if (pCurPlatform->getType() == EEntityType_NON_STATIC)
				{
					collisionManager.addRidingContact(&mPlayer, pCurPlatform);
				}
			}
			if (pCurPlatform->getSubClassType() == EEntitySubClassTypes_AREA_EFFECT)
			{
				AreaEffectPlatform * pCurAreaEffectPlatform = (AreaEffectPlatform*) pCurPlatform;
				if (movementManager.getHitbox().overlap(pCurAreaEffectPlatform->mAreaEffectHitbox))
				{
					movementManager.push(pCurAreaEffectPlatform->mAreaEffectMovement, pCurAreaEffectPlatform->mEffectDirection);
				}
			}
		}
	}

	//ENEMIES
	std::vector <Entity*> enemyEntities;
	pCurLevel->mDynamicEntities.getEntitiesInHitbox(enemyEntities, movementManager.getHitbox(), EEntityClassTypes_ENEMY);
	for (Entity* pEntity : enemyEntities)
	{
		if (check)
		{
			Enemy* pCurEnemy = (Enemy*)pEntity;
			
			bool overlap = movementManager.getHitbox().overlap(pCurEnemy->getMovementManager().getHitbox());
			if (overlap)
			{
				bool fallenOnIt = false;
				bool doSeparate = false;
				EBoxSide playerSeparationPath = pCurEnemy->getMovementManager().getHitbox().separate(movementManager.getHitbox(), doSeparate);
				doSeparate = true;
				EBoxSide enemySeparationPath  = movementManager.getHitbox().separate(pCurEnemy->getMovementManager().getHitbox(), doSeparate);

				if (enemySeparationPath == EBoxSide_TOP)
				{
					fallenOnIt = true;
				}

				EEntityEdgeType enemyEdgeType = pCurEnemy->getMovementManager().getEdgeType(enemySeparationPath);
				if (damageManager.willKillCharacter(&mPlayer, enemyEdgeType))
				{
					killedCharacter(&mPlayer, !instantDeath);
				}
				if (enemyEdgeType == EEntityEdgeType_GRABBING)
				{
					EDirection curDirection = EDirection_RIGHT;
					if (movementManager.getHitbox().getCenter().getX() < pCurEnemy->getMovementManager().getHitbox().getCenter().getX())
					{
						curDirection = EDirection_LEFT;
					}

					if (pCurEnemy->mHasAttachmentPoint and pCurEnemy->mRideable and pCurEnemy->getAmAlive() and curDirection == pCurEnemy->getMovementManager().getCurFacingDirection())
					{
						pCurEnemy->setNextAnimationToPlay(EAnimationType_GRABBING);
						movementManager.setCurFacingDirection(pCurEnemy->getMovementManager().getCurDirection());
						int x = pCurEnemy->getMovementManager().getHitbox().getTopLeft().getX() + pCurEnemy->mAttachmentPoint.getX();
						int y = pCurEnemy->getMovementManager().getHitbox().getTopLeft().getY() + pCurEnemy->mAttachmentPoint.getY() - movementManager.getHitbox().getHeight();
						movementManager.getHitbox().setTopLeft(Vect2(x,y));
						collisionManager.addRidingContact(&mPlayer, pCurEnemy);
						check = false;
					}
				}

				if (fallenOnIt and check)
				{
					movementManager.setOnGroundTrue(pCurEnemy->getMovementEffect(), pCurEnemy->getCurCharacteristics(), pCurEnemy->getMovementManager().getHitboxEdges().mTop);
					movementManager.getMovementStates()[EMovementStateIndex_JUMPING]->landed();
					((JumpingState*)movementManager.getMovementStates()[EMovementStateIndex_JUMPING])->endJump();

					EEntityEdgeType playerEdgeType = movementManager.getHitboxEdges().mBottom;

					if (damageManager.willKillCharacter(pCurEnemy, playerEdgeType) and mPlayer.getAmAlive() == true and mPlayer.takingDamage() == false)
					{
						killedCharacter(pCurEnemy, !instantDeath);
					}

					//SPECIAL CASES
					if (enemyEdgeType == EEntityEdgeType_BOUNCY)
					{
						if (movementManager.isAmJump() == false)
						{
							movementManager.collideWithBouncy();
						}
					}
					else if (pCurEnemy->mRideable and (pCurEnemy->getAmAlive() == true))
					{
						collisionManager.addRidingContact(&mPlayer, pCurEnemy);
					}
				}
				else if (check)
				{
					collisionManager.entitiesCollidedHorizontal(pCurEnemy, &mPlayer);
				}
			}
		}
	}

	//COLLECTIBLES
	std::vector <Entity*> collectibleEntities;
	pCurLevel->mDynamicEntities.getEntitiesInHitbox(collectibleEntities, movementManager.getHitbox(), EEntityClassTypes_COLLECTIBLE);
	for (Entity* pEntity : collectibleEntities)
	{
		Collectible* pCurCollectible = (Collectible*)pEntity;
		if (pCurCollectible->getMovementManager().getHitbox().overlap(movementManager.getHitbox()) and pCurCollectible->isAmPickedUp() == false)
		{
			collectedCollectible(pCurCollectible);
		}
	}

	//PROJECTILES
	for (int count = 0; count < mpProjectiles.size(); count++)
	{
		Projectile* pCurProjectile = mpProjectiles[count];
		if (pCurProjectile->getCharacterType() == EEntityCharacterTypes_PJ_ENEMY_PROJECTILE)
		{
			if (mPlayer.getAmAlive() and movementManager.getHitbox().overlap(pCurProjectile->getMovementManager().getHitbox()))
			{
				bool instantDeath = true;
				killedCharacter(&mPlayer, pCurProjectile);
				killedCharacter(pCurProjectile, &mPlayer);
			}
		}
	}

}

bool WorldData::canPlayerWallJump()
{
	Vect2 movementVect = mPlayer.getMovementManager().getMovementVect2();
	Hitbox& hitbox = mPlayer.getMovementManager().getHitbox();
	Hitbox IncreasedPlayerHitbox = Hitbox(hitbox.getTopLeft() - movementVect, hitbox.getWidth() + movementVect.getX(), hitbox.getHeight() + movementVect.getY());
	
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	std::vector <Entity*> platformEntities;
	pCurLevel->mStaticEntities.getEntitiesInHitbox(platformEntities, IncreasedPlayerHitbox);
	pCurLevel->mDynamicEntities.getEntitiesInHitbox(platformEntities, IncreasedPlayerHitbox, EEntityClassTypes_PLATFORM);

	for (Entity* curEntity : platformEntities)
	{
		Platform* curPlatform = (Platform*)curEntity;
		Hitbox& curPlatformHitbox = curPlatform->getMovementManager().getHitbox();

		bool doSeparate = false;
		EBoxSide separationPath = mPlayer.getMovementManager().getHitbox().separate(curPlatformHitbox, doSeparate);
		EEntityEdgeType edgeType = curPlatform->getMovementManager().getEdgeType(separationPath);

		if ((edgeType == EEntityEdgeType_WALL_JUMPABLE) and ((separationPath == EBoxSide_LEFT ) or (separationPath == EBoxSide_RIGHT)))
		{
			if (mPlayer.getMovementManager().isOnGround() == false)
			{
				doSeparate = true;
				mPlayer.getMovementManager().getHitbox().separate(curPlatformHitbox, doSeparate);
				return true;
			}
		}
	}
	return false;
}

void WorldData::collideWithWorld(Entity* pCurEntity)
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	Hitbox& curHitbox = pCurEntity->getMovementManager().getHitbox();
	bool instantDeath = true;
	bool isHorizontalPath = pCurEntity->getMovementManager().getPath() == EEntityMovementPath_HORIZONTAL or pCurEntity->getMovementManager().getPath() == EEntityMovementPath_HORIZONTAL_CAN_FALL;
	if (curHitbox.getTopLeft().getX() <= 0)
	{
		int updatedX = pCurEntity->getMovementManager().getMovementVect2().getX();
		curHitbox.setTopLeftX(updatedX);
		if (isHorizontalPath)
		{
			pCurEntity->getMovementManager().collided(EDirection_LEFT);
			if (pCurEntity->getMovementManager().getSwitchedDir())
			{
				pCurEntity->isPathBlocked();
			}
		}
		if (pCurEntity->getClassType() == EEntityClassTypes_PROJECTILE)
		{
			killedCharacter(pCurEntity, instantDeath);
		}
	}
	else if (curHitbox.getBottomRight().getX() >= pCurLevel->mLevelX2)
	{
		curHitbox.setTopLeftX(pCurLevel->mLevelX2 - curHitbox.getWidth() - pCurEntity->getMovementManager().getMovementVect2().getX());
		if (isHorizontalPath)
		{
			pCurEntity->getMovementManager().collided(EDirection_RIGHT);
			if (pCurEntity->getMovementManager().getSwitchedDir())
			{
				pCurEntity->isPathBlocked();
			}
		}
		if (pCurEntity->getClassType() == EEntityClassTypes_PROJECTILE)
		{
			killedCharacter(pCurEntity, instantDeath);
		}
	}

	bool isVerticalPath = pCurEntity->getMovementManager().getPath() == EEntityMovementPath_VERTICAL;
	if (curHitbox.getTopLeft().getY() <= 0)
	{
		curHitbox.setTopLeftY(pCurEntity->getMovementManager().getMovementVect2().getY());
		if (isVerticalPath)
		{
			pCurEntity->getMovementManager().collided(EDirection_UP);

		}
		if (pCurEntity->getClassType() == EEntityClassTypes_PROJECTILE)
		{
			killedCharacter(pCurEntity, instantDeath);
		}
	}
	else if (curHitbox.getBottomRight().getY() >= pCurLevel->mLevelY2)
	{
		curHitbox.setTopLeftY(pCurLevel->mLevelY2 - curHitbox.getHeight() - pCurEntity->getMovementManager().getMovementVect2().getY());
		if (isVerticalPath)
		{
			pCurEntity->getMovementManager().collided(EDirection_DOWN);
		}
		else
		{
			//kill character
			curHitbox.setTopLeftY(pCurEntity->getMovementManager().getMovementVect2().getY());
			if (pCurEntity->getClassType() == EEntityClassTypes_PROJECTILE)
			{
				killedCharacter(pCurEntity, instantDeath);
			}
		}
	}

}

void WorldData::collideWithPlatforms(CollisionManager& collisionManager, DamageManager& damageManager, Entity* pCurEntity)
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	MovementManager& movementManager = pCurEntity->getMovementManager();
	bool instantDeath = true;

	movementManager.setOnGroundFalse();

	std::vector <Entity*> platformEntities;
	pCurLevel->mStaticEntities.getEntitiesInHitbox(platformEntities, movementManager.getHitbox());

	for (Entity* pEntity : platformEntities)
	{
		bool check = true;
		Platform& curPlatform = *(Platform*)pEntity;
		Hitbox& curPlatformHitbox = curPlatform.getMovementManager().getHitbox();

		if (curPlatform.mIsVisible == false or pCurEntity == pEntity)
		{
			check = false;
		}

		if (check)
		{
			if (pCurEntity->getClassType() == EEntityClassTypes_PROJECTILE)
			{
				killedCharacter(pCurEntity, instantDeath);
				if (curPlatform.getCharacterType() == EEntityCharacterTypes_P_TARGET)
				{
					if (pCurEntity->getCharacterType() == EEntityCharacterTypes_PJ_PLAYER_PROJECTILE)
					{
						curPlatform.hide();
						mPlayer.updateTargets(1);
					}
				}
				return;
			}
			int doSeparate = false;
			EBoxSide platformSeparationPath  = movementManager.getHitbox().separate(curPlatformHitbox, doSeparate);
			EBoxSide entitySeparationPath    = curPlatformHitbox.separate(movementManager.getHitbox(), doSeparate);
			EEntityEdgeType platformEdgeType = curPlatform.getMovementManager().getEdgeType(platformSeparationPath);
			EEntityEdgeType entityEdgeType   = movementManager.getEdgeType(entitySeparationPath);

			if (platformEdgeType == EEntityEdgeType_NON_EXISTENT and check)
			{
				if (pCurEntity->getMovementManager().getPath() == EEntityMovementPath_VERTICAL or pCurEntity->getMovementManager().getPath() == EEntityMovementPath_DIAGONAL)
				{

				}
				else
				{
					check = false;
				}
			}
			if (check)
			{
				doSeparate = true;
				platformSeparationPath = movementManager.getHitbox().separate(curPlatformHitbox, doSeparate);
				doSeparate = false;
				entitySeparationPath   = curPlatformHitbox.separate(movementManager.getHitbox(), doSeparate);
				platformEdgeType = curPlatform.getMovementManager().getEdgeType(platformSeparationPath);
				entityEdgeType = movementManager.getEdgeType(entitySeparationPath);

				damageManager.spreadEdges(&curPlatform, pCurEntity);

				if (damageManager.willKillCharacter(pCurEntity, platformEdgeType))
				{
					killedCharacter(pCurEntity, !instantDeath);
					check = false;
				}

				if (platformSeparationPath == EBoxSide_TOP and check)
				{
					if (movementManager.getPath() == EEntityMovementPath_HORIZONTAL or movementManager.getPath() == EEntityMovementPath_HORIZONTAL_CAN_FALL)
					{
						movementManager.setOnGroundTrue(curPlatform.getMovementEffect(), curPlatform.getCurCharacteristics(), curPlatform.getMovementManager().getHitboxEdges().mTop);
						//cur platform fallen on
						((JumpingState*)movementManager.getMovementStates()[EMovementStateIndex_JUMPING])->endJump();
						movementManager.getMovementStates()[EMovementStateIndex_JUMPING]->landed();

						if (platformEdgeType == EEntityEdgeType_BOUNCY)
						{
							pCurEntity->getMovementManager().collideWithBouncy();
						}
						else if (platformEdgeType == EEntityEdgeType_WEIGHT_SENSITIVE and pCurEntity->getCharacterType() == EEntityCharacterTypes_P_ARMORED_CRATE)
						{
							int codeNumber = curPlatform.mCodeNumber;
							curPlatform.activate();
							for (int countGate = 0; countGate < pCurLevel->mpPlatforms.size(); countGate++)
							{
								Platform& curPossibleGate = *(pCurLevel->mpPlatforms[countGate]);
								if (curPossibleGate.getCharacterType() == EEntityCharacterTypes_P_PRESSURE_OPERATED_GATE and curPossibleGate.mCodeNumber == codeNumber)
								{
									curPossibleGate.hide();
								}
							}

						}

					}
					else
					{
						collisionManager.entitiesCollidedVertical(pCurEntity, &curPlatform);
						check = false;
					}
				}
				else if (platformSeparationPath == EBoxSide_BOTTOM and check)
				{
					if (movementManager.getPath() == EEntityMovementPath_VERTICAL or movementManager.getPath() == EEntityMovementPath_DIAGONAL)
					{
						collisionManager.entitiesCollidedVertical(pCurEntity, &curPlatform);
					}
				}
				else if (check)
				{
					if (movementManager.getPath() == EEntityMovementPath_HORIZONTAL or movementManager.getPath() == EEntityMovementPath_HORIZONTAL_CAN_FALL or movementManager.getPath() == EEntityMovementPath_DIAGONAL)
					{
						collisionManager.entitiesCollidedHorizontal(pCurEntity, &curPlatform);
						check = false;
					}
				}
			}
		}
	}
}

void WorldData::collideWithNonStaticPlatforms(CollisionManager& collisionManager, DamageManager& damageManager, Entity* pCurEntity)
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	bool instantDeath = true;
	bool createdRidingContact = false;

	std::vector <Entity*> platformEntities;
	pCurLevel->mDynamicEntities.getEntitiesInHitbox(platformEntities, pCurEntity->getMovementManager().getHitbox(), EEntityClassTypes_PLATFORM);
	
	for (Entity* pEntity : platformEntities)
	{
		Platform* pCurNonStaticPlatform = (Platform*)pEntity;
		bool check = true;
		if (pCurEntity != pCurNonStaticPlatform)
		{
			if (pCurEntity->getClassType() == EEntityClassTypes_PROJECTILE)
			{
				if (damageManager.willProjectileKillCharacter(pCurEntity, pCurNonStaticPlatform))
				{
					killedCharacter(pCurNonStaticPlatform, instantDeath);
				}
				killedCharacter(pCurEntity, instantDeath);
				return;
			}
			else
			{
				bool doSeparate = false;
				EBoxSide separationPath  = pCurNonStaticPlatform->getMovementManager().getHitbox().separate(pCurEntity->getMovementManager().getHitbox(), doSeparate);
				EEntityEdgeType edgeType = pCurNonStaticPlatform->getMovementManager().getEdgeType(separationPath);

				damageManager.spreadEdges(pCurNonStaticPlatform, pCurEntity);
				if (damageManager.willKillCharacter(pCurEntity, edgeType))
				{
					killedCharacter(pCurEntity, !instantDeath);
					check = false;
				}

				if (check)
				{
					if (separationPath == EBoxSide_BOTTOM)
					{
						pCurEntity->getMovementManager().setOnGroundTrue(pCurNonStaticPlatform->getMovementEffect(), pCurNonStaticPlatform->getCurCharacteristics(), pCurNonStaticPlatform->getMovementManager().getHitboxEdges().mTop);
						pCurEntity->getMovementManager().getMovementStates()[EMovementStateIndex_JUMPING]->landed();
					}
					else if (separationPath == EBoxSide_LEFT or separationPath == EBoxSide_RIGHT)
					{
						if (pCurEntity->getMovementManager().getPath() == EEntityMovementPath_HORIZONTAL or pCurEntity->getMovementManager().getPath() == EEntityMovementPath_HORIZONTAL_CAN_FALL)
						{
							collisionManager.entitiesCollidedHorizontal(pCurEntity, pCurNonStaticPlatform);
							check = false;
						}
					}

					if (pCurEntity->getSubClassType() == EEntitySubClassTypes_CRATE)
					{
						doSeparate							= false;
						EBoxSide		crateSeparationPath = pCurEntity->getMovementManager().getHitbox().separate(pCurNonStaticPlatform->getMovementManager().getHitbox(), doSeparate);
						EEntityEdgeType crateEdgeType		= pCurEntity->getMovementManager().getEdgeType(crateSeparationPath);

						if (edgeType == EEntityEdgeType_MOVEABLE and crateEdgeType == EEntityEdgeType_MOVEABLE)
						{
							int movement = pCurEntity->getMovementManager().getMovementVect2().getX();
							if (pCurEntity->getMovementManager().getCurDirection() == EDirection_LEFT)
							{
								movement *= -1;
							}
							pCurNonStaticPlatform->getMovementManager().getHitbox().updateTopLeftX(movement);
							collisionManager.mCollisionsToSeparate.push_back(Collision(pCurEntity, pCurNonStaticPlatform));
							collisionManager.addCrateContact(pCurEntity);
							collisionManager.addCrateContact(pCurNonStaticPlatform);

							doSeparate = true;
							pCurNonStaticPlatform->getMovementManager().getHitbox().separate(pCurEntity->getMovementManager().getHitbox(), doSeparate);
						}
						else
						{
							if (separationPath == EBoxSide_BOTTOM )
							{
								//crate on crate or crate on Moving platform 
								createdRidingContact = true;
								collisionManager.addRidingContact(pCurEntity, pCurNonStaticPlatform);
							}
							else if ((pCurNonStaticPlatform->getSubClassType() == EEntitySubClassTypes_CRATE) and (separationPath == EBoxSide_TOP))
							{
								createdRidingContact = true;
								collisionManager.addRidingContact(pCurNonStaticPlatform, pCurEntity);
							}
							else
							{
								doSeparate = true;
								pCurEntity->getMovementManager().getHitbox().separate(pCurNonStaticPlatform->getMovementManager().getHitbox(), doSeparate);
							}
						}
					}
					else if (pCurEntity->getCharacterType() == EEntityCharacterTypes_P_MOVING_PLATFORM)
					{
						if (pCurNonStaticPlatform->getSubClassType() == EEntitySubClassTypes_CRATE)
						{
							if (separationPath == EBoxSide_TOP )
							{
								//crate on crate or crate on Moving platform
								createdRidingContact = true;
								collisionManager.addRidingContact(pCurNonStaticPlatform, pCurEntity);
							}
						}
					}
					else if (pCurEntity->getClassType() == EEntityClassTypes_ENEMY)
					{
						if (separationPath == EBoxSide_TOP or separationPath == EBoxSide_BOTTOM)
						{
							//pass
						}
						else
						{
							collisionManager.entitiesCollidedHorizontal(pCurEntity, pCurNonStaticPlatform);
							return;
						}
					}
					if (createdRidingContact == false)
					{
						bool doSeparate = true;
						EBoxSide separationPath  = pCurNonStaticPlatform->getMovementManager().getHitbox().separate(pCurEntity->getMovementManager().getHitbox(), doSeparate);
					}
					if ((separationPath == EBoxSide_TOP or separationPath == EBoxSide_BOTTOM) and createdRidingContact == false)
					{
						collisionManager.entitiesCollidedVertical(pCurEntity, pCurNonStaticPlatform);
						return;
					}
				}
			}
		}
	}
}

void WorldData::collideWithEnemies(CollisionManager& collisionManager, DamageManager& damageManager, Entity* pCurEntity)
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	bool instantDeath = true;

	std::vector <Entity*> enemiesEntities;
	pCurLevel->mDynamicEntities.getEntitiesInHitbox(enemiesEntities, pCurEntity->getMovementManager().getHitbox(), EEntityClassTypes_ENEMY);


	for (Entity* pEntity : enemiesEntities)
	{
		Enemy* pCurEnemy = (Enemy*)pEntity;
		bool check = true;
		if (pCurEntity != pCurEnemy)
		{
			if (pCurEntity->getClassType() == EEntityClassTypes_PROJECTILE)
			{
				Projectile* pCurProjectile = (Projectile*) pCurEntity;
				if (pCurEnemy->takeDamageFromProjectile(pCurEntity->getHostName()))
				{
					killedCharacter(pCurEnemy,  instantDeath);
					killedCharacter(pCurEntity, instantDeath);
					return;
				}
			}
			else
			{
				bool doSeparate = false;

				EBoxSide separationPath = pCurEntity->getMovementManager().getHitbox().separate(pCurEnemy->getMovementManager().getHitbox(), doSeparate);
				if (separationPath == EBoxSide_TOP)
				{
					pCurEntity->getMovementManager().setOnGroundTrue(pCurEnemy->getMovementEffect(), pCurEnemy->getCurCharacteristics(), pCurEnemy->getMovementManager().getHitboxEdges().mTop);
					pCurEntity->getMovementManager().getMovementStates()[EMovementStateIndex_JUMPING]->landed();
					//TODO SET CUR GROUND???
				}

				if (pCurEntity->getSubClassType() == EEntitySubClassTypes_CRATE)
				{
					if (separationPath == EBoxSide_TOP)
					{
						//CRATE ON ENEMY
						EEntityEdgeType crateEdgeType = pCurEntity->getMovementManager().getHitboxEdges().mBottom;
						EEntityEdgeType enemyEdgeType = pCurEnemy->getMovementManager().getHitboxEdges().mTop;

						if (damageManager.willKillCharacter(pCurEnemy, crateEdgeType))
						{
							killedCharacter(pCurEnemy, !instantDeath);
							check = false;
						}
						if (damageManager.willKillCharacter(pCurEntity, enemyEdgeType))
						{
							killedCharacter(pCurEntity, !instantDeath);
							check = false;
						}

						if (pCurEnemy->mRideable)
						{
							collisionManager.addRidingContact(pCurEntity, pCurEnemy);
							check = false;
						}
						else if (enemyEdgeType == EEntityEdgeType_BOUNCY)
						{
							pCurEntity->getMovementManager().collideWithBouncy();
							killedCharacter(pCurEnemy, instantDeath);
						}
					}
					else if (separationPath == EBoxSide_BOTTOM)
					{
						//ENEMY ON CRATE
						collisionManager.addRidingContact(pCurEnemy, pCurEntity);
					}
					else
					{
						//ENEMY ON SIDE OF CRATE
						EEntityEdgeType enemyEdgeType = pCurEnemy->getMovementManager().getEdgeType(separationPath);
						if (enemyEdgeType == EEntityEdgeType_GRABBING)
						{
							if (pCurEnemy->mHasAttachmentPoint and pCurEnemy->mRideable and pCurEnemy->getAmAlive())
							{
								EDirection crateDirection = EDirection_RIGHT;
								if (pCurEntity->getMovementManager().getHitbox().getCenter().getX() < pCurEnemy->getMovementManager().getHitbox().getCenter().getX())
								{
									crateDirection = EDirection_LEFT;
								}

								if (crateDirection == pCurEnemy->getMovementManager().getCurFacingDirection())
								{
									int x = pCurEnemy->getMovementManager().getHitbox().getTopLeft().getX();
									if (pCurEnemy->getMovementManager().getCurDirection() == EDirection_LEFT)
									{
										x += pCurEnemy->mAttachmentPoint.getX();
									}
									else
									{
										x -= pCurEnemy->mAttachmentPoint.getX();
									}
									int y = pCurEnemy->getMovementManager().getHitbox().getTopLeft().getY() + pCurEnemy->mAttachmentPoint.getY() - pCurEntity->getMovementManager().getHitbox().getHeight();

									pCurEnemy->setNextAnimationToPlay(EAnimationType_GRABBING);
									pCurEntity->getMovementManager().getHitbox().setTopLeft(Vect2(x,y));
									collisionManager.addRidingContact(pCurEntity, pCurEnemy);
								}
							}
						}
						else
						{
							collisionManager.entitiesCollidedHorizontal(pCurEntity, pCurEnemy);
						}
						check = false;
					}
				}
				else if (pCurEntity->getClassType() == EEntityClassTypes_ENEMY)
				{
					doSeparate = false;
					separationPath = pCurEntity->getMovementManager().getHitbox().separate(pCurEnemy->getMovementManager().getHitbox(), doSeparate);


					if (separationPath == EBoxSide_TOP)
					{
						if (pCurEntity->getMovementManager().getPath() == EEntityMovementPath_HORIZONTAL)
						{
							//pass
						}
						else
						{
							collisionManager.entitiesCollidedVertical(pCurEnemy, pCurEntity);
							check = false;
						}
					}
					else
					{
						doSeparate = true;
						separationPath = pCurEntity->getMovementManager().getHitbox().separate(pCurEnemy->getMovementManager().getHitbox(), doSeparate);
						collisionManager.entitiesCollidedHorizontal(pCurEntity, pCurEnemy);
						check = false;
					}
				}
				else
				{
					collisionManager.entitiesCollidedHorizontal(pCurEntity, pCurEnemy);
					check = false;
				}
			}
		}
	}
}

void WorldData::checkIfOnEdgeOfPlatform(CollisionManager& collisionManager, Enemy* pCurEnemy)
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	if (pCurEnemy->getMovementManager().getPath() == EEntityMovementPath_HORIZONTAL or pCurEnemy->getMovementManager().getPath() == EEntityMovementPath_VERTICAL)
	{
		return;
	}
	if (pCurEnemy->getMovementManager().getCode() == EEntityMovements_JUMP)
	{
		return;
	}

	std::vector <Entity*> platformEntities;
	pCurLevel->mStaticEntities.getEntitiesInHitbox(platformEntities, pCurEnemy->getMovementManager().getHitbox());
	pCurLevel->mDynamicEntities.getEntitiesInHitbox(platformEntities, pCurEnemy->getMovementManager().getHitbox(), EEntityClassTypes_PLATFORM);

	for (Entity* pEntity : platformEntities)
	{
		Platform& curPlatform = *(Platform*)pEntity;
		Hitbox& curPlatformHitbox = curPlatform.getMovementManager().getHitbox();

		int curEnemyY2    = pCurEnemy->getMovementManager().getHitbox().getBottomRight().getY();
		int curPlatformY1 = curPlatformHitbox.getTopLeft().getY();
		//make sure the enemy is on the platform we are testing against
		if (std::abs(curPlatformY1 - curEnemyY2) <= 5) 
		{

			int curHeight = pCurEnemy->getMovementManager().getHeight();
			Vect2 leftHitboxVect2 = Vect2(curPlatformHitbox.getTopLeft().getX(), curPlatformHitbox.getTopLeft().getY() - curHeight);
			Hitbox platformHitboxLeft = Hitbox(leftHitboxVect2, pCurEnemy->getMovementManager().getMovementVect2().getX(), curHeight);
			Vect2 rightHitboxVect2 = Vect2(curPlatformHitbox.getBottomRight().getX() - pCurEnemy->getMovementManager().getMovementVect2().getX(), curPlatformHitbox.getTopLeft().getY() - curHeight);
			Hitbox platformHitboxRight = Hitbox(rightHitboxVect2, pCurEnemy->getMovementManager().getMovementVect2().getX(), curHeight);

			bool collide = false;

			EDirection curDirection     = EDirection_INVALID;
			EDirection directionToSetTo = EDirection_INVALID;
			bool doSeparate = true;
			if (platformHitboxLeft.overlap(pCurEnemy->getMovementManager().getHitbox()))
			{
				collide			 = true;
				curDirection	 = EDirection_LEFT;
				directionToSetTo = EDirection_RIGHT;
				pCurEnemy->getMovementManager().getHitbox().separate(platformHitboxLeft, doSeparate);
			}
			else if (platformHitboxRight.overlap(pCurEnemy->getMovementManager().getHitbox()))
			{
				collide			 = true;
				curDirection	 = EDirection_RIGHT;
				directionToSetTo = EDirection_LEFT;
				pCurEnemy->getMovementManager().getHitbox().separate(platformHitboxRight, doSeparate);
			}

			if (collide)
			{
				if (collisionManager.isInThisFrameCollisions(pCurEnemy) == false)
				{
					collisionManager.mThisFrameCollisions.push_back(Collision(pCurEnemy, &curPlatform));
					if (pCurEnemy->getMovementManager().getCurDirection() != directionToSetTo)
					{
						pCurEnemy->getMovementManager().collided(curDirection);
						if (pCurEnemy->getMovementManager().getSwitchedDir())
						{
							pCurEnemy->isPathBlocked();
						}
					}
				}
				return;
			}
		}
	}
}

void WorldData::collectedCollectible(Collectible* curCollectible)
{
	curCollectible->setAmPickedUp(true);
	CanGoToNextLevelResults results = canGoToNextLevel();;
	switch (curCollectible->getCharacterType())
	{
	case EEntityCharacterTypes_C_KEY:
		mPlayer.updateKeys(1);
		break;
	case EEntityCharacterTypes_C_SAVE_POINT:
		saveInGameStats();
		break;
	case EEntityCharacterTypes_C_LOTUS_COLLECTIBLE:
		mPlayer.addHeldCollectible(curCollectible);
		break;
	case EEntityCharacterTypes_C_END_OF_LEVEL:
		if (results.mCanGoToNextLevel)
		{
			setNextLevel(results.mNextWorldNumber, results.mNextLevelNumber);
		}
		else
		{
			curCollectible->setAmPickedUp(false);
		}
		break;
	default:
		SDL_assert(false);
		break;
	}
	
}


void WorldData::playerShootProjectile(EEntityMovementPath path)
{
	//First test should be ask player if they can shoot before all this...
	// maybe can't shoot while recoil from last shot, reloading, in double jump...

	if (mpProjectiles.size() + 1 <= mProjectileLimit)
	{
		Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
		if (		   (pCurLevel->mThrowProjectileAllowed and path == EEntityMovementPath_HORIZONTAL)
			or (pCurLevel->mThrowDownwardProjectileAllowed and path == EEntityMovementPath_VERTICAL))
		{
			PlayerProjectilePreset preset = PlayerProjectilePreset(path);
			EDirection curDirection = mPlayer.getMovementManager().getCurFacingDirection();
			int projectileX1 = 0;
			int projectileY1 = mPlayer.getMovementManager().getHitbox().getTopLeft().getY();
			int spaceFromPlayer = 10;

			if (curDirection == EDirection_LEFT)
			{
				projectileX1 = mPlayer.getMovementManager().getHitbox().getTopLeft().getX() - preset.mWidth - spaceFromPlayer;
			}
			else
			{
				projectileX1 = mPlayer.getMovementManager().getHitbox().getBottomRight().getX() + spaceFromPlayer;
			}

			EDirection projectileDirection = EDirection_NONE;
			if 		(path == EEntityMovementPath_HORIZONTAL)
			{
				projectileDirection = curDirection;
			}
			else if (path == EEntityMovementPath_VERTICAL)
			{
				projectileDirection = EDirection_DOWN;
			}

			Projectile * pProjectile = new Projectile(Vect2(projectileX1, projectileY1), &preset, projectileDirection, mPlayer.mName);
			pProjectile->mAnimationManager.setUpAllTextures(mScreen.mpRenderer);
			pProjectile->setHitboxTexture(AssetManager::getTextureFromSurface(mScreen.mpRenderer, pProjectile->getImageObjectHitbox().getSurface()));
			mpProjectiles.push_back(pProjectile);
		}
	}

}

void WorldData::playerSwordSlash(SlashManager& slashManager)
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	if (pCurLevel->mSlashAllowed)
	{
		slashManager.startSlash();
		slashManager.mAnimationManager.setUpAllTextures(mScreen.mpRenderer);
		slashManager.mHitboxTexture = AssetManager::getTextureFromSurface(mScreen.mpRenderer, slashManager.mImageObjectHitbox.getSurface());
		slashManager.mSlashImageHitboxTexture = AssetManager::getTextureFromSurface(mScreen.mpRenderer, slashManager.mImageObjectImageHitbox.getSurface());

	}
}

void WorldData::enemyShootProjectile(Enemy* pCurEnemy)
{
	EnemyProjectilePreset preset = EnemyProjectilePreset();

	int projectileX1 = pCurEnemy->getMovementManager().getHitbox().getCenter().getX() - (preset.mWidth / 2);
	int projectileY1 = pCurEnemy->getMovementManager().getHitbox().getCenter().getY() - (preset.mHeight / 2);

	int difX = mPlayer.getMovementManager().getHitbox().getCenter().getX() - pCurEnemy->getMovementManager().getHitbox().getCenter().getX();
	int difY = mPlayer.getMovementManager().getHitbox().getCenter().getY() - pCurEnemy->getMovementManager().getHitbox().getCenter().getY();
	int difH = (int)sqrt(pow(difX, 2) + pow(difY, 2));
	int defaultProjectileMovement = preset.mMovementVect2.getX();
	float distanceToMovementRatio = (float)difH / (float)defaultProjectileMovement;
	int moveX = int((float)difX / distanceToMovementRatio);
	int moveY = int((float)difY / distanceToMovementRatio);

	EDirection projectileDirectionX = EDirection_LEFT;
	if (moveX > 0)
	{
		projectileDirectionX = EDirection_RIGHT;
	}

	EDirection projectileDirectionY = EDirection_UP;
	if (moveY > 0)
	{
		projectileDirectionY = EDirection_DOWN;
	}

	Projectile * pProjectile = new Projectile(Vect2(projectileX1 + moveX, projectileY1 + moveY), Vect2(std::abs(moveX), std::abs(moveY)), &preset, projectileDirectionX, projectileDirectionY, pCurEnemy->mName);
	pProjectile->mAnimationManager.setUpAllTextures(mScreen.mpRenderer);
	pProjectile->setHitboxTexture(AssetManager::getTextureFromSurface(mScreen.mpRenderer, pProjectile->getImageObjectHitbox().getSurface()));
	mpProjectiles.push_back(pProjectile);
	bool doSeparate = true;
	pProjectile->getMovementManager().getHitbox().separate(pCurEnemy->getMovementManager().getHitbox(), doSeparate);
	pProjectile->getMovementManager().getHitbox().updateTopLeft(Vect2(moveX, moveY));

	pCurEnemy->shootProjectile();
}


void WorldData::entityPostTick()
{
	Level* curLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];

	mPlayer.postTick();

	// ALL Platforms
	std::vector <Platform*> allPlatforms = curLevel->getAllActivePlatforms();
	for (Platform* pCurPlatform : allPlatforms)
	{
		pCurPlatform->getMovementManager().postTick();
		pCurPlatform->postTick();
	}

	//ENEMIES
	for (Enemy* pCurEnemy : curLevel->mpActiveEnemies)
	{
		pCurEnemy->postTick();
	}

	//PROJECTILES
	for (Projectile* pCurProjectile : mpProjectiles)
	{
		pCurProjectile->postTick();
	}

	if (mPlayer.getAmAlive() == false)
	{
		resetToCheckpoint();
	}
}

void WorldData::clearDeadProjectiles()
{
	for (int count = (int)mpProjectiles.size() -1; count >= 0; count--)
	{
		if (mpProjectiles[count]->isAlive() == false)
		{
			delete mpProjectiles[count];
			mpProjectiles.erase(mpProjectiles.begin() + count);
		}
	}
}

int  WorldData::getNumPlayerProjectiles()
{
	int numPlayerProjectiles = 0;
	for (int count = 0; count < mpProjectiles.size(); count++)
	{
		if (mpProjectiles[count]->getCharacterType() == EEntityCharacterTypes_PJ_PLAYER_PROJECTILE)
		{
			numPlayerProjectiles++;
		}
	}
	return numPlayerProjectiles;
}

void WorldData::updatePermanentCollectibles()
{
	for (int count = (int)mPlayer.mpCurHeldCollectibles.size() - 1; count >= 0; count--)
	{
		Collectible* curCollectible = mPlayer.mpCurHeldCollectibles[count];
		if (curCollectible->isPermanentlyPickedUp())
		{
			mNumLotusCollectibles += 1;
			mPlayer.mpCurHeldCollectibles.erase(mPlayer.mpCurHeldCollectibles.begin() + count);
		}
	}
}

void WorldData::killedCharacter(Entity* pCharacterKilled, bool instantDeath)
{
	if (instantDeath)
	{
		if (pCharacterKilled == &mPlayer)
		{
			//player killed
			resetToCheckpoint();
		}
		pCharacterKilled->died();
	}
	else
	{
		if (pCharacterKilled == &mPlayer)
		{
			//player damaged
			mPlayer.takeDamage();
		}
		pCharacterKilled->takeDamage();
	}

}


void WorldData::saveInGameStats()
{
	mPlayer.setCheckpointStats();

	Level * curLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];

	for (int count = 0; count < curLevel->mpPlatforms.size(); count++)
	{
		curLevel->mpPlatforms[count]->setCheckpointStats();
	}
	for (int count = 0; count < curLevel->mpAllNonStaticPlatforms.size(); count++)
	{
		curLevel->mpAllNonStaticPlatforms[count]->setCheckpointStats();
	}
	for (int count = 0; count < curLevel->mpAllEnemies.size(); count++)
	{
		curLevel->mpAllEnemies[count]->setCheckpointStats();
	}
	for (int count = 0; count < curLevel->mpAllCollectibles.size(); count++)
	{
		curLevel->mpAllCollectibles[count]->setCheckpointStats();
	}

}

void WorldData::resetStats()
{
	mPlayer.resetStats();
	mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber]->resetStats();
	resetBaseStats();
	saveInGameStats();
}

void WorldData::resetToCheckpoint()
{
	mPlayer.resetToCheckpoint();
	mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber]->resetToCheckpoint();
	resetBaseStats();
}

void WorldData::resetBaseStats()
{
	if (mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber]->mDoubleJumpAllowed)
	{
		mPlayer.getMovementManager().setMaxJumps(2);
	}
	for (int count = 0; count < mpProjectiles.size(); count++)
	{
		mpProjectiles[count]->died();
	}
}


CanGoToNextLevelResults WorldData::canGoToNextLevel()
{
	int nextWorldNumber = mCurWorldNumber;
	int nextLevelNumber = mCurLevelNumber;
	bool canGoToNextLevel = false;
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	if (mCurLevelNumber != mpWorlds[mCurWorldNumber]->mpLevels.size() - 1)
	{
		nextLevelNumber += 1;
		canGoToNextLevel = true;
	}
	else if (mCurWorldNumber != mpWorlds.size() - 1)
	{
		nextWorldNumber += 1;
		nextLevelNumber = 0;
		canGoToNextLevel = true;
	}
	if (pCurLevel->mMustKillAllEnemies and pCurLevel->mpActiveEnemies.size() != 0)
	{
		canGoToNextLevel = false;
	}

	return CanGoToNextLevelResults(nextWorldNumber, nextLevelNumber, canGoToNextLevel);
}

void WorldData::setNextLevel(int nextWorldNumber, int nextLevelNumber)
{
	mCurWorldNumber = nextWorldNumber;
	mCurLevelNumber = nextLevelNumber;
	mPlayer.getMovementManager().setStartPosition(&mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber]->mPlayerStartingPosition);
	resetStats();
}


void WorldData::renderTexture(SDL_Texture* pTexture, Hitbox& hitbox, Vect2 imageOffset)
{
	EImageOffset offsetType = EImageOffset_PRINT_TOP_LEFT;
	EDirection direction = EDirection_LEFT;
	bool rotating = false;
	float rotation = 0.0;
	renderTexture(pTexture, hitbox, offsetType, imageOffset, direction, rotating, rotation);

}

void WorldData::renderTexture(SDL_Texture* pTexture, Hitbox& hitbox, EImageOffset offsetType, Vect2 imageOffset, EDirection entityFacingDirection, bool rotating, float rotation)
{
	Level* curLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];

	float printX = float(hitbox.getTopLeft().getX() + imageOffset.getX() - mCurLevelChunk.mHitbox.getTopLeft().getX()) * mScreen.mGameScreenToGameLevelChunkRatio;
	float printY = float(hitbox.getTopLeft().getY() + imageOffset.getY() - mCurLevelChunk.mHitbox.getTopLeft().getY()) * mScreen.mGameScreenToGameLevelChunkRatio;
	float printWidth  = float(hitbox.getWidth()  * mScreen.mGameScreenToGameLevelChunkRatio);
	float printHeight = float(hitbox.getHeight() * mScreen.mGameScreenToGameLevelChunkRatio);

	SDL_FPoint rotationPoint = {printWidth / 2, printHeight / 2};
	if (rotating)
	{
		if (offsetType == EImageOffset_PRINT_TOP_LEFT)
		{
			rotationPoint = {0, 0};
			if (entityFacingDirection == EDirection_LEFT)
			{
				rotationPoint = {printWidth, 0};
			}
		}
		else if (offsetType == EImageOffset_FULL_UPWARDS)
		{
			//this doesn't make sense in this case
			//SDL_assert(false);
		}
		else if (offsetType == EImageOffset_MIDDLE)
		{
			//default 

		}
		else if (offsetType == EImageOffset_LEFT_X_MIDDLE_Y)
		{
			rotationPoint = {0.0, printHeight / 2};
			if (entityFacingDirection == EDirection_LEFT)
			{
				printX -= printWidth;
				rotationPoint = {printWidth, printHeight / 2};
			}
		}
		else if (offsetType == EImageOffset_MIDDLE_X_TOP_Y)
		{
			rotationPoint = {printWidth / 2, 0.0};
		}
		else
		{
			SDL_assert(false);
		}

		if (entityFacingDirection == EDirection_RIGHT)
		{
			rotation += 180;
		}
	}
	else
	{
		rotation = 0.0;
	}

	SDL_FRect curEntityPositionToPrintTo = { printX, printY, printWidth, printHeight };

	SDL_RendererFlip direction = SDL_FLIP_NONE;
	if (entityFacingDirection == EDirection_RIGHT)
	{
		direction = SDL_FLIP_HORIZONTAL;
	}

	SDL_RenderTextureRotated(mScreen.mpRenderer, pTexture, NULL, &curEntityPositionToPrintTo, rotation, &rotationPoint, direction);


}

void WorldData::renderEntityWithHitbox(Entity* pCurEntity)
{
	Vect2 hitboxOffset = Vect2(0, 0);
	renderTexture(pCurEntity->getHitboxTexture(), pCurEntity->getMovementManager().getHitbox(), hitboxOffset);
	renderEntity(pCurEntity);
}

void WorldData::renderEntityWithHitbox(ImageObject* pCurImageObject, SDL_Texture* pHitboxTexture, EImageOffset offsetType, EDirection entityFacingDirection, Hitbox& entityHitbox, bool rotating, float rotation)
{
	Vect2 hitboxOffset = Vect2(0, 0);
	renderTexture(pHitboxTexture, entityHitbox, offsetType, hitboxOffset, entityFacingDirection, rotating, rotation);
	renderEntity(pCurImageObject, offsetType, entityFacingDirection, entityHitbox, rotating, rotation);
}

void WorldData::renderEntityViaChunk(Entity* pCurEntity) 
{
	if (pCurEntity->mIsVisible)
	{
		ImageObject* pCurImageObject = pCurEntity->mAnimationManager.getCurImage();
		Hitbox& hitbox = pCurEntity->getMovementManager().getHitbox();

		renderEntityViaChunk(pCurImageObject, hitbox);
	}
}

void WorldData::renderEntityViaChunk(ImageObject* pCurImageObject, Hitbox& entityHitbox) 
{
	//CAN EITHER CHUNK DOWN OR CHUNK RIGHT
	Level* curLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];

	int imageOffsetX = pCurImageObject->getImageOffsetX();
	int imageOffsetY = pCurImageObject->getImageOffsetY();

	float rightChunkShift = 0.0;
	float upChunkShift    = 0.0;
	if (entityHitbox.getWidth() == entityHitbox.getHeight())
	{
		float printX = float(entityHitbox.getTopLeft().getX() - mCurLevelChunk.mHitbox.getTopLeft().getX() + imageOffsetX) * mScreen.mGameScreenToGameLevelChunkRatio;
		float printY = float(entityHitbox.getTopLeft().getY() - mCurLevelChunk.mHitbox.getTopLeft().getY() + imageOffsetY) * mScreen.mGameScreenToGameLevelChunkRatio;

		float printWidth   = entityHitbox.getWidth()  * mScreen.mGameScreenToGameLevelChunkRatio;
		float printHeight  = entityHitbox.getHeight() * mScreen.mGameScreenToGameLevelChunkRatio;

		float actualWidth  = (float)pCurImageObject->getSurface()->w;
		float actualHeight = (float)pCurImageObject->getSurface()->h;
		float actualToPrintRatio = actualWidth / printWidth;

		float cropWidth   = printWidth  * actualToPrintRatio;
		float cropHeight  = printHeight * actualToPrintRatio;

		SDL_FRect curCropBox = {0, 0, cropWidth, cropHeight};
		SDL_FRect curEntityPositionToPrintTo = { printX, printY, printWidth, printHeight };

		SDL_RenderTextureRotated(mScreen.mpRenderer, pCurImageObject->getTexture(), &curCropBox, &curEntityPositionToPrintTo, 0, NULL, SDL_FLIP_NONE);
	}
	else
	{
		for (int count = 0; count <= pCurImageObject->mNumChunks - 1; count++)
		{
			if      (pCurImageObject->mChunkDirection == EDirection_RIGHT)
			{
				rightChunkShift = float(count * pCurImageObject->getIdealImageWidth());
			}
			else if (pCurImageObject->mChunkDirection == EDirection_DOWN)
			{
				upChunkShift    = float(count * pCurImageObject->getIdealImageHeight());
			}
			else
			{
				SDL_assert(false);
			}

			float printX = float(entityHitbox.getTopLeft().getX() - mCurLevelChunk.mHitbox.getTopLeft().getX() + imageOffsetX + rightChunkShift ) * mScreen.mGameScreenToGameLevelChunkRatio;
			float printY = float(entityHitbox.getTopLeft().getY() - mCurLevelChunk.mHitbox.getTopLeft().getY() + imageOffsetY + upChunkShift    ) * mScreen.mGameScreenToGameLevelChunkRatio;

			float printWidth  = float(pCurImageObject->getIdealImageWidth())  * mScreen.mGameScreenToGameLevelChunkRatio;
			float printHeight = float(pCurImageObject->getIdealImageHeight()) * mScreen.mGameScreenToGameLevelChunkRatio;

			float actualWidth  = (float)pCurImageObject->getSurface()->w;
			float actualHeight = (float)pCurImageObject->getSurface()->h;
			float actualToPrintRatio = actualWidth / printWidth;

			float cropWidth   = actualWidth;
			float cropHeight  = actualHeight;

			if (count == pCurImageObject->mNumChunks - 1)
			{
				//last chunk is shortened based on where platform ends
				if (pCurImageObject->mChunkDirection == EDirection_RIGHT)
				{
					float width  = entityHitbox.getWidth()  - (rightChunkShift);
					printWidth  = width * mScreen.mGameScreenToGameLevelChunkRatio;
					cropWidth   = printWidth * actualToPrintRatio;
				}
				else if (pCurImageObject->mChunkDirection == EDirection_DOWN)
				{
					float height = entityHitbox.getHeight() - (upChunkShift);
					printHeight  = height * mScreen.mGameScreenToGameLevelChunkRatio;
					cropHeight  = printHeight * actualToPrintRatio;
				}
			}


			SDL_FRect curCropBox = {0.0, 0.0, cropWidth, cropHeight};
			SDL_FRect curEntityPositionToPrintTo = { printX, printY, printWidth, printHeight };

			SDL_RenderTextureRotated(mScreen.mpRenderer, pCurImageObject->getTexture(), &curCropBox, &curEntityPositionToPrintTo, 0.0, NULL, SDL_FLIP_NONE);
		}
	}
}

void WorldData::renderEntityViaSplice(Entity* pCurEntity) 
{
	if (pCurEntity->mIsVisible)
	{
		AnimationManager& animationManager = pCurEntity->mAnimationManager;
		Hitbox&           hitbox		   = pCurEntity->getMovementManager().getHitbox();
		renderEntityViaSplice(animationManager, hitbox);
	}
}

void WorldData::renderEntityViaSplice(AnimationManager& animationManager, Hitbox& entityHitbox) 
{
	//CAN EITHER SPLICE RIGHT OR DOWN
	Level* curLevel           = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];

	ImageObject* pCurImageObject = animationManager.getCurImage();

	int imageOffsetX = pCurImageObject->getImageOffsetX();
	int imageOffsetY = pCurImageObject->getImageOffsetY();

	float rightChunkShift = 0.0;
	float upChunkShift    = 0.0;
	if (entityHitbox.getWidth() == entityHitbox.getHeight())
	{
		//square
		float printX = float(entityHitbox.getTopLeft().getX() - mCurLevelChunk.mHitbox.getTopLeft().getX() + imageOffsetX) * mScreen.mGameScreenToGameLevelChunkRatio;
		float printY = float(entityHitbox.getTopLeft().getY() - mCurLevelChunk.mHitbox.getTopLeft().getY() + imageOffsetY) * mScreen.mGameScreenToGameLevelChunkRatio;

		float printWidth   = entityHitbox.getWidth()  * mScreen.mGameScreenToGameLevelChunkRatio;
		float printHeight  = entityHitbox.getHeight() * mScreen.mGameScreenToGameLevelChunkRatio;

		float actualWidth  = (float)pCurImageObject->getSurface()->w;
		float actualHeight = (float)pCurImageObject->getSurface()->h;
		float actualToPrintRatio = actualWidth / printWidth;

		float cropWidth   = printWidth  * actualToPrintRatio;
		float cropHeight  = printHeight * actualToPrintRatio;

		SDL_FRect curCropBox = {0, 0, cropWidth, cropHeight};
		SDL_FRect curEntityPositionToPrintTo = { printX, printY, printWidth, printHeight };

		SDL_RenderTextureRotated(mScreen.mpRenderer, pCurImageObject->getTexture(), &curCropBox, &curEntityPositionToPrintTo, 0, NULL, SDL_FLIP_NONE);
	}
	else
	{
		//splice images by pasting each animation frame one after another until total shape is filled
		int curImage = 0;

		for (int count = 0; count < pCurImageObject->mNumChunks; count++)
		{
			pCurImageObject = animationManager.getCurFrames()[curImage];
			if (pCurImageObject->mChunkDirection == EDirection_RIGHT)
			{
				rightChunkShift = float(count * pCurImageObject->getIdealImageWidth());
			}
			else if (pCurImageObject->mChunkDirection == EDirection_DOWN)
			{
				upChunkShift    = float(count * pCurImageObject->getIdealImageHeight());
			}
			else
			{
				SDL_assert(false);
			}

			float printX = float(entityHitbox.getTopLeft().getX() - mCurLevelChunk.mHitbox.getTopLeft().getX() + imageOffsetX + rightChunkShift ) * mScreen.mGameScreenToGameLevelChunkRatio;
			float printY = float(entityHitbox.getTopLeft().getY() - mCurLevelChunk.mHitbox.getTopLeft().getY() + imageOffsetY + upChunkShift    ) * mScreen.mGameScreenToGameLevelChunkRatio;

			float printWidth  = float(pCurImageObject->getIdealImageWidth())  * mScreen.mGameScreenToGameLevelChunkRatio;
			float printHeight = float(pCurImageObject->getIdealImageHeight()) * mScreen.mGameScreenToGameLevelChunkRatio;

			float actualWidth  = (float)pCurImageObject->getSurface()->w;
			float actualHeight = (float)pCurImageObject->getSurface()->h;
			float actualToPrintRatio = actualWidth / printWidth;

			SDL_FRect printBox = {0.0, 0.0, actualWidth, actualHeight};
			SDL_FRect curEntityPositionToPrintTo = { printX, printY, printWidth, printHeight };

			SDL_RenderTextureRotated(mScreen.mpRenderer, pCurImageObject->getTexture(), &printBox, &curEntityPositionToPrintTo, 0, NULL, SDL_FLIP_NONE);

			curImage += 1;
			if (curImage >= animationManager.getCurFrames().size())
			{
				curImage = 0;
			}
		}
	}
}

void WorldData::renderEntity(Entity* pCurEntity)
{
	if (pCurEntity->mIsVisible)
	{
		ImageObject*	pCurImageObject = pCurEntity->mAnimationManager.getCurImage();
		EDirection		imageDirection	 = pCurEntity->getMovementManager().getCurFacingDirection();
		EImageOffset	imageOffsetType = EImageOffset_MIDDLE;
		if (pCurEntity->getMovementManager().getPath() == EEntityMovementPath_HORIZONTAL_CAN_FALL)
		{
			imageOffsetType = EImageOffset_FULL_UPWARDS;
		}
		Hitbox&			hitbox = pCurEntity->getMovementManager().getHitbox();
		renderEntity(pCurImageObject, imageOffsetType, imageDirection, hitbox);
	}
}

void WorldData::renderEntity(ImageObject* pCurImageObject, EImageOffset offsetType, EDirection entityFacingDirection, Hitbox& entityHitbox)
{
	Level* curLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];

	int imageOffsetX = -((pCurImageObject->getIdealImageWidth() - entityHitbox.getWidth()) / 2) + pCurImageObject->getImageOffsetX();
	int imageOffsetY = pCurImageObject->getImageOffsetY();
	if (offsetType == EImageOffset_MIDDLE)
	{
		imageOffsetY -= (pCurImageObject->getIdealImageHeight() - entityHitbox.getHeight()) / 2;
	}
	else if (offsetType == EImageOffset_FULL_UPWARDS)
	{
		imageOffsetY -= (pCurImageObject->getIdealImageHeight() - entityHitbox.getHeight());
	}
	else
	{
		SDL_assert(false);
	}
	
	bool rotating = false;
	float rotation = 0.0;

	Hitbox imageHitbox = Hitbox(entityHitbox.getTopLeft(), pCurImageObject->getIdealImageWidth(), pCurImageObject->getIdealImageHeight());
	renderTexture(pCurImageObject->getTexture(), imageHitbox, offsetType, Vect2(imageOffsetX, imageOffsetY), entityFacingDirection, rotating, rotation);
}

void WorldData::renderEntity(ImageObject* pCurImageObject, EImageOffset offsetType, EDirection entityFacingDirection, Hitbox& entityHitbox, bool rotating, float rotation)
{
	Vect2 imageOffset = Vect2(0,0);
	renderTexture(pCurImageObject->getTexture(), entityHitbox, offsetType, imageOffset, entityFacingDirection, rotating, rotation);
}

void WorldData::renderBackgrounds()
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	if (pCurLevel->mArtFileName != "")
	{

		int printW = mCurLevelChunk.getHitbox().getWidth();
		int printH = mCurLevelChunk.getHitbox().getHeight();

		int xShift = pCurLevel->mArtFileX;
		int yShift = pCurLevel->mArtFileY;

		int levelW = pCurLevel->mArtFileWidth;
		int levelH = pCurLevel->mArtFileHeight;
		int levelX2 = pCurLevel->mArtFileWidth  + xShift;
		int levelY2 = pCurLevel->mArtFileHeight + yShift;

		float actualWidth  = (float)pCurLevel->mArtFileSurface->w;
		float actualHeight = (float)pCurLevel->mArtFileSurface->h;
		float actualToPrintRatio = actualWidth / levelW;

		int curLevelChunkX1 = mCurLevelChunk.getHitbox().getTopLeft().getX();
		int curLevelChunkX2 = mCurLevelChunk.getHitbox().getBottomRight().getX();
		int curLevelChunkY1 = mCurLevelChunk.getHitbox().getTopLeft().getY();
		int curLevelChunkY2 = mCurLevelChunk.getHitbox().getBottomRight().getY();

		float cropX = (curLevelChunkX1 - xShift) * actualToPrintRatio;
		float cropY = (curLevelChunkY1 - yShift) * actualToPrintRatio;
		float cropW = printW * actualToPrintRatio;
		float cropH = printH * actualToPrintRatio;

		float dX = 0.0f;
		float dY = 0.0f;
		float dW = (float)mScreen.mGameScreenWidth;
		float dH = (float)mScreen.mGameScreenHeight;

		if (curLevelChunkX1 < xShift)
		{
			dX     = (xShift - curLevelChunkX1) * mScreen.mGameScreenToGameLevelChunkRatio;
			cropW += (xShift - curLevelChunkX1) * actualToPrintRatio;
		}
		if (curLevelChunkY1 < yShift)
		{
			dY     = (yShift - curLevelChunkY1) * mScreen.mGameScreenToGameLevelChunkRatio;
			cropH += (yShift - curLevelChunkY1) * actualToPrintRatio;
		}

		if (curLevelChunkX2 > (levelW + xShift))
		{
			dW    -= (curLevelChunkX2 - (levelW + xShift)) * mScreen.mGameScreenToGameLevelChunkRatio;
			cropW -= (curLevelChunkX2 - (levelW + xShift)) * actualToPrintRatio;
		}
		if (curLevelChunkY2 > (levelH + yShift))
		{
			dH    -= (curLevelChunkY2 - (levelH + yShift)) * mScreen.mGameScreenToGameLevelChunkRatio;
			cropH -= (curLevelChunkY2 - (levelH + yShift)) * actualToPrintRatio;
		}

		SDL_FRect destination = {dX, dY, dW, dH};
		SDL_FRect curCropBox = {cropX, cropY, cropW, cropH};

		SDL_RenderTextureRotated(mScreen.mpRenderer, pCurLevel->mArtFileTexture, &curCropBox, &destination, 0, NULL, SDL_FLIP_NONE);
	}
}

void WorldData::renderBackgroundEffects()
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	ScreenObject & screen = mScreen;
	for (int count = 0; count < pCurLevel->mpBackgroundEffects.size(); count++)
	{
		CircleEffect* pCircleEffect = pCurLevel->mpBackgroundEffects[count];
		renderCircleGradient(pCircleEffect->mColor, pCircleEffect->mCurCenter, pCircleEffect->mRadius);
	}
}

void WorldData::renderCircleGradient(SDL_Color color, Vect2 center, int radius)
{
	SDL_Renderer* pRenderer = mScreen.mpRenderer;
	float gameScreenToGameLevelChunkRatio = mScreen.mGameScreenToGameLevelChunkRatio;

	float actualCenterX = (center.getX() - mCurLevelChunk.mHitbox.getTopLeft().getX()) * gameScreenToGameLevelChunkRatio;
	float actualCenterY = (center.getY() - mCurLevelChunk.mHitbox.getTopLeft().getY()) * gameScreenToGameLevelChunkRatio;
	float actualRadius  = radius * gameScreenToGameLevelChunkRatio;


	for (int degrees = 0; degrees < 360; degrees++)
	{
		for (int curRadius = 0; curRadius < actualRadius; curRadius++)
		{
			float radians = degToRad(degrees);
			float curX = actualCenterX + (cos(radians) * curRadius);
			float curY = actualCenterY + (sin(radians) * curRadius);
			float alpha = (actualRadius - curRadius) / actualRadius;
			SDL_SetRenderDrawColor(pRenderer, color.r, color.g, color.b, Uint8(alpha));
			SDL_RenderPoint(pRenderer, curX, curY);
		}
	}
}
