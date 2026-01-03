#include "worldData.h"


#define degToRad(angleDegrees) ((float)(angleDegrees) * M_PI / 180.0f)
#define radToDeg(angleRadians) ((float)(angleRadians) * 180.0f / M_PI)


WorldData::WorldData(ScreenObject& screenObject, SlashManager& slashManager, CollisionManager& collisionManager) 
		: mScreen(screenObject), mSlashManager(slashManager), mCollisionManager(collisionManager){;}

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

	delete mpNextLevelData; 
	mpNextLevelData = nullptr;
}


void WorldData::getAllDynamicEntities(std::vector<Entity*>& pDynamicEntities, const Hitbox& hitbox)
{
	Level* pLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	pLevel->mDynamicEntities.getEntitiesInHitbox(pDynamicEntities, hitbox);
	if (mPlayer.getMovementManager().getHitbox().overlap(hitbox)) 
	{
		pDynamicEntities.push_back(&mPlayer);
	}
	for (Projectile* projectile : mpProjectiles)
	{
		if (projectile->getMovementManager().getHitbox().overlap(hitbox)) 
		{
			pDynamicEntities.push_back(projectile);
		}
	}
}

void WorldData::getAllStaticEntities(std::vector<Entity*>& pStaticEntities, const Hitbox& hitbox) 
{
	Level* pLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	pLevel->mStaticEntities.getEntitiesInHitbox(pStaticEntities, hitbox);
}

void WorldData::entityPreTickCalcMovement()
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];

	// PLAYER
	mPlayer.preTick();
	mPlayer.tick();

	pCurLevel->removeInactiveEntitiesFromActiveEntitiesVector();

	// REGULAR PLATFORMS
	for (Platform* pCurPlatform : pCurLevel->mpPlatforms)
	{
		pCurPlatform->preTick();
	}

	// NON STATIC PLATFORMS
	for (Platform* pCurPlatform : pCurLevel->mpActiveNonStaticPlatforms)
	{
		pCurPlatform->preTick();
		pCurPlatform->tick();
	}

	// ENEMIES
	for (Enemy* pCurEnemy : pCurLevel->mpActiveEnemies)
	{
		pCurEnemy->preTick();
		if (pCurEnemy->shouldShootProjectile())
		{
			enemyShootProjectile(*pCurEnemy);
		}
		pCurEnemy->tick();
	}

	// PROJECTILES
	for (Projectile* pCurProjectile : mpProjectiles)
	{
		pCurProjectile->preTick();
		pCurProjectile->tick();
	}

	// COLLECTIBLES
	for (Collectible* pCurCollectible : pCurLevel->mpActiveCollectibles)
	{
		pCurCollectible->preTick();
		pCurCollectible->tick();
	}

	mSlashManager.tick(mPlayer.getMovementManager());
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

	// move towards player
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

// old collision system
#if COLLISION_SYSTEM == 0

void WorldData::updateNonstaticMovement(std::vector<Entity*>& pNonstaticEntities)
{
	for (Entity* pCurEntity : pNonstaticEntities)
	{
		pCurEntity->getMovementManager().moveToWantToMoveTo();
	}
}

void WorldData::entityCollisions()
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];

	// move
	// get all nonstatic in level
	std::vector <Entity*> pNonstaticEntities;
	getAllDynamicEntities(pNonstaticEntities, pCurLevel->getHitbox());

	updateNonstaticMovement(pNonstaticEntities);

	// collision
	updatePlayerCollisions();
	
	updateCurLevelChunk();

	// NON STATIC PLATFORMS
	for (Platform* curPlatform : pCurLevel->mpActiveNonStaticPlatforms)
	{
		collideWithWorld(*curPlatform);
		collideWithPlatforms(*curPlatform);
		collideWithNonStaticPlatforms(*curPlatform);
		collideWithEnemies(*curPlatform);
	}

	// ENEMIES
	for (Enemy* curEnemy : pCurLevel->mpActiveEnemies)
	{
		collideWithWorld(*curEnemy);
		collideWithPlatforms(*curEnemy);
		checkIfOnEdgeOfPlatform(*curEnemy);
		collideWithNonStaticPlatforms(*curEnemy);
		collideWithEnemies(*curEnemy);
	}

	// PROJECTILES
	for (Projectile* curProjectile : mpProjectiles)
	{
		collideWithWorld(*curProjectile);
		collideWithPlatforms(*curProjectile);
		collideWithNonStaticPlatforms(*curProjectile);
		collideWithEnemies(*curProjectile);
	}

	// SLASH COLLISIONS
	if (mSlashManager.mCurSlash)
	{
		std::vector<EntityDistance> struckEntities;
		int closestBlockingDistance = INT_MAX;
		std::vector<Entity*> entities = pCurLevel->getAllActiveEntities();

		int hitboxX1 = std::min(mPlayer.getMovementManager().getHitbox().getTopLeft().getX(), mSlashManager.mHitbox.getTopLeft().getX());
		int hitboxX2 = std::max(mPlayer.getMovementManager().getHitbox().getBottomRight().getX(), mSlashManager.mHitbox.getBottomRight().getX());
		int hitboxY1 = mSlashManager.mHitbox.getTopLeft().getY();
		int hitboxY2 = mSlashManager.mHitbox.getBottomRight().getY();
		Hitbox approxSlashHitbox = Hitbox(hitboxX1, hitboxX2, hitboxY1, hitboxY2);

		for (Entity* pEntity : entities)
		{
			if (pEntity->getAmAlive() and pEntity->getMovementManager().getHitbox().overlap(approxSlashHitbox))
			{
				if (pEntity->mVulnerableToProjectiles)
				{
					struckEntities.push_back(EntityDistance(*pEntity, getEntityDistance(*pEntity, mPlayer, approxSlashHitbox, mSlashManager.mCurRotation)));
				}
				else
				{
					int curBlockingDistance = getEntityDistance(*pEntity, mPlayer, approxSlashHitbox, mSlashManager.mCurRotation);
					if (closestBlockingDistance < curBlockingDistance)
					{
						closestBlockingDistance = curBlockingDistance;
					}
				}
			}
		}

		for (EntityDistance& curEntityDistance : struckEntities)
		{
			if (curEntityDistance.mDistance < closestBlockingDistance)
			{
				// entity closer, will be hit
				bool instantDeath = true;
				killedCharacter(curEntityDistance.mEntity, instantDeath);
			}
		}
	}
}

void WorldData::updatePlayerCollisions()
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	MovementManager& movementManager = mPlayer.getMovementManager();

	// WINDOW
	bool instantDeath = true;
	if (movementManager.getHitbox().getTopLeft().getX() <= 0)
	{
		movementManager.getHitbox().setTopLeftX(1);
	}
	else if (movementManager.getHitbox().getBottomRight().getX() >= pCurLevel->mLevelX2)
	{
		// CHANGE LEVEL
		movementManager.getHitbox().setTopLeftX(pCurLevel->mLevelX2 - movementManager.getHitbox().getWidth() - 1);
	}
	if (movementManager.getHitbox().getTopLeft().getY() <= 0)
	{
		movementManager.getHitbox().setTopLeftY(1);
	}
	else if (movementManager.getHitbox().getBottomRight().getY() >= pCurLevel->mLevelY2)
	{
		killedCharacter(mPlayer, instantDeath);
	}

	movementManager.setOnGroundFalse();
	bool check = true;

	// PLATFORMS
	std::vector <Entity*> platformEntities;
	pCurLevel->mStaticEntities.getEntitiesInHitbox(platformEntities, movementManager.getHitbox());
	pCurLevel->mDynamicEntities.getEntitiesInHitbox(platformEntities, movementManager.getHitbox(), EEntityClassTypes_PLATFORM);

	mPlayer.mCanWallJump = false;
	bool collidedWithWallJumpable = false;
	for (Entity* pEntity : platformEntities)
	{
		Platform* pCurPlatform = (Platform*)pEntity;

		Hitbox& curPlatformHitbox = pCurPlatform->getMovementManager().getHitbox();
		if (pCurPlatform->mIsVisible)
		{
			bool fallenOnIt = false;
			bool doSeparate = false;
			EBoxSide playerSeparationPath = curPlatformHitbox.separate(movementManager.getHitbox(), doSeparate);
			EBoxSide platformSeparationPath = movementManager.getHitbox().separate(curPlatformHitbox, doSeparate);

			if (platformSeparationPath == EBoxSide_TOP and (movementManager.getCurDirectionY() == EDirection_NONE or movementManager.getCurDirectionY() == EDirection_DOWN))
			{
				fallenOnIt = true;
			}

			EEntityEdgeType playerEdgeType = movementManager.getEdgeType(playerSeparationPath);
			EEntityEdgeType platformEdgeType = pCurPlatform->getMovementManager().getEdgeType(platformSeparationPath);
			
			if (platformEdgeType == EEntityEdgeType_WALL_JUMPABLE)
			{
				collidedWithWallJumpable = true;
			}
			if (platformEdgeType == EEntityEdgeType_NON_EXISTENT)
			{
				// PASS
			}
			else if (platformEdgeType == EEntityEdgeType_MOVEABLE)
			{
				mCollisionManager.mCollisionsToSeparate.push_back(Collision(mPlayer, *pCurPlatform));
				int xOverlap = rangeOverlapDistance(movementManager.getHitbox().getTopLeft().getX(), movementManager.getHitbox().getBottomRight().getX(), curPlatformHitbox.getTopLeft().getX(), curPlatformHitbox.getBottomRight().getX());
				if ((xOverlap > int(movementManager.getMovementVect2().getX() + movementManager.getMovementEffect()) / 2))
				{
					mCollisionManager.addCrateContact(*pCurPlatform);
					int curMovement = movementManager.getMovementVect2().getX() + movementManager.getMovementEffect();
					if (movementManager.getCurDirection() == EDirection_LEFT)
					{
						curMovement *= -1;
					}
					mCollisionManager.setCrateContactMovementIncrement(curMovement);
				}
				doSeparate = true;
				platformSeparationPath = movementManager.getHitbox().separate(curPlatformHitbox, doSeparate);
			}
			else
			{
				doSeparate = true;
				platformSeparationPath = movementManager.getHitbox().separate(curPlatformHitbox, doSeparate);

				if (willKillCharacter(mPlayer, platformEdgeType))
				{
					killedCharacter(mPlayer, !instantDeath);
				}

				spreadEdges(*pCurPlatform, mPlayer);

				// check for special cases
				if (platformEdgeType == EEntityEdgeType_TELEPORTER)
				{
					// TODO TELEPORT
					return;
				}

			}

			if (pCurPlatform->getCharacterType() == EEntityCharacterTypes_P_GATE and pCurPlatform->mIsVisible)
			{
				if (mPlayer.mKeys > 0)
				{
					pCurPlatform->hide();
					mPlayer.updateKeys(-1);
				}
			}
			else if (pCurPlatform->getCharacterType() == EEntityCharacterTypes_P_TARGET_GATE and pCurPlatform->mIsVisible)
			{
				if (mPlayer.mTargets > 0)
				{
					pCurPlatform->hide();
					mPlayer.updateTargets(-1);
				}
			}

			if (fallenOnIt)
			{
				movementManager.setOnGroundTrue(pCurPlatform->getMovementEffect(), pCurPlatform->getCurCharacteristics(), pCurPlatform->getMovementManager().getHitboxEdges().mTop);
				movementManager.getMovementStates()[EMovementStateIndex_JUMPING]->landed();
				((JumpingState*)movementManager.getMovementStates()[EMovementStateIndex_JUMPING])->endJump();

				playerEdgeType = movementManager.getHitboxEdges().mBottom;
				platformEdgeType = pCurPlatform->getMovementManager().getHitboxEdges().mTop;

				if (willKillCharacter(*pCurPlatform, playerEdgeType))
				{
					killedCharacter(*pCurPlatform, !instantDeath);
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
					mCollisionManager.addRidingContact(mPlayer, *pCurPlatform);
				}
			}
			if (pCurPlatform->getSubClassType() == EEntitySubClassTypes_AREA_EFFECT)
			{
				AreaEffectPlatform* pCurAreaEffectPlatform = (AreaEffectPlatform*)pCurPlatform;
				if (movementManager.getHitbox().overlap(pCurAreaEffectPlatform->mAreaEffectHitbox))
				{
					movementManager.push(pCurAreaEffectPlatform->mAreaEffectMovement, pCurAreaEffectPlatform->mEffectDirection);
				}
			}
		}
	}

	if (collidedWithWallJumpable && !movementManager.isOnGround())
	{
		mPlayer.mCanWallJump = true;
	}

	// ENEMIES
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
				EBoxSide enemySeparationPath = movementManager.getHitbox().separate(pCurEnemy->getMovementManager().getHitbox(), doSeparate);

				if (enemySeparationPath == EBoxSide_TOP)
				{
					fallenOnIt = true;
				}

				EEntityEdgeType enemyEdgeType = pCurEnemy->getMovementManager().getEdgeType(enemySeparationPath);
				if (willKillCharacter(mPlayer, enemyEdgeType))
				{
					killedCharacter(mPlayer, (enemyEdgeType == EEntityEdgeType_BURNING));
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
						movementManager.getHitbox().setTopLeft(Vect2(x, y));
						mCollisionManager.addRidingContact(mPlayer, *pCurEnemy);
						check = false;
					}
				}

				if (fallenOnIt and check)
				{
					movementManager.setOnGroundTrue(pCurEnemy->getMovementEffect(), pCurEnemy->getCurCharacteristics(), pCurEnemy->getMovementManager().getHitboxEdges().mTop);
					movementManager.getMovementStates()[EMovementStateIndex_JUMPING]->landed();
					((JumpingState*)movementManager.getMovementStates()[EMovementStateIndex_JUMPING])->endJump();

					EEntityEdgeType playerEdgeType = movementManager.getHitboxEdges().mBottom;

					if (willKillCharacter(*pCurEnemy, playerEdgeType) and mPlayer.getAmAlive() and !mPlayer.takingDamage())
					{
						killedCharacter(*pCurEnemy, !instantDeath);
					}

					// SPECIAL CASES
					if (enemyEdgeType == EEntityEdgeType_BOUNCY)
					{
						if (!movementManager.inJump())
						{
							movementManager.collideWithBouncy();
						}
					}
					else if (pCurEnemy->mRideable and pCurEnemy->getAmAlive())
					{
						mCollisionManager.addRidingContact(mPlayer, *pCurEnemy);
					}
				}
				else if (check)
				{
					mCollisionManager.entitiesCollidedHorizontal(*pCurEnemy, mPlayer);
				}
			}
		}
	}

	// COLLECTIBLES
	std::vector <Entity*> collectibleEntities;
	pCurLevel->mDynamicEntities.getEntitiesInHitbox(collectibleEntities, movementManager.getHitbox(), EEntityClassTypes_COLLECTIBLE);
	for (Entity* pEntity : collectibleEntities)
	{
		Collectible* pCurCollectible = (Collectible*)pEntity;
		if (pCurCollectible->getMovementManager().getHitbox().overlap(movementManager.getHitbox()) and !pCurCollectible->isAmPickedUp())
		{
			collectedCollectible(*pCurCollectible);
		}
	}

	// PROJECTILES
	for (Projectile* pCurProjectile : mpProjectiles)
	{
		if (pCurProjectile->getCharacterType() == EEntityCharacterTypes_PJ_ENEMY_PROJECTILE)
		{
			if (mPlayer.getAmAlive() and movementManager.getHitbox().overlap(pCurProjectile->getMovementManager().getHitbox()))
			{
				bool instantDeath = true;
				killedCharacter(mPlayer, pCurProjectile);
				killedCharacter(*pCurProjectile, &mPlayer);
			}
		}
	}

}

void WorldData::collideWithWorld(Entity& curEntity)
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	MovementManager& movementManager = curEntity.getMovementManager();
	Hitbox& curHitbox = curEntity.getMovementManager().getHitbox();
	bool instantDeath = true;
	bool isHorizontalPath = movementManager.getPath() == EEntityMovementPath_HORIZONTAL or movementManager.getPath() == EEntityMovementPath_HORIZONTAL_CAN_FALL;
	if (curHitbox.getTopLeft().getX() <= 0)
	{
		int updatedX = movementManager.getMovementVect2().getX();
		curHitbox.setTopLeftX(updatedX);
		if (isHorizontalPath)
		{
			movementManager.collided(EDirection_LEFT);
			if (movementManager.getDidSwitchedDir())
			{
				curEntity.setTrapped();
			}
		}
		if (curEntity.getClassType() == EEntityClassTypes_PROJECTILE)
		{
			killedCharacter(curEntity, instantDeath);
		}
	}
	else if (curHitbox.getBottomRight().getX() >= pCurLevel->mLevelX2)
	{
		curHitbox.setTopLeftX(pCurLevel->mLevelX2 - curHitbox.getWidth() - movementManager.getMovementVect2().getX());
		if (isHorizontalPath)
		{
			movementManager.collided(EDirection_RIGHT);
			if (movementManager.getDidSwitchedDir())
			{
				curEntity.setTrapped();
			}
		}
		if (curEntity.getClassType() == EEntityClassTypes_PROJECTILE)
		{
			killedCharacter(curEntity, instantDeath);
		}
	}

	bool isVerticalPath = movementManager.getPath() == EEntityMovementPath_VERTICAL;
	if (curHitbox.getTopLeft().getY() <= 0)
	{
		curHitbox.setTopLeftY(movementManager.getMovementVect2().getY());
		if (isVerticalPath)
		{
			movementManager.collided(EDirection_UP);

		}
		if (curEntity.getClassType() == EEntityClassTypes_PROJECTILE)
		{
			killedCharacter(curEntity, instantDeath);
		}
	}
	else if (curHitbox.getBottomRight().getY() >= pCurLevel->mLevelY2)
	{
		curHitbox.setTopLeftY(pCurLevel->mLevelY2 - curHitbox.getHeight() - movementManager.getMovementVect2().getY());
		if (isVerticalPath)
		{
			movementManager.collided(EDirection_DOWN);
		}
		else
		{
			// kill character
			curHitbox.setTopLeftY(movementManager.getMovementVect2().getY());
			if (curEntity.getClassType() == EEntityClassTypes_PROJECTILE)
			{
				killedCharacter(curEntity, instantDeath);
			}
		}
	}

}

void WorldData::collideWithPlatforms(Entity& curEntity)
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	MovementManager& movementManager = curEntity.getMovementManager();
	bool instantDeath = true;

	movementManager.setOnGroundFalse();

	std::vector <Entity*> platformEntities;
	pCurLevel->mStaticEntities.getEntitiesInHitbox(platformEntities, movementManager.getHitbox());

	for (Entity* pEntity : platformEntities)
	{
		bool check = true;
		Platform& curPlatform = *(Platform*)pEntity;
		Hitbox& curPlatformHitbox = curPlatform.getMovementManager().getHitbox();

		if (!curPlatform.mIsVisible or &curEntity == pEntity)
		{
			check = false;
		}

		if (check)
		{
			if (curEntity.getClassType() == EEntityClassTypes_PROJECTILE)
			{
				killedCharacter(curEntity, instantDeath);
				if (curPlatform.getCharacterType() == EEntityCharacterTypes_P_TARGET)
				{
					if (curEntity.getCharacterType() == EEntityCharacterTypes_PJ_PLAYER_PROJECTILE)
					{
						curPlatform.hide();
						mPlayer.updateTargets(1);
					}
				}
				return;
			}
			int doSeparate = false;
			EBoxSide platformSeparationPath = movementManager.getHitbox().separate(curPlatformHitbox, doSeparate);
			EBoxSide entitySeparationPath = curPlatformHitbox.separate(movementManager.getHitbox(), doSeparate);
			EEntityEdgeType platformEdgeType = curPlatform.getMovementManager().getEdgeType(platformSeparationPath);
			EEntityEdgeType entityEdgeType = movementManager.getEdgeType(entitySeparationPath);

			if (platformEdgeType == EEntityEdgeType_NON_EXISTENT and check)
			{
				if (! (movementManager.getPath() == EEntityMovementPath_VERTICAL or movementManager.getPath() == EEntityMovementPath_DIAGONAL))
				{
					check = false;
				}
			}
			if (check)
			{
				doSeparate = true;
				platformSeparationPath = movementManager.getHitbox().separate(curPlatformHitbox, doSeparate);
				doSeparate = false;
				entitySeparationPath = curPlatformHitbox.separate(movementManager.getHitbox(), doSeparate);
				platformEdgeType = curPlatform.getMovementManager().getEdgeType(platformSeparationPath);
				entityEdgeType = movementManager.getEdgeType(entitySeparationPath);

				spreadEdges(curPlatform, curEntity);

				if (willKillCharacter(curEntity, platformEdgeType))
				{
					killedCharacter(curEntity, !instantDeath);
					check = false;
				}

				if (platformSeparationPath == EBoxSide_TOP and check)
				{
					if (movementManager.getPath() == EEntityMovementPath_HORIZONTAL or movementManager.getPath() == EEntityMovementPath_HORIZONTAL_CAN_FALL)
					{
						movementManager.setOnGroundTrue(curPlatform.getMovementEffect(), curPlatform.getCurCharacteristics(), curPlatform.getMovementManager().getHitboxEdges().mTop);
						// cur platform fallen on
						((JumpingState*)movementManager.getMovementStates()[EMovementStateIndex_JUMPING])->endJump();
						movementManager.getMovementStates()[EMovementStateIndex_JUMPING]->landed();

						if (platformEdgeType == EEntityEdgeType_BOUNCY)
						{
							movementManager.collideWithBouncy();
						}
						else if (platformEdgeType == EEntityEdgeType_WEIGHT_SENSITIVE and curEntity.getCharacterType() == EEntityCharacterTypes_P_ARMORED_CRATE)
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
						mCollisionManager.entitiesCollidedVertical(curEntity, curPlatform);
						check = false;
					}
				}
				else if (platformSeparationPath == EBoxSide_BOTTOM and check)
				{
					if (movementManager.getPath() == EEntityMovementPath_VERTICAL or movementManager.getPath() == EEntityMovementPath_DIAGONAL)
					{
						mCollisionManager.entitiesCollidedVertical(curEntity, curPlatform);
					}
				}
				else if (check)
				{
					if (movementManager.getPath() == EEntityMovementPath_HORIZONTAL or movementManager.getPath() == EEntityMovementPath_HORIZONTAL_CAN_FALL or movementManager.getPath() == EEntityMovementPath_DIAGONAL)
					{
						mCollisionManager.entitiesCollidedHorizontal(curEntity, curPlatform);
						check = false;
					}
				}
			}
		}
	}
}

void WorldData::collideWithNonStaticPlatforms(Entity& curEntity)
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	bool instantDeath = true;
	bool createdRidingContact = false;

	MovementManager& movementManager = curEntity.getMovementManager();
	std::vector <Entity*> platformEntities;
	pCurLevel->mDynamicEntities.getEntitiesInHitbox(platformEntities, movementManager.getHitbox(), EEntityClassTypes_PLATFORM);

	for (Entity* pEntity : platformEntities)
	{
		Platform* pCurNonStaticPlatform = (Platform*)pEntity;
		bool check = true;
		if (&curEntity != pCurNonStaticPlatform)
		{
			if (curEntity.getClassType() == EEntityClassTypes_PROJECTILE)
			{
				if (pCurNonStaticPlatform->mVulnerableToProjectiles)
				{
					killedCharacter(*pCurNonStaticPlatform, instantDeath);
				}
				killedCharacter(curEntity, instantDeath);
				return;
			}
			else
			{
				bool doSeparate = false;
				EBoxSide separationPath = pCurNonStaticPlatform->getMovementManager().getHitbox().separate(movementManager.getHitbox(), doSeparate);
				EEntityEdgeType edgeType = pCurNonStaticPlatform->getMovementManager().getEdgeType(separationPath);

				spreadEdges(*pCurNonStaticPlatform, curEntity);
				if (willKillCharacter(curEntity, edgeType))
				{
					killedCharacter(curEntity, !instantDeath);
					check = false;
				}

				if (check)
				{
					if (separationPath == EBoxSide_BOTTOM)
					{
						movementManager.setOnGroundTrue(pCurNonStaticPlatform->getMovementEffect(), pCurNonStaticPlatform->getCurCharacteristics(), pCurNonStaticPlatform->getMovementManager().getHitboxEdges().mTop);
						movementManager.getMovementStates()[EMovementStateIndex_JUMPING]->landed();
					}
					else if (separationPath == EBoxSide_LEFT or separationPath == EBoxSide_RIGHT)
					{
						if (movementManager.getPath() == EEntityMovementPath_HORIZONTAL or movementManager.getPath() == EEntityMovementPath_HORIZONTAL_CAN_FALL)
						{
							mCollisionManager.entitiesCollidedHorizontal(curEntity, *pCurNonStaticPlatform);
							check = false;
						}
					}

					if (curEntity.getSubClassType() == EEntitySubClassTypes_CRATE)
					{
						doSeparate = false;
						EBoxSide		crateSeparationPath = movementManager.getHitbox().separate(pCurNonStaticPlatform->getMovementManager().getHitbox(), doSeparate);
						EEntityEdgeType crateEdgeType = movementManager.getEdgeType(crateSeparationPath);

						if (edgeType == EEntityEdgeType_MOVEABLE and crateEdgeType == EEntityEdgeType_MOVEABLE)
						{
							int movement = movementManager.getMovementVect2().getX();
							if (movementManager.getCurDirection() == EDirection_LEFT)
							{
								movement *= -1;
							}
							pCurNonStaticPlatform->getMovementManager().getHitbox().updateTopLeftX(movement);
							mCollisionManager.mCollisionsToSeparate.push_back(Collision(curEntity, *pCurNonStaticPlatform));
							mCollisionManager.addCrateContact(curEntity);
							mCollisionManager.addCrateContact(*pCurNonStaticPlatform);

							doSeparate = true;
							pCurNonStaticPlatform->getMovementManager().getHitbox().separate(movementManager.getHitbox(), doSeparate);
						}
						else
						{
							if (separationPath == EBoxSide_BOTTOM)
							{
								// crate on crate or crate on Moving platform 
								createdRidingContact = true;
								mCollisionManager.addRidingContact(curEntity, *pCurNonStaticPlatform);
							}
							else if ((pCurNonStaticPlatform->getSubClassType() == EEntitySubClassTypes_CRATE) and (separationPath == EBoxSide_TOP))
							{
								createdRidingContact = true;
								mCollisionManager.addRidingContact(*pCurNonStaticPlatform, curEntity);
							}
							else
							{
								doSeparate = true;
								movementManager.getHitbox().separate(pCurNonStaticPlatform->getMovementManager().getHitbox(), doSeparate);
							}
						}
					}
					else if (curEntity.getCharacterType() == EEntityCharacterTypes_P_MOVING_PLATFORM)
					{
						if (pCurNonStaticPlatform->getSubClassType() == EEntitySubClassTypes_CRATE)
						{
							if (separationPath == EBoxSide_TOP)
							{
								// crate on crate or crate on Moving platform
								createdRidingContact = true;
								mCollisionManager.addRidingContact(*pCurNonStaticPlatform, curEntity);
							}
						}
					}
					else if (curEntity.getClassType() == EEntityClassTypes_ENEMY)
					{
						if (separationPath == EBoxSide_TOP or separationPath == EBoxSide_BOTTOM)
						{
							// pass
						}
						else
						{
							mCollisionManager.entitiesCollidedHorizontal(curEntity, *pCurNonStaticPlatform);
							return;
						}
					}
					if (!createdRidingContact)
					{
						bool doSeparate = true;
						EBoxSide separationPath = pCurNonStaticPlatform->getMovementManager().getHitbox().separate(movementManager.getHitbox(), doSeparate);
					}
					if ((separationPath == EBoxSide_TOP or separationPath == EBoxSide_BOTTOM) and !createdRidingContact)
					{
						mCollisionManager.entitiesCollidedVertical(curEntity, *pCurNonStaticPlatform);
						return;
					}
				}
			}
		}
	}
}

void WorldData::collideWithEnemies(Entity& curEntity)
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	bool instantDeath = true;

	MovementManager& movementManager = curEntity.getMovementManager();

	std::vector <Entity*> enemiesEntities;
	pCurLevel->mDynamicEntities.getEntitiesInHitbox(enemiesEntities, movementManager.getHitbox(), EEntityClassTypes_ENEMY);


	for (Entity* pEntity : enemiesEntities)
	{
		Enemy* pCurEnemy = (Enemy*)pEntity;
		bool check = true;
		if (&curEntity != pCurEnemy)
		{
			if (curEntity.getClassType() == EEntityClassTypes_PROJECTILE)
			{
				Projectile& curProjectile = (Projectile&)curEntity;
				std::string hostName = curProjectile.getHostName();
				if (pCurEnemy->shouldTakeDamageFromProjectile(hostName))
				{
					killedCharacter(*pCurEnemy, instantDeath);
					killedCharacter(curEntity, instantDeath);
					return;
				}
			}
			else
			{
				bool doSeparate = false;

				EBoxSide separationPath = movementManager.getHitbox().separate(pCurEnemy->getMovementManager().getHitbox(), doSeparate);
				if (separationPath == EBoxSide_TOP)
				{
					movementManager.setOnGroundTrue(pCurEnemy->getMovementEffect(), pCurEnemy->getCurCharacteristics(), pCurEnemy->getMovementManager().getHitboxEdges().mTop);
					movementManager.getMovementStates()[EMovementStateIndex_JUMPING]->landed();
				}

				if (curEntity.getSubClassType() == EEntitySubClassTypes_CRATE)
				{
					if (separationPath == EBoxSide_TOP)
					{
						// CRATE ON ENEMY
						EEntityEdgeType crateEdgeType = movementManager.getHitboxEdges().mBottom;
						EEntityEdgeType enemyEdgeType = pCurEnemy->getMovementManager().getHitboxEdges().mTop;

						if (willKillCharacter(*pCurEnemy, crateEdgeType))
						{
							killedCharacter(*pCurEnemy, !instantDeath);
							check = false;
						}
						if (willKillCharacter(curEntity, enemyEdgeType))
						{
							killedCharacter(curEntity, !instantDeath);
							check = false;
						}

						if (pCurEnemy->mRideable)
						{
							mCollisionManager.addRidingContact(curEntity, *pCurEnemy);
							check = false;
						}
						else if (enemyEdgeType == EEntityEdgeType_BOUNCY)
						{
							movementManager.collideWithBouncy();
							killedCharacter(*pCurEnemy, instantDeath);
						}
					}
					else if (separationPath == EBoxSide_BOTTOM)
					{
						// ENEMY ON CRATE
						mCollisionManager.addRidingContact(*pCurEnemy, curEntity);
					}
					else
					{
						// ENEMY ON SIDE OF CRATE
						EEntityEdgeType enemyEdgeType = pCurEnemy->getMovementManager().getEdgeType(separationPath);
						if (enemyEdgeType == EEntityEdgeType_GRABBING)
						{
							if (pCurEnemy->mHasAttachmentPoint and pCurEnemy->mRideable and pCurEnemy->getAmAlive())
							{
								EDirection crateDirection = EDirection_RIGHT;
								if (movementManager.getHitbox().getCenter().getX() < pCurEnemy->getMovementManager().getHitbox().getCenter().getX())
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
									int y = pCurEnemy->getMovementManager().getHitbox().getTopLeft().getY() + pCurEnemy->mAttachmentPoint.getY() - movementManager.getHitbox().getHeight();

									pCurEnemy->setNextAnimationToPlay(EAnimationType_GRABBING);
									movementManager.getHitbox().setTopLeft(Vect2(x, y));
									mCollisionManager.addRidingContact(curEntity, *pCurEnemy);
								}
							}
						}
						else
						{
							mCollisionManager.entitiesCollidedHorizontal(curEntity, *pCurEnemy);
						}
						check = false;
					}
				}
				else if (curEntity.getClassType() == EEntityClassTypes_ENEMY)
				{
					doSeparate = false;
					separationPath = movementManager.getHitbox().separate(pCurEnemy->getMovementManager().getHitbox(), doSeparate);


					if (separationPath == EBoxSide_TOP)
					{
						if (movementManager.getPath() == EEntityMovementPath_HORIZONTAL)
						{
							// pass
						}
						else
						{
							mCollisionManager.entitiesCollidedVertical(*pCurEnemy, curEntity);
							check = false;
						}
					}
					else
					{
						doSeparate = true;
						separationPath = movementManager.getHitbox().separate(pCurEnemy->getMovementManager().getHitbox(), doSeparate);
						mCollisionManager.entitiesCollidedHorizontal(curEntity, *pCurEnemy);
						check = false;
					}
				}
				else
				{
					mCollisionManager.entitiesCollidedHorizontal(curEntity, *pCurEnemy);
					check = false;
				}
			}
		}
	}
}

void WorldData::checkIfOnEdgeOfPlatform(Enemy& curEnemy)
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	MovementManager& movementManager = curEnemy.getMovementManager();
	if (movementManager.getPath() == EEntityMovementPath_HORIZONTAL or movementManager.getPath() == EEntityMovementPath_VERTICAL)
	{
		return;
	}
	if (movementManager.getMovementCode() == EEntityMovements_JUMP)
	{
		return;
	}

	std::vector <Entity*> platformEntities;
	pCurLevel->mStaticEntities.getEntitiesInHitbox(platformEntities, movementManager.getHitbox());
	pCurLevel->mDynamicEntities.getEntitiesInHitbox(platformEntities, movementManager.getHitbox(), EEntityClassTypes_PLATFORM);

	for (Entity* pEntity : platformEntities)
	{
		Platform& curPlatform = *(Platform*)pEntity;
		Hitbox& curPlatformHitbox = curPlatform.getMovementManager().getHitbox();

		int curEnemyY2 = movementManager.getHitbox().getBottomRight().getY();
		int curPlatformY1 = curPlatformHitbox.getTopLeft().getY();
		// make sure the enemy is on the platform we are testing against
		if (std::abs(curPlatformY1 - curEnemyY2) <= 5)
		{

			int curHeight = movementManager.getHitbox().getHeight();
			Vect2 leftHitboxVect2 = Vect2(curPlatformHitbox.getTopLeft().getX(), curPlatformHitbox.getTopLeft().getY() - curHeight);
			Hitbox platformHitboxLeft = Hitbox(leftHitboxVect2, movementManager.getMovementVect2().getX(), curHeight);
			Vect2 rightHitboxVect2 = Vect2(curPlatformHitbox.getBottomRight().getX() - movementManager.getMovementVect2().getX(), curPlatformHitbox.getTopLeft().getY() - curHeight);
			Hitbox platformHitboxRight = Hitbox(rightHitboxVect2, movementManager.getMovementVect2().getX(), curHeight);

			bool collide = false;

			EDirection curDirection = EDirection_INVALID;
			EDirection directionToSetTo = EDirection_INVALID;
			bool doSeparate = true;
			if (platformHitboxLeft.overlap(movementManager.getHitbox()))
			{
				collide = true;
				curDirection = EDirection_LEFT;
				directionToSetTo = EDirection_RIGHT;
				movementManager.getHitbox().separate(platformHitboxLeft, doSeparate);
			}
			else if (platformHitboxRight.overlap(movementManager.getHitbox()))
			{
				collide = true;
				curDirection = EDirection_RIGHT;
				directionToSetTo = EDirection_LEFT;
				movementManager.getHitbox().separate(platformHitboxRight, doSeparate);
			}

			if (collide)
			{
				if (!mCollisionManager.isInThisFrameCollisions(curEnemy))
				{
					mCollisionManager.mThisFrameCollisions.push_back(Collision(curEnemy, curPlatform));
					if (movementManager.getCurDirection() != directionToSetTo)
					{
						movementManager.collided(curDirection);
						if (movementManager.getDidSwitchedDir())
						{
							curEnemy.setTrapped();
						}
					}
				}
				return;
			}
		}
	}
}

void WorldData::collectedCollectible(Collectible& collectible)
{
	collectible.setAmPickedUp(true);
	CanGoToNextLevelResults results = canGoToNextLevel();;
	switch (collectible.getCharacterType())
	{
	case EEntityCharacterTypes_C_KEY:
		mPlayer.updateKeys(1);
		break;
	case EEntityCharacterTypes_C_SAVE_POINT:
		saveInGameStats();
		break;
	case EEntityCharacterTypes_C_LOTUS_COLLECTIBLE:
		mPlayer.addHeldCollectible(&collectible);
		break;
	case EEntityCharacterTypes_C_END_OF_LEVEL:
		if (results.mCanGoToNextLevel)
		{
			mpNextLevelData = results.mpNextLevelData;
			mGoToNextLevel = true;
		}
		else
		{
			collectible.setAmPickedUp(false);
		}
		break;
	case EEntityCharacterTypes_C_MINI_GAME_LEVEL:
		mpNextLevelData = &((MiniGameLevelCollectible&)collectible).mNextLevelData;
		mGoToNextLevel = true;
		break;
	default:
		SDL_assert(false);
		break;
	}

}

// new collision system
#elif COLLISION_SYSTEM == 1

void WorldData::entityCollisions()
{
	updateCurLevelChunk();
	updateNonstaticCollisions();
	slashCollisions();
}

void WorldData::collideWithWorld(Entity& curEntity)
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	MovementManager& movementManager = curEntity.getMovementManager();
	Hitbox& curHitbox = movementManager.getHitbox();
	bool instantDeath = true;
	bool isHorizontalPath = movementManager.getPath() == EEntityMovementPath_HORIZONTAL or movementManager.getPath() == EEntityMovementPath_HORIZONTAL_CAN_FALL;
	if (curHitbox.getTopLeft().getX() <= 0)
	{
		int updatedX = movementManager.getMovementVect2().getX();
		curHitbox.setTopLeftX(updatedX);
		if (isHorizontalPath)
		{
			movementManager.collided(EDirection_LEFT);
			if (movementManager.getDidSwitchedDir())
			{
				curEntity.setTrapped();
			}
		}
		if (curEntity.getClassType() == EEntityClassTypes_PROJECTILE)
		{
			killedCharacter(curEntity, instantDeath);
		}
	}
	else if (curHitbox.getBottomRight().getX() >= pCurLevel->mLevelX2)
	{
		curHitbox.setTopLeftX(pCurLevel->mLevelX2 - curHitbox.getWidth() - movementManager.getMovementVect2().getX());
		if (isHorizontalPath)
		{
			movementManager.collided(EDirection_RIGHT);
			if (movementManager.getDidSwitchedDir())
			{
				curEntity.setTrapped();
			}
		}
		if (curEntity.getClassType() == EEntityClassTypes_PROJECTILE)
		{
			killedCharacter(curEntity, instantDeath);
		}
	}

	bool isVerticalPath = movementManager.getPath() == EEntityMovementPath_VERTICAL;
	if (curHitbox.getTopLeft().getY() <= 0)
	{
		curHitbox.setTopLeftY(movementManager.getMovementVect2().getY());
		if (isVerticalPath)
		{
			movementManager.collided(EDirection_UP);

		}
		if (curEntity.getClassType() == EEntityClassTypes_PROJECTILE)
		{
			killedCharacter(curEntity, instantDeath);
		}
	}
	else if (curHitbox.getBottomRight().getY() >= pCurLevel->mLevelY2)
	{
		curHitbox.setTopLeftY(pCurLevel->mLevelY2 - curHitbox.getHeight() - movementManager.getMovementVect2().getY());
		if (isVerticalPath)
		{
			movementManager.collided(EDirection_DOWN);
		}
		else
		{
			// kill character
			curHitbox.setTopLeftY(movementManager.getMovementVect2().getY());
			if (curEntity.getClassType() == EEntityClassTypes_PROJECTILE)
			{
				killedCharacter(curEntity, instantDeath);
			}
		}
	}

}

/*
* Go through all entities, mark ideal movement. Then run through a loop to test that movement and apply actual movement. Store the movement and whether it was interupted
* If any are interupted then run through loop again
*/
void WorldData::updateNonstaticCollisions()
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	
	// get all nonstatic in level
	std::vector <Entity*> pNonstaticEntities;
	getAllDynamicEntities(pNonstaticEntities, pCurLevel->getHitbox());

	bool interrupted = true;
	int loop = 0;
	while (loop < 5 && interrupted) // TODO make dynamic based on frame rate
	{ 
		interrupted = false;
		// calculate where all nonstatic can actually move to
		runNonstaticCollisions(pNonstaticEntities, interrupted);

		// move everything
		updateNonstaticMovement(pNonstaticEntities);
		
		// update effects
		bool endLoop = updateNonstaticCollisionEffects(pNonstaticEntities);
		if (endLoop) { break; }
		
		loop++;
	}
}

void WorldData::runNonstaticCollisions(std::vector<Entity*>& pNonstaticEntities, bool& interrupted) 
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];

	for (Entity* pCurEntity : pNonstaticEntities)
	{
		MovementManager& curMovementManager	= pCurEntity->getMovementManager();
		Vect2			 curVect2			= curMovementManager.getMovementVect2();
		Hitbox& curHitbox					= curMovementManager.getHitbox();
		AttemptMove& attemptMove			= curMovementManager.getAttemptMove();
		Hitbox curTestMovement				= Hitbox(attemptMove.mWantToMoveTo, curHitbox.getWidth(), curHitbox.getHeight());
		attemptMove.mMoveTo					= attemptMove.mWantToMoveTo;


		// get all entities in this current entity's hitbox
		std::vector <Entity*> pOtherEntities;
		getAllDynamicEntities(pOtherEntities, curTestMovement);
		getAllStaticEntities(pOtherEntities, curTestMovement);

		// determine how to resolve the collision
		for (int i = (int)pOtherEntities.size() - 1; i > -1; i--) 
		{
			Entity& otherEntity = *pOtherEntities[i];

			if (!otherEntity.getAmAlive() or !otherEntity.mIsVisible or pCurEntity == &otherEntity)
			{
				pOtherEntities.erase(pOtherEntities.begin() + i);
				continue;
			}
			MovementManager& otherMovementManager	= otherEntity.getMovementManager();
			Hitbox&			 otherHitbox			= otherMovementManager.getHitbox();
			AttemptMove&	 otherMove				= otherMovementManager.getAttemptMove();
			Hitbox			 otherTestMovement		= Hitbox(otherMove.mWantToMoveTo, otherHitbox.getWidth(), otherHitbox.getHeight());
			
			if (curTestMovement.overlap(otherTestMovement))
			{
				// TODO move based on ratio between this object's movement and colliding object's movement
				Vect2 movement		= curMovementManager.getMovementVect2();
				Vect2 otherMovement = otherMovementManager.getMovementVect2();
				
				bool doSeparate = false;
				EBoxSide otherSeparationPath	= curTestMovement.separate(otherTestMovement, doSeparate);
				EEntityEdgeType otherEdgeType	= otherMovementManager.getEdgeType(otherSeparationPath);
				if (otherEdgeType == EEntityEdgeType_NON_EXISTENT)
				{
					if		(!(curMovementManager.getPath() == EEntityMovementPath_VERTICAL
						or	   curMovementManager.getPath() == EEntityMovementPath_DIAGONAL))
					{
						continue; // non existent edge, pretend there's no collision
					}
				}
				if		((pCurEntity->getClassType()	== EEntityClassTypes_PROJECTILE && ((Projectile*)pCurEntity)->getHostName()	== otherEntity.mName) 
					|| (otherEntity.getClassType()		== EEntityClassTypes_PROJECTILE && ((Projectile&)otherEntity).getHostName()	== pCurEntity->mName)) {
					
					continue; // projectile colliding with its spawner, pretend there's no collision
				}

				
				// TODO move based on ratio between this object's movement and colliding object's movement
				curTestMovement.separate(otherTestMovement, true);
				movement = curTestMovement.getCenter() - curHitbox.getCenter();

				// Force resolution to be along the colliding character's path
				int increment = 0;
				switch (curMovementManager.getPath())
				{
				case EEntityMovementPath_HORIZONTAL:
					movement.setY(0);
					break;
				case EEntityMovementPath_VERTICAL:
					movement.setX(0);
					break;
				case EEntityMovementPath_DIAGONAL:
					increment = (int)std::max(std::ceil(movement.getX() / curVect2.getX()), std::ceil(movement.getY() / curVect2.getY()));
					movement.setX(increment * curVect2.getX()); // move in increments of the diagonal path
					movement.setY(increment * curVect2.getY());
					break;
				default:
					break;
				}

				// mark collision
				mCollisionManager.addCollision(*pCurEntity, otherEntity);
				attemptMove.mInterrupted = true;
				interrupted = true;
				attemptMove.mMoveTo = curHitbox.getTopLeft() + movement;
			}
		}

	}
}

void WorldData::updateNonstaticMovement(std::vector<Entity*>& pNonstaticEntities)
{
	for (Entity* pCurEntity : pNonstaticEntities)
	{
		pCurEntity->getMovementManager().move();
	}
}

bool WorldData::updateNonstaticCollisionEffects(std::vector<Entity*>& pNonstaticEntities)
{
	for (Entity* pEntity : pNonstaticEntities)
	{
		pEntity->getMovementManager().setOnGroundFalse();
	}
	mPlayer.mCanWallJump = false;

	bool instantDeath = true;
	for (Collision& collision : mCollisionManager.mThisFrameCollisions)
	{
		
		if (collision.mEntity1.getClassType() != EEntityClassTypes_PROJECTILE and collision.mEntity2.getClassType() != EEntityClassTypes_PROJECTILE) 
		{
			bool doSeparate = false;
			EBoxSide separationPath			= collision.mEntity2.getMovementManager().getHitbox().separate(collision.mEntity1.getMovementManager().getHitbox(), doSeparate);
			EBoxSide otherSeparationPath	= collision.mEntity1.getMovementManager().getHitbox().separate(collision.mEntity2.getMovementManager().getHitbox(), doSeparate);
			EEntityEdgeType otherEdgeType	= collision.mEntity1.getMovementManager().getEdgeType(separationPath);
			EEntityEdgeType edgeType		= collision.mEntity2.getMovementManager().getEdgeType(otherSeparationPath);

			spreadEdges(collision.mEntity2, collision.mEntity1);
			spreadEdges(collision.mEntity1, collision.mEntity2);
			if (willKillCharacter(collision.mEntity1, edgeType))
			{
				killedCharacter(collision.mEntity1);
			}
			if (willKillCharacter(collision.mEntity2, otherEdgeType))
			{
				killedCharacter(collision.mEntity2);
			}
		}
		if (!collision.mEntity1.getAmAlive() or !collision.mEntity2.getAmAlive())
		{
			continue; // one of these entities is dead
		}
		bool endLoop = false;
		switch (collision.mEntity2.getClassType())
		{
		case EEntityClassTypes_PLATFORM:
			if (collision.mEntity2.getMovementManager().getCurMode() == ECharacterModes_STATIC) {
				collideWithPlatform(collision);
			}
			else
			{
				collideWithNonStaticPlatform(collision);
			}
			break;
		case EEntityClassTypes_ENEMY:
			collideWithEnemy(collision);
			
			break;
		case EEntityClassTypes_PROJECTILE:
			collideWithProjectile(collision);
			break;
		case EEntityClassTypes_PLAYER:
			collideWithPlayer(collision);
			break;
		case EEntityClassTypes_COLLECTIBLE:
			endLoop = collectedCollectible((Collectible&)collision.mEntity2);
			if (endLoop) { return endLoop;  }
			break;
		default:
			SDL_assert(false);
			break;
		}

		Collision reverseCollision = Collision(collision.mEntity2, collision.mEntity1);
		switch (reverseCollision.mEntity2.getClassType())
		{
		case EEntityClassTypes_PLATFORM:
			if (reverseCollision.mEntity2.getMovementManager().getCurMode() == ECharacterModes_STATIC) {
				collideWithPlatform(reverseCollision);
			}
			else
			{
				collideWithNonStaticPlatform(reverseCollision);
			}
			break;
		case EEntityClassTypes_ENEMY:
			collideWithEnemy(reverseCollision);
			break;
		case EEntityClassTypes_PROJECTILE:
			collideWithProjectile(reverseCollision);
			break;
		case EEntityClassTypes_PLAYER:
			collideWithPlayer(reverseCollision);
			break;
		case EEntityClassTypes_COLLECTIBLE:
			
			break;
		default:
			SDL_assert(false);
			break;
		}
	}

	for (Entity* pEntity : pNonstaticEntities)
	{
		collideWithWorld(*pEntity);
	}

	return false;
}

// TODO assumes mpEntity2 is a player
void WorldData::collideWithPlayer(Collision& collision) { return; }

// assumes mpEntity2 is a platform
void WorldData::collideWithPlatform(Collision& collision)
{
	bool instantDeath = true;
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];

	
	MovementManager& entity1MovementManager = collision.mEntity1.getMovementManager();
	MovementManager& entity2MovementManager = collision.mEntity2.getMovementManager();

	bool doSeparate = false;
	EBoxSide separationPath = entity2MovementManager.getHitbox().separate(entity1MovementManager.getHitbox(), doSeparate);
	EEntityEdgeType edgeType = entity2MovementManager.getEdgeType(separationPath);


	switch (separationPath) 
	{
	case EBoxSide_TOP:
		// PLATFORM on ENTITY
		if (entity1MovementManager.getPath() == EEntityMovementPath_VERTICAL
			or entity1MovementManager.getPath() == EEntityMovementPath_DIAGONAL)
		{
			mCollisionManager.entitiesCollidedVertical(collision.mEntity1, collision.mEntity2);
		}
		break;

	case EBoxSide_BOTTOM:
			
		// ENTITY on PLATFORM
		if (entity1MovementManager.getPath() == EEntityMovementPath_HORIZONTAL
			or entity1MovementManager.getPath() == EEntityMovementPath_HORIZONTAL_CAN_FALL)
		{
			JumpingState* jumpState = (JumpingState*)(entity1MovementManager.getMovementStates()[EMovementStateIndex_JUMPING]);
			if (jumpState->isOver())
			{
				// cur platform fallen on
				entity1MovementManager.setOnGroundTrue(collision.mEntity2.getMovementEffect(), collision.mEntity2.getCurCharacteristics(), entity2MovementManager.getHitboxEdges().mTop);
				jumpState->endJump();
				jumpState->landed();
			}

			EEntityEdgeType platformTop = entity2MovementManager.getEdgeType(EBoxSide_TOP);

			if (platformTop == EEntityEdgeType_BOUNCY)
			{
				entity1MovementManager.collideWithBouncy();
			}
			else if (platformTop == EEntityEdgeType_WEIGHT_SENSITIVE and collision.mEntity1.getCharacterType() == EEntityCharacterTypes_P_ARMORED_CRATE)
			{
				int codeNumber = ((Platform&)collision.mEntity2).mCodeNumber;
				((Platform&)collision.mEntity2).activate();
				for (Platform* pCurPossibleGate : pCurLevel->mpPlatforms)
				{
					if (pCurPossibleGate->getCharacterType() == EEntityCharacterTypes_P_PRESSURE_OPERATED_GATE and pCurPossibleGate->mCodeNumber == codeNumber)
					{
						pCurPossibleGate->hide();
					}
				}
			}
			if (platformTop == EEntityEdgeType_CRUMBLING && collision.mEntity1.getClassType() == EEntityClassTypes_PLAYER)
			{
				((Platform&)collision.mEntity2).startCrumble();
			}
		}
		else
		{
			mCollisionManager.entitiesCollidedVertical(collision.mEntity1, collision.mEntity2);
		}
		break;

	case EBoxSide_LEFT:
	case EBoxSide_RIGHT:
			
		if (collision.mEntity1.getClassType() == EEntityClassTypes_PLAYER) {
			if (edgeType == EEntityEdgeType_WALL_JUMPABLE)
			{
				if (!mPlayer.getMovementManager().isOnGround())
				{
					mPlayer.mCanWallJump = true;
				}
			}
			else if (collision.mEntity2.getCharacterType() == EEntityCharacterTypes_P_GATE) {
				{
					if (mPlayer.mKeys > 0)
					{
						((Gate&)collision.mEntity2).hide();
						mPlayer.updateKeys(-1);
					}
				}
			}
			else if (collision.mEntity2.getCharacterType() == EEntityCharacterTypes_P_TARGET_GATE)
			{
				if (mPlayer.mTargets > 0)
				{
					((Gate&)collision.mEntity2).hide();
					mPlayer.updateTargets(-1);
				}
			}
		}
		if		(entity1MovementManager.getPath() == EEntityMovementPath_HORIZONTAL
				or entity1MovementManager.getPath() == EEntityMovementPath_HORIZONTAL_CAN_FALL
				or entity1MovementManager.getPath() == EEntityMovementPath_DIAGONAL)
		{
			mCollisionManager.entitiesCollidedHorizontal(collision.mEntity1, collision.mEntity2);
		}

		break;
	default:
		break;
	}
}

// assumes mpEntity2 is a nonstatic platform
void WorldData::collideWithNonStaticPlatform(Collision& collision)
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	bool instantDeath = true;
	bool createdRidingContact = false;
	
	MovementManager& entity1MovementManager = collision.mEntity1.getMovementManager();
	MovementManager& entity2MovementManager = collision.mEntity2.getMovementManager();

	bool doSeparate = false;
	EBoxSide separationPath  = entity2MovementManager.getHitbox().separate(entity1MovementManager.getHitbox(), doSeparate);
	EEntityEdgeType edgeType = entity2MovementManager.getEdgeType(separationPath);

	switch (separationPath) 
	{		
	case EBoxSide_TOP:
		// ENTITY on NONSTATIC
		entity1MovementManager.setOnGroundTrue(collision.mEntity2.getMovementEffect(), collision.mEntity2.getCurCharacteristics(), entity2MovementManager.getHitboxEdges().mTop);
		entity1MovementManager.getMovementStates()[EMovementStateIndex_JUMPING]->landed();

			
		if (collision.mEntity1.getType() == EEntityType_NON_STATIC)
		{
			if (collision.mEntity1.mRideable)
			{
				createdRidingContact = true;
				mCollisionManager.addRidingContact(collision.mEntity1, collision.mEntity2);
			}
			else
			{
				killedCharacter(collision.mEntity1);
			}
		}
			
		break;
	case EBoxSide_BOTTOM:
		entity1MovementManager.setOnGroundTrue(collision.mEntity2.getMovementEffect(), collision.mEntity2.getCurCharacteristics(), entity2MovementManager.getHitboxEdges().mTop);
		entity1MovementManager.getMovementStates()[EMovementStateIndex_JUMPING]->landed();

		if (collision.mEntity1.getType() == EEntityType_NON_STATIC)
		{
			// nonstatic on nonstatic
			createdRidingContact = true;
			mCollisionManager.addRidingContact(collision.mEntity1, collision.mEntity2);
		}
		break;

	case EBoxSide_LEFT:
	case EBoxSide_RIGHT:
		if (collision.mEntity1.getClassType() == EEntityClassTypes_PLAYER && edgeType == EEntityEdgeType_WALL_JUMPABLE)
		{
			if (!mPlayer.getMovementManager().isOnGround())
			{
				mPlayer.mCanWallJump = true;
			}
		}
		if (collision.mEntity1.getSubClassType() == EEntitySubClassTypes_CRATE or collision.mEntity1.getClassType() == EEntityClassTypes_PLAYER)
		{
			// CRATE into CRATE or PLAYER into CRATE
			mCollisionManager.mCollisionsToSeparate.push_back(collision);
			mCollisionManager.addCrateContact(collision.mEntity2);
			if (collision.mEntity1.getSubClassType() == EEntitySubClassTypes_CRATE) 
			{
				mCollisionManager.addCrateContact(collision.mEntity1);
			}
			break;
		}
		if (	entity1MovementManager.getPath() == EEntityMovementPath_HORIZONTAL
			or  entity1MovementManager.getPath() == EEntityMovementPath_HORIZONTAL_CAN_FALL)
		{
			mCollisionManager.entitiesCollidedHorizontal(collision.mEntity1, collision.mEntity2);
		}
		break;

	default:
		break;
	}
	

	if ((separationPath == EBoxSide_TOP or separationPath == EBoxSide_BOTTOM) and !createdRidingContact)
	{
		mCollisionManager.entitiesCollidedVertical(collision.mEntity1, collision.mEntity2);
		return;
	}
}

// asumes mpEntity2 is an enemy
void WorldData::collideWithEnemy(Collision& collision)
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	bool instantDeath = true;

	MovementManager& entity1MovementManager = collision.mEntity1.getMovementManager();
	MovementManager& entity2MovementManager = collision.mEntity2.getMovementManager();

	bool doSeparate = false;	
	EBoxSide separationPath = entity1MovementManager.getHitbox().separate(entity2MovementManager.getHitbox(), doSeparate);
	EEntityEdgeType edgeType = entity2MovementManager.getEdgeType(separationPath);

	switch (separationPath) {
	case EBoxSide_TOP:
		// ENTITY on ENEMY
		entity1MovementManager.setOnGroundTrue(collision.mEntity2.getMovementEffect(), collision.mEntity2.getCurCharacteristics(), entity2MovementManager.getHitboxEdges().mTop);
		entity1MovementManager.getMovementStates()[EMovementStateIndex_JUMPING]->landed();

		if (collision.mEntity1.getType() == EEntityType_NON_STATIC)
		{
			if (collision.mEntity2.mRideable)
			{
				mCollisionManager.addRidingContact(collision.mEntity1, collision.mEntity2);
			}
			else if (entity2MovementManager.getHitboxEdges().mTop == EEntityEdgeType_BOUNCY)
			{
				entity1MovementManager.collideWithBouncy();
				killedCharacter(collision.mEntity2, instantDeath);
			}
		}
		
		if (collision.mEntity1.getClassType() == EEntityClassTypes_ENEMY)
		{
			// ENEMY ON ENEMY
			if (entity1MovementManager.getPath() != EEntityMovementPath_HORIZONTAL)
			{
				mCollisionManager.entitiesCollidedVertical(collision.mEntity2, collision.mEntity1);
			}
		}
		
		break;

	case EBoxSide_BOTTOM:
		// ENEMY On ENTITY
		entity2MovementManager.setOnGroundTrue(collision.mEntity1.getMovementEffect(), collision.mEntity1.getCurCharacteristics(), entity1MovementManager.getHitboxEdges().mTop);
		entity2MovementManager.getMovementStates()[EMovementStateIndex_JUMPING]->landed();
		if (collision.mEntity1.getType() == EEntityType_NON_STATIC)
		{
			// ENEMY ON NONSTATIC
			mCollisionManager.addRidingContact(collision.mEntity2, collision.mEntity1);
		}
		checkIfOnEdgeOfPlatform(collision);

		break;

	case EBoxSide_LEFT:
	case EBoxSide_RIGHT:
		if (collision.mEntity1.getClassType() == EEntityClassTypes_PLAYER and edgeType == EEntityEdgeType_WALL_JUMPABLE and !mPlayer.getMovementManager().isOnGround())
		{
			mPlayer.mCanWallJump = true;
		}
		if (collision.mEntity1.getType() == EEntityType_NON_STATIC)
		{
			// ENEMY ON SIDE OF NONSTATIC
			EEntityEdgeType enemySideEdgeType = entity2MovementManager.getEdgeType(separationPath);
			if (enemySideEdgeType == EEntityEdgeType_GRABBING)
			{
				if (collision.mEntity2.mHasAttachmentPoint and collision.mEntity2.mRideable and collision.mEntity2.getAmAlive())
				{
					EDirection nonstaticDirection = EDirection_RIGHT;
					if (entity1MovementManager.getHitbox().getCenter().getX() < entity2MovementManager.getHitbox().getCenter().getX())
					{
						nonstaticDirection = EDirection_LEFT;
					}

					if (nonstaticDirection == entity2MovementManager.getCurFacingDirection())
					{
						int x = entity2MovementManager.getHitbox().getTopLeft().getX();
						int attach = collision.mEntity2.mAttachmentPoint.getX();

						if (entity2MovementManager.getCurDirection() == EDirection_LEFT)
						{
							x += attach;
						}
						else
						{
							x -= attach;
						}
						int y = entity2MovementManager.getHitbox().getTopLeft().getY() + collision.mEntity2.mAttachmentPoint.getY() - entity1MovementManager.getHitbox().getHeight();

						((Enemy&)collision.mEntity2).setNextAnimationToPlay(EAnimationType_GRABBING);
						entity1MovementManager.getHitbox().setTopLeft(Vect2(x, y));
						mCollisionManager.addRidingContact(collision.mEntity1, collision.mEntity2);
						return;
					}
					// else collide
				}
			}
		}	
		mCollisionManager.entitiesCollidedHorizontal(collision.mEntity1, collision.mEntity2);
		break;
	default:
		break;
	}
}

// assumes mpEntity2 is a projectile
void WorldData::collideWithProjectile(Collision& collision) 
{
	bool instantDeath = true;
	if (collision.mEntity1.mName == ((Projectile&)collision.mEntity2).getHostName()) 
	{
		return;
	}
	killedCharacter(collision.mEntity1, instantDeath);
	killedCharacter(collision.mEntity2, instantDeath);
	if (collision.mEntity1.getCharacterType() == EEntityCharacterTypes_P_TARGET and collision.mEntity2.getCharacterType() == EEntityCharacterTypes_PJ_PLAYER_PROJECTILE)
	{
		((Platform&)collision.mEntity1).hide();
		mPlayer.updateTargets(1);
	}
}

void WorldData::slashCollisions() 
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];

	if (mSlashManager.mCurSlash)
	{
		std::vector<EntityDistance> struckEntities;
		int closestBlockingDistance = INT_MAX;
		std::vector<Entity*> pEntities = pCurLevel->getAllActiveEntities();

		int hitboxX1 = std::min(mPlayer.getMovementManager().getHitbox().getTopLeft().getX(), mSlashManager.mHitbox.getTopLeft().getX());
		int hitboxX2 = std::max(mPlayer.getMovementManager().getHitbox().getBottomRight().getX(), mSlashManager.mHitbox.getBottomRight().getX());
		int hitboxY1 = mSlashManager.mHitbox.getTopLeft().getY();
		int hitboxY2 = mSlashManager.mHitbox.getBottomRight().getY();
		Hitbox approxSlashHitbox = Hitbox(hitboxX1, hitboxX2, hitboxY1, hitboxY2);

		for (Entity* pEntity : pEntities)
		{
			if (pEntity->getAmAlive() and pEntity->getMovementManager().getHitbox().overlap(approxSlashHitbox))
			{
				if (pEntity->mVulnerableToProjectiles)
				{
					struckEntities.push_back(EntityDistance(*pEntity, getEntityDistance(*pEntity, mPlayer, approxSlashHitbox, mSlashManager.mCurRotation)));
				}
				else
				{
					int curBlockingDistance = getEntityDistance(*pEntity, mPlayer, approxSlashHitbox, mSlashManager.mCurRotation);
					if (curBlockingDistance < closestBlockingDistance)
					{
						closestBlockingDistance = curBlockingDistance;
					}
				}
			}
		}

		for (EntityDistance& curEntityDistance : struckEntities)
		{
			if (curEntityDistance.mDistance < closestBlockingDistance)
			{
				// will be hit
				bool instantDeath = true;
				killedCharacter(curEntityDistance.mEntity, instantDeath);
			}
		}
	}
}

// assumes mpEntity2 is an enemy
void WorldData::checkIfOnEdgeOfPlatform(Collision& curCollision)
{
	MovementManager& enemyMovementManager = curCollision.mEntity2.getMovementManager();
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	if (enemyMovementManager.getPath() == EEntityMovementPath_HORIZONTAL or
		enemyMovementManager.getPath() == EEntityMovementPath_VERTICAL or
		enemyMovementManager.getMovementCode() == EEntityMovements_JUMP or
		curCollision.mEntity1.getClassType() != EEntityClassTypes_PLATFORM)
	{
		return;
	}

	Platform& curPlatform = (Platform&)curCollision.mEntity1;
	Hitbox& curPlatformHitbox = curPlatform.getMovementManager().getHitbox();

	int curEnemyY2    = enemyMovementManager.getHitbox().getBottomRight().getY();
	int curPlatformY1 = curPlatformHitbox.getTopLeft().getY();

	// make sure the enemy is on the platform
	if (std::abs(curPlatformY1 - curEnemyY2) <= 5) 
	{
		int curHeight = enemyMovementManager.getHitbox().getHeight();
		Vect2 leftHitboxVect2 = Vect2(curPlatformHitbox.getTopLeft().getX(), curPlatformHitbox.getTopLeft().getY() - curHeight);
		Hitbox platformHitboxLeft = Hitbox(leftHitboxVect2, enemyMovementManager.getMovementVect2().getX(), curHeight);
		Vect2 rightHitboxVect2 = Vect2(curPlatformHitbox.getBottomRight().getX() - enemyMovementManager.getMovementVect2().getX(), curPlatformHitbox.getTopLeft().getY() - curHeight);
		Hitbox platformHitboxRight = Hitbox(rightHitboxVect2, enemyMovementManager.getMovementVect2().getX(), curHeight);

		bool collide = false;

		EDirection curDirection     = EDirection_INVALID;
		EDirection directionToSetTo = EDirection_INVALID;
		if (platformHitboxLeft.overlap(enemyMovementManager.getHitbox()))
		{
			collide			 = true;
			curDirection	 = EDirection_LEFT;
			directionToSetTo = EDirection_RIGHT;
		}
		else if (platformHitboxRight.overlap(enemyMovementManager.getHitbox()))
		{
			collide			 = true;
			curDirection	 = EDirection_RIGHT;
			directionToSetTo = EDirection_LEFT;
		}

		if (collide and enemyMovementManager.getCurDirection() != directionToSetTo)
		{
			enemyMovementManager.collided(curDirection);
			if (enemyMovementManager.getDidSwitchedDir())
			{
				curCollision.mEntity2.setTrapped();
			}
		}
	}
}

bool WorldData::collectedCollectible(Collectible& curCollectible)
{
	if (curCollectible.isAmPickedUp())
	{
		return false;
	}
	curCollectible.setAmPickedUp(true);
	CanGoToNextLevelResults results = canGoToNextLevel();;
	switch (curCollectible.getCharacterType())
	{
	case EEntityCharacterTypes_C_KEY:
		mPlayer.updateKeys(1);
		break;
	case EEntityCharacterTypes_C_SAVE_POINT:
		saveInGameStats();
		break;
	case EEntityCharacterTypes_C_LOTUS_COLLECTIBLE:
		mPlayer.addHeldCollectible(&curCollectible);
		break;
	case EEntityCharacterTypes_C_END_OF_LEVEL:
		if (results.mCanGoToNextLevel)
		{
			mpNextLevelData = results.mpNextLevelData;
			mGoToNextLevel = true;
			return true;
		}
		else
		{
			curCollectible.setAmPickedUp(false);
		}
		break;
	case EEntityCharacterTypes_C_MINI_GAME_LEVEL:
		mpNextLevelData = &((MiniGameLevelCollectible&)curCollectible).mNextLevelData;
		mGoToNextLevel = true;
		return true;
	default:
		SDL_assert(false);
		break;
	}
	return false;
	
}
#endif

void WorldData::playerShootProjectile(EEntityMovementPath path)
{
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

			Projectile* pProjectile = new Projectile(Vect2(projectileX1, projectileY1), preset, projectileDirection, mPlayer.mName);
			pProjectile->mAnimationManager.setUpAllTextures(mScreen.mpRenderer);
			if (DEBUG)
			{
				pProjectile->setHitboxTexture(AssetManager::getTextureFromSurface(mScreen.mpRenderer, pProjectile->getImageObjectHitbox().getSurface()));
			}
			pProjectile->getHostName() = mPlayer.mName;
			mpProjectiles.push_back(pProjectile);
			pProjectile = nullptr;
		}
	}

}

void WorldData::playerSwordSlash()
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	if (pCurLevel->mSlashAllowed)
	{
		mSlashManager.startSlash(mPlayer.getMovementManager());
		mSlashManager.mAnimationManager.setUpAllTextures(mScreen.mpRenderer);
		mSlashManager.mpHitboxTexture			= AssetManager::getTextureFromSurface(mScreen.mpRenderer, mSlashManager.mImageObjectHitbox.getSurface());
		mSlashManager.mpSlashImageHitboxTexture	= AssetManager::getTextureFromSurface(mScreen.mpRenderer, mSlashManager.mImageObjectImageHitbox.getSurface());
	}
}

void WorldData::enemyShootProjectile(Enemy& curEnemy)
{
	EnemyProjectilePreset preset = EnemyProjectilePreset();

	int projectileX1 = curEnemy.getMovementManager().getHitbox().getCenter().getX() - (preset.mWidth / 2);
	int projectileY1 = curEnemy.getMovementManager().getHitbox().getCenter().getY() - (preset.mHeight / 2);

	int difX = mPlayer.getMovementManager().getHitbox().getCenter().getX() - curEnemy.getMovementManager().getHitbox().getCenter().getX();
	int difY = mPlayer.getMovementManager().getHitbox().getCenter().getY() - curEnemy.getMovementManager().getHitbox().getCenter().getY();
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

	Projectile* pProjectile = new Projectile(Vect2(projectileX1 + moveX, projectileY1 + moveY), Vect2(std::abs(moveX), std::abs(moveY)), preset, projectileDirectionX, projectileDirectionY, curEnemy.mName);
	pProjectile->mAnimationManager.setUpAllTextures(mScreen.mpRenderer);
	pProjectile->setHitboxTexture(AssetManager::getTextureFromSurface(mScreen.mpRenderer, pProjectile->getImageObjectHitbox().getSurface()));
	mpProjectiles.push_back(pProjectile);
	bool doSeparate = true;
	pProjectile->getMovementManager().getHitbox().separate(curEnemy.getMovementManager().getHitbox(), doSeparate);
	pProjectile->getMovementManager().getHitbox().updateTopLeft(Vect2(moveX, moveY));

	curEnemy.shootProjectile();
}


void WorldData::entityPostTick()
{
	Level* curLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];

	mPlayer.postTick();

	// ALL Platforms
	std::vector <Platform*> pAllPlatforms = curLevel->getAllActivePlatforms();
	for (Platform* pCurPlatform : pAllPlatforms)
	{
		pCurPlatform->postTick();
	}

	// ENEMIES
	for (Enemy* pCurEnemy : curLevel->mpActiveEnemies)
	{
		pCurEnemy->postTick();
	}

	// PROJECTILES
	for (Projectile* pCurProjectile : mpProjectiles)
	{
		pCurProjectile->postTick();
	}

	if (!mPlayer.getAmAlive())
	{
		resetToCheckpoint();
	}
}

void WorldData::clearDeadProjectiles()
{
	for (int count = (int)mpProjectiles.size() -1; count >= 0; count--)
	{
		if (!mpProjectiles[count]->getAmAlive())
		{
			delete mpProjectiles[count];
			mpProjectiles.erase(mpProjectiles.begin() + count);
		}
	}
}

int  WorldData::getNumPlayerProjectiles()
{
	int numPlayerProjectiles = 0;
	for (Projectile* pProjectile : mpProjectiles)
	{
		if (pProjectile->getCharacterType() == EEntityCharacterTypes_PJ_PLAYER_PROJECTILE)
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
		Collectible* pCurCollectible = mPlayer.mpCurHeldCollectibles[count];
		if (pCurCollectible->isPermanentlyPickedUp())
		{
			mNumLotusCollectibles += 1;
			mPlayer.mpCurHeldCollectibles.erase(mPlayer.mpCurHeldCollectibles.begin() + count);
		}
	}
}

void WorldData::killedCharacter(Entity& characterKilled, bool instantDeath)
{
	if (instantDeath)
	{
		if (&characterKilled == &mPlayer)
		{
			// player killed
			resetToCheckpoint();
		}
		characterKilled.died();
	}
	else
	{
		if (&characterKilled == &mPlayer)
		{
			// player damaged
			mPlayer.takeDamage();
		}
		characterKilled.takeDamage();
	}
}


void WorldData::saveInGameStats()
{
	mPlayer.setCheckpointStats();

	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];

	for (Platform* pPlatform : pCurLevel->mpPlatforms)
	{
		pPlatform->setCheckpointStats();
	}
	for (Platform* pPlatform : pCurLevel->mpAllNonStaticPlatforms)
	{
		pPlatform->setCheckpointStats();
	}
	for (Enemy* pEnemy : pCurLevel->mpAllEnemies)
	{
		pEnemy->setCheckpointStats();
	}
	for (Collectible* pCollectible : pCurLevel->mpAllCollectibles)
	{
		pCollectible->setCheckpointStats();
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
	for (Projectile* pProjectile : mpProjectiles)
	{
		pProjectile->died();
	}
}


CanGoToNextLevelResults WorldData::canGoToNextLevel()
{
	bool canGoToNextLevel = true;
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	LevelData& nextLevelData = pCurLevel->mNextLevelData;
	int nextWorldNumber = nextLevelData.mWorldNumber;
	int nextLevelNumber = nextLevelData.mLevelNumber;
	if (pCurLevel->mMustKillAllEnemies and pCurLevel->mpActiveEnemies.size() != 0)
	{
		canGoToNextLevel = false;
	}
	if (nextLevelData.mType == ELevelType_PLATFORMING)
	{
		if (nextWorldNumber >= mpWorlds.size() )
		{
			canGoToNextLevel = false;
		}
		else if (nextLevelNumber >= mpWorlds[nextWorldNumber]->mpLevels.size())
		{
			canGoToNextLevel = false;
		}

	}
	
	return canGoToNextLevel ? CanGoToNextLevelResults(&nextLevelData) : CanGoToNextLevelResults();
}

void WorldData::setNextLevel(int nextWorldNumber, int nextLevelNumber)
{
	mCurWorldNumber = nextWorldNumber;
	mCurLevelNumber = nextLevelNumber;
	mPlayer.getMovementManager().setStartPosition(mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber]->mPlayerStartingPosition);
	mpNextLevelData = nullptr;
	mGoToNextLevel = false;
	resetStats();
}


void WorldData::renderTexture(SDL_Texture* pTexture, const Hitbox& hitbox, const Vect2& imageOffset)
{
	EImageOffset offsetType = EImageOffset_PRINT_TOP_LEFT;
	EDirection direction = EDirection_LEFT;
	bool rotating = false;
	float rotation = 0.0;
	renderTexture(pTexture, hitbox, offsetType, imageOffset, direction, rotating, rotation);
}

void WorldData::renderTexture(SDL_Texture* pTexture, const Hitbox& hitbox, EImageOffset offsetType, const Vect2& imageOffset, EDirection entityFacingDirection, bool rotating, float rotation)
{
	float printX = float(hitbox.getTopLeft().getX() + imageOffset.getX() - mCurLevelChunk.mHitbox.getTopLeft().getX()) * mScreen.mGameScreenToGameLevelChunkRatio;
	float printY = float(hitbox.getTopLeft().getY() + imageOffset.getY() - mCurLevelChunk.mHitbox.getTopLeft().getY()) * mScreen.mGameScreenToGameLevelChunkRatio;
	float printWidth  = float(hitbox.getWidth()  * mScreen.mGameScreenToGameLevelChunkRatio);
	float printHeight = float(hitbox.getHeight() * mScreen.mGameScreenToGameLevelChunkRatio);

	SDL_FPoint rotationPoint = {printWidth / 2, printHeight / 2};
	if (rotating)
	{
		if (offsetType == EImageOffset_PRINT_TOP_LEFT)
		{
			rotationPoint = {0.0f, 0.0f};
			if (entityFacingDirection == EDirection_LEFT)
			{
				rotationPoint = {printWidth, 0.0f};
			}
		}
		else if (offsetType == EImageOffset_FULL_UPWARDS || offsetType == EImageOffset_MIDDLE)
		{
			// defaults
		}
		else if (offsetType == EImageOffset_LEFT_X_MIDDLE_Y)
		{
			rotationPoint = {0.0f, printHeight / 2};
			if (entityFacingDirection == EDirection_LEFT)
			{
				printX -= printWidth;
				rotationPoint = {printWidth, printHeight / 2};
			}
		}
		else if (offsetType == EImageOffset_MIDDLE_X_TOP_Y)
		{
			rotationPoint = {printWidth / 2, 0.0f};
		}
		else
		{
			SDL_assert(false);
		}

		if (entityFacingDirection == EDirection_RIGHT)
		{
			rotation += 180.0f;
		}
	}
	else
	{
		rotation = 0.0f;
	}

	SDL_FRect curEntityPositionToPrintTo = { printX, printY, printWidth, printHeight };

	SDL_RendererFlip direction = SDL_FLIP_NONE;
	if (entityFacingDirection == EDirection_RIGHT)
	{
		direction = SDL_FLIP_HORIZONTAL;
	}

	SDL_RenderTextureRotated(mScreen.mpRenderer, pTexture, NULL, &curEntityPositionToPrintTo, rotation, &rotationPoint, direction);
}

void WorldData::renderEntityWithHitbox(Entity& curEntity)
{
	Vect2 hitboxOffset = Vect2(0, 0);
	renderTexture(curEntity.getHitboxTexture(), curEntity.getMovementManager().getHitbox(), hitboxOffset);
	renderEntity(curEntity);
}

void WorldData::renderEntityWithHitbox(const ImageObject& curImageObject, SDL_Texture* pHitboxTexture, EImageOffset offsetType, EDirection entityFacingDirection, const Hitbox& entityHitbox, bool rotating, float rotation)
{
	Vect2 hitboxOffset = Vect2(0, 0);
	renderTexture(pHitboxTexture, entityHitbox, offsetType, hitboxOffset, entityFacingDirection, rotating, rotation);
	renderEntity(curImageObject, offsetType, entityFacingDirection, entityHitbox, rotating, rotation);
}

void WorldData::renderEntityViaChunk(Entity& curEntity) 
{
	if (curEntity.mIsVisible)
	{
		ImageObject* pCurImageObject = curEntity.mAnimationManager.getCurImage();
		Hitbox& hitbox = curEntity.getMovementManager().getHitbox();

		renderEntityViaChunk(*pCurImageObject, hitbox);
	}
}

void WorldData::renderEntityViaChunk(const ImageObject& curImageObject, const Hitbox& entityHitbox) 
{
	// CAN EITHER CHUNK DOWN OR CHUNK RIGHT
	float rightChunkShift = 0.0;
	float upChunkShift    = 0.0;
	if (entityHitbox.getWidth() == entityHitbox.getHeight())
	{
		float printX = float(entityHitbox.getTopLeft().getX() - mCurLevelChunk.mHitbox.getTopLeft().getX() + curImageObject.mImageOffsetX) * mScreen.mGameScreenToGameLevelChunkRatio;
		float printY = float(entityHitbox.getTopLeft().getY() - mCurLevelChunk.mHitbox.getTopLeft().getY() + curImageObject.mImageOffsetY) * mScreen.mGameScreenToGameLevelChunkRatio;

		float printWidth   = entityHitbox.getWidth()  * mScreen.mGameScreenToGameLevelChunkRatio;
		float printHeight  = entityHitbox.getHeight() * mScreen.mGameScreenToGameLevelChunkRatio;

		float actualWidth  = (float)curImageObject.getSurface()->w;
		float actualHeight = (float)curImageObject.getSurface()->h;
		float actualToPrintRatio = actualWidth / printWidth;

		float cropWidth   = printWidth  * actualToPrintRatio;
		float cropHeight  = printHeight * actualToPrintRatio;

		SDL_FRect curCropBox = {0, 0, cropWidth, cropHeight};
		SDL_FRect curEntityPositionToPrintTo = { printX, printY, printWidth, printHeight };

		SDL_RenderTextureRotated(mScreen.mpRenderer, curImageObject.getTexture(), &curCropBox, &curEntityPositionToPrintTo, 0, NULL, SDL_FLIP_NONE);
	}
	else
	{
		for (int count = 0; count <= curImageObject.mNumChunks - 1; count++)
		{
			switch (curImageObject.mChunkDirection)
			{
			case EDirection_RIGHT:
				rightChunkShift = float(count * curImageObject.mIdealImageWidth);
				break;
			case EDirection_DOWN:
				upChunkShift = float(count * curImageObject.mIdealImageHeight);
				break;
			default:
				SDL_assert(false);
				break;
			}

			float printX = float(entityHitbox.getTopLeft().getX() - mCurLevelChunk.mHitbox.getTopLeft().getX() + curImageObject.mImageOffsetX + rightChunkShift ) * mScreen.mGameScreenToGameLevelChunkRatio;
			float printY = float(entityHitbox.getTopLeft().getY() - mCurLevelChunk.mHitbox.getTopLeft().getY() + curImageObject.mImageOffsetY + upChunkShift    ) * mScreen.mGameScreenToGameLevelChunkRatio;

			float printWidth  = float(curImageObject.mIdealImageWidth)  * mScreen.mGameScreenToGameLevelChunkRatio;
			float printHeight = float(curImageObject.mIdealImageHeight) * mScreen.mGameScreenToGameLevelChunkRatio;

			float actualWidth  = (float)curImageObject.getSurface()->w;
			float actualHeight = (float)curImageObject.getSurface()->h;
			float actualToPrintRatio = actualWidth / printWidth;

			float cropWidth   = actualWidth;
			float cropHeight  = actualHeight;

			if (count == curImageObject.mNumChunks - 1)
			{
				// last chunk is shortened based on where platform ends
				switch (curImageObject.mChunkDirection)
				{
				case EDirection_RIGHT:
					printWidth = (entityHitbox.getWidth() - (rightChunkShift)) * mScreen.mGameScreenToGameLevelChunkRatio;
					cropWidth = printWidth * actualToPrintRatio;
					break;
				case EDirection_DOWN:
					printHeight = (entityHitbox.getHeight() - (upChunkShift)) * mScreen.mGameScreenToGameLevelChunkRatio;
					cropHeight = printHeight * actualToPrintRatio;
					break;
				default:
					break;
				}
			}


			SDL_FRect curCropBox = {0.0, 0.0, cropWidth, cropHeight};
			SDL_FRect curEntityPositionToPrintTo = { printX, printY, printWidth, printHeight };

			SDL_RenderTextureRotated(mScreen.mpRenderer, curImageObject.getTexture(), &curCropBox, &curEntityPositionToPrintTo, 0.0, NULL, SDL_FLIP_NONE);
		}
	}
}

void WorldData::renderEntityViaSplice(Entity& curEntity) 
{
	if (curEntity.mIsVisible)
	{
		const AnimationManager& animationManager	= curEntity.mAnimationManager;
		const Hitbox&           hitbox				= curEntity.getMovementManager().getHitbox();
		renderEntityViaSplice(animationManager, hitbox);
	}
}

void WorldData::renderEntityViaSplice(const AnimationManager& animationManager, const Hitbox& entityHitbox) 
{
	// CAN EITHER SPLICE RIGHT OR DOWN
	ImageObject* pCurImageObject = animationManager.getCurImage();
	float rightChunkShift = 0.0;
	float upChunkShift    = 0.0;
	if (entityHitbox.getWidth() == entityHitbox.getHeight())
	{
		// square
		float printX = float(entityHitbox.getTopLeft().getX() - mCurLevelChunk.mHitbox.getTopLeft().getX() + pCurImageObject->mImageOffsetX) * mScreen.mGameScreenToGameLevelChunkRatio;
		float printY = float(entityHitbox.getTopLeft().getY() - mCurLevelChunk.mHitbox.getTopLeft().getY() + pCurImageObject->mImageOffsetY) * mScreen.mGameScreenToGameLevelChunkRatio;

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
		// splice images by pasting each animation frame one after another until total shape is filled
		int curImage = 0;

		for (int count = 0; count < pCurImageObject->mNumChunks; count++)
		{
			pCurImageObject = animationManager.getCurFrames()[curImage];
			if (pCurImageObject->mChunkDirection == EDirection_RIGHT)
			{
				rightChunkShift = float(count * pCurImageObject->mIdealImageWidth);
			}
			else if (pCurImageObject->mChunkDirection == EDirection_DOWN)
			{
				upChunkShift    = float(count * pCurImageObject->mIdealImageHeight);
			}
			else
			{
				SDL_assert(false);
			}

			float printX = float(entityHitbox.getTopLeft().getX() - mCurLevelChunk.mHitbox.getTopLeft().getX() + pCurImageObject->mImageOffsetX + rightChunkShift ) * mScreen.mGameScreenToGameLevelChunkRatio;
			float printY = float(entityHitbox.getTopLeft().getY() - mCurLevelChunk.mHitbox.getTopLeft().getY() + pCurImageObject->mImageOffsetY + upChunkShift    ) * mScreen.mGameScreenToGameLevelChunkRatio;

			float printWidth  = float(pCurImageObject->mIdealImageWidth)  * mScreen.mGameScreenToGameLevelChunkRatio;
			float printHeight = float(pCurImageObject->mIdealImageHeight) * mScreen.mGameScreenToGameLevelChunkRatio;

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

void WorldData::renderEntity(Entity& curEntity)
{
	if (curEntity.mIsVisible)
	{
		ImageObject*	pCurImageObject = curEntity.mAnimationManager.getCurImage();
		EDirection		imageDirection	 = curEntity.getMovementManager().getCurFacingDirection();
		EImageOffset	imageOffsetType = EImageOffset_MIDDLE;
		if (curEntity.getMovementManager().getPath() == EEntityMovementPath_HORIZONTAL_CAN_FALL)
		{
			imageOffsetType = EImageOffset_FULL_UPWARDS;
		}
		Hitbox& hitbox = curEntity.getMovementManager().getHitbox();
		renderEntity(*pCurImageObject, imageOffsetType, imageDirection, hitbox);
	}
}

void WorldData::renderEntity(const ImageObject& curImageObject, EImageOffset offsetType, EDirection entityFacingDirection, const Hitbox& entityHitbox)
{
	int imageOffsetX = -((curImageObject.mIdealImageWidth - entityHitbox.getWidth()) / 2) + curImageObject.mImageOffsetX;
	int imageOffsetY = curImageObject.mImageOffsetY;
	switch (offsetType)
	{
	case EImageOffset_MIDDLE:
		imageOffsetY -= (curImageObject.mIdealImageHeight - entityHitbox.getHeight()) / 2;
		break;
	case EImageOffset_FULL_UPWARDS:
		imageOffsetY -= (curImageObject.mIdealImageHeight - entityHitbox.getHeight());
		break;
	default:
		SDL_assert(false);
		break;
	}
	
	bool rotating = false;
	float rotation = 0.0;

	Hitbox imageHitbox = Hitbox(entityHitbox.getTopLeft(), curImageObject.mIdealImageWidth, curImageObject.mIdealImageHeight);
	renderTexture(curImageObject.getTexture(), imageHitbox, offsetType, Vect2(imageOffsetX, imageOffsetY), entityFacingDirection, rotating, rotation);
}

void WorldData::renderEntity(const ImageObject& curImageObject, EImageOffset offsetType, EDirection entityFacingDirection, const Hitbox& entityHitbox, bool rotating, float rotation)
{
	Vect2 imageOffset = Vect2(0,0);
	renderTexture(curImageObject.getTexture(), entityHitbox, offsetType, imageOffset, entityFacingDirection, rotating, rotation);
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

		float actualWidth  = (float)pCurLevel->mpArtFileSurface->w;
		float actualHeight = (float)pCurLevel->mpArtFileSurface->h;
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

		if (curLevelChunkX1 < xShift && curLevelChunkX2 >(levelW + xShift))
		{
			dX = (xShift - curLevelChunkX1) * mScreen.mGameScreenToGameLevelChunkRatio;
			// cur level chunk x1 is neg
			cropW -= -((xShift - curLevelChunkX1) + (curLevelChunkX2 - (levelW + xShift)))  * actualToPrintRatio;
			dW -= ((xShift - curLevelChunkX1) + (curLevelChunkX2 - (levelW + xShift))) * mScreen.mGameScreenToGameLevelChunkRatio;
		}
		else if (curLevelChunkX1 < xShift)
		{
			// cur level chunk x1 is neg
			dX     = (xShift - curLevelChunkX1) * mScreen.mGameScreenToGameLevelChunkRatio;
			cropW += (xShift - curLevelChunkX1) * actualToPrintRatio;
		}
		else if (curLevelChunkX2 > (levelW + xShift))
		{
			dW -= (curLevelChunkX2 - (levelW + xShift)) * mScreen.mGameScreenToGameLevelChunkRatio;
			cropW -= (curLevelChunkX2 - (levelW + xShift)) * actualToPrintRatio;
		}

		if (curLevelChunkY1 < yShift && curLevelChunkY2 >(levelH + yShift))
		{
			// cur level chunk y1 is neg
			dY = (yShift - curLevelChunkY1) * mScreen.mGameScreenToGameLevelChunkRatio;
			cropH -= -((yShift - curLevelChunkY1) + (curLevelChunkY2 - (levelH + yShift))) * actualToPrintRatio;
			dH -= ((yShift - curLevelChunkY1) + (curLevelChunkY2 - (levelH + yShift))) * mScreen.mGameScreenToGameLevelChunkRatio;
		}
		else if (curLevelChunkY1 < yShift)
		{
			// cur level chunk y1 is neg
			dY     = (yShift - curLevelChunkY1) * mScreen.mGameScreenToGameLevelChunkRatio;
			cropH += (yShift - curLevelChunkY1) * actualToPrintRatio;
		}
		else if (curLevelChunkY2 > (levelH + yShift))
		{
			dH    -= (curLevelChunkY2 - (levelH + yShift)) * mScreen.mGameScreenToGameLevelChunkRatio;
			cropH -= (curLevelChunkY2 - (levelH + yShift)) * actualToPrintRatio;
		}

		

		// where on the screen it's being printed (in screen space)
		SDL_FRect destination = {dX, dY, dW, dH};
		// what section of the texture is being printed (in texture space)
		SDL_FRect curCropBox = {cropX, cropY, cropW, cropH};

		SDL_RenderTextureRotated(mScreen.mpRenderer, pCurLevel->mpArtFileTexture, &curCropBox, &destination, 0, NULL, SDL_FLIP_NONE);
	}
}

void WorldData::renderBackgroundEffects()
{
	Level* pCurLevel = mpWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber];
	ScreenObject& screen = mScreen;
	for (CircleEffect* pCircleEffect : pCurLevel->mpBackgroundEffects)
	{
		renderCircleGradient(pCircleEffect->mColor, pCircleEffect->mCurCenter, pCircleEffect->mRadius);
	}
}

void WorldData::renderCircleGradient(const SDL_Color& color, const Vect2& center, int radius)
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
			float radians = (float)degToRad(degrees);
			float curX = actualCenterX + (cos(radians) * curRadius);
			float curY = actualCenterY + (sin(radians) * curRadius);
			float alpha = (actualRadius - curRadius) / actualRadius;
			SDL_SetRenderDrawColor(pRenderer, color.r, color.g, color.b, Uint8(alpha));
			SDL_RenderPoint(pRenderer, curX, curY);
		}
	}
}
