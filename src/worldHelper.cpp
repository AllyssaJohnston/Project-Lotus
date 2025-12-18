#include <vector>

#include "worldHelper.h"
#include "collectibleHelper.h"
#include "platformHelper.h"
#include "enemyHelper.h"
#include "imageHelper.h"

Level::Level(int levelNumber, Vect2 playerStartingPosition, LevelInfo levelInfo, LevelData nextLevelData) : 
		mLevelNumber(levelNumber), mPlayerStartingPosition(playerStartingPosition), mNextLevelData(nextLevelData)
{
	mDoubleJumpAllowed				= levelInfo.mDoubleJumpAllowed;
	mThrowProjectileAllowed			= levelInfo.mThrowProjectileAllowed;
	mThrowDownwardProjectileAllowed = levelInfo.mThrowDownwardProjectileAllowed;
	mSlashAllowed					= levelInfo.mSlashAllowed;
	mMustKillAllEnemies		        = levelInfo.mMustKillAllEnemies;

	if (mThrowDownwardProjectileAllowed and !mThrowProjectileAllowed)
	{
		SDL_assert(false);
	}	
}

Level::~Level()
{
	mpArtFileSurface = nullptr;
	mpArtFileTexture = nullptr;

	for (Platform* pPlatform : mpPlatforms)
	{
		if (pPlatform)
		{
			delete pPlatform;
		}
	}
	mpPlatforms.clear();

	mpActiveNonStaticPlatforms.clear();
	for (Platform* pPlatform : mpAllNonStaticPlatforms)
	{
		if (pPlatform)
		{
			delete pPlatform;
		}
	}
	mpAllNonStaticPlatforms.clear();

	for (Platform* pPlatform : mpAreaEffectPlatforms)
	{
		if (pPlatform)
		{
			delete pPlatform;
		}
	}
	mpAreaEffectPlatforms.clear();

	mpActiveEnemies.clear();
	for (Enemy* pEnemy : mpAllEnemies)
	{
		if (pEnemy)
		{
			delete pEnemy;
		}
	}
	mpAllEnemies.clear();

	mpActiveCollectibles.clear();
	for (Collectible* pCollectible : mpAllCollectibles)
	{
		pCollectible = nullptr;
	}
	mpAllCollectibles.clear();

	for (CircleEffect* pBackgroundEffect : mpBackgroundEffects)
	{
		pBackgroundEffect = nullptr;
	}
	mpAllCollectibles.clear();
}

void Level::resetStats()
{
	for (Platform* pPlatform : mpPlatforms)
	{
		pPlatform->resetStats();
	}
	for (Platform* pPlatform : mpAllNonStaticPlatforms)
	{
		pPlatform->resetStats();
	}
	for (Enemy* pEnemy : mpAllEnemies)
	{
		pEnemy->resetStats();
	}
	for (Collectible* pCollectible : mpAllCollectibles)
	{
		pCollectible->resetStats();
	}
	setUpActiveEntitiesVectors();
	resetDynamicQuadTree();
}

void Level::resetToCheckpoint()
{
	for (Platform* pPlatform : mpPlatforms)
	{
		pPlatform->resetToCheckpoint();
	}
	for (Platform* pPlatform : mpAllNonStaticPlatforms)
	{
		pPlatform->resetToCheckpoint();
	}
	for (Enemy* pEnemy : mpAllEnemies)
	{
		pEnemy->resetToCheckpoint();
	}
	for (Collectible* pCollectible : mpAllCollectibles)
	{
		pCollectible->resetToCheckpoint();
	}
	setUpActiveEntitiesVectors();

	resetDynamicQuadTree();
}

void Level::setUpActiveEntitiesVectors()
{
	mpActiveNonStaticPlatforms.clear();
	for (Platform* pNonStatic : mpAllNonStaticPlatforms)
	{
		if (pNonStatic->getAmAlive())
		{
			mpActiveNonStaticPlatforms.push_back(pNonStatic);
		}
	}
	mpActiveEnemies.clear();
	for (Enemy* pEnemy : mpAllEnemies)
	{
		if (pEnemy->getAmAlive())
		{
			mpActiveEnemies.push_back(pEnemy);
		}
	}
	mpActiveCollectibles.clear();
	for (Collectible* pCollectible : mpAllCollectibles)
	{
		if (!pCollectible->isAmPickedUp())
		{
			mpActiveCollectibles.push_back(pCollectible);
		}
	}
}

void Level::resetDynamicQuadTree()
{
	for (Platform* pPlatform : mpAllNonStaticPlatforms)
	{
		if (pPlatform->getAmAlive())
		{
			mDynamicEntities.AddItem(pPlatform);
		}
	}
	for (Enemy* pEnemy : mpAllEnemies)
	{
		if (pEnemy->getAmAlive())
		{
			mDynamicEntities.AddItem(pEnemy);
		}
	}
	for (Collectible* pCollectible : mpAllCollectibles)
	{
		if (!pCollectible->isAmPickedUp())
		{
			mDynamicEntities.AddItem(pCollectible);
		}
	}
}

void Level::removeInactiveEntitiesFromActiveEntitiesVector()
{
	for (int count = (int)mpActiveNonStaticPlatforms.size() - 1; count >= 0; count--)
	{
		if (!mpActiveNonStaticPlatforms[count]->getAmAlive())
		{
			mDynamicEntities.RemoveItem(mpActiveNonStaticPlatforms[count]);
			mpActiveNonStaticPlatforms.erase(mpActiveNonStaticPlatforms.begin() + count);
		}
	}
	for (int count = (int)mpActiveEnemies.size() - 1; count >= 0; count--)
	{
		if (!mpActiveEnemies[count]->getAmAlive())
		{
			mDynamicEntities.RemoveItem(mpActiveEnemies[count]);
			mpActiveEnemies.erase(mpActiveEnemies.begin() + count);
		}
	}
	for (int count = (int)mpActiveCollectibles.size() - 1; count >= 0; count--)
	{
		if (mpActiveCollectibles[count]->isAmPickedUp())
		{
			mDynamicEntities.RemoveItem(mpActiveCollectibles[count]);
			mpActiveCollectibles.erase(mpActiveCollectibles.begin() + count);
		}
	}

}

std::vector <Platform*> Level::getAllPlatforms() const
{
	std::vector <Platform*> pPlatforms;
	for (Platform* pPlatform : mpPlatforms)
	{
		pPlatforms.push_back(pPlatform);
	}
	for (Platform* pPlatform : mpAllNonStaticPlatforms)
	{
		pPlatforms.push_back(pPlatform);
	}
	for (Platform* pPlatform : mpAreaEffectPlatforms)
	{
		pPlatforms.push_back(pPlatform);
	}
	return pPlatforms;
}

std::vector <Platform*> Level::getAllActivePlatforms() const
{
	std::vector <Platform*> pPlatforms;
	for (Platform* pPlatform : mpPlatforms)
	{
		pPlatforms.push_back(pPlatform);
	}
	for (Platform* pPlatform : mpActiveNonStaticPlatforms)
	{
		pPlatforms.push_back(pPlatform);
	}
	for (Platform* pPlatform : mpAreaEffectPlatforms)
	{
		pPlatforms.push_back(pPlatform);
	}
	return pPlatforms;
}

// PLATFORMS + ENEMIES
std::vector <Entity*> Level::getAllActiveEntities() const
{
	std::vector <Entity*> entities;
	for (Entity* pEnemy : mpActiveEnemies)
	{
		entities.push_back(pEnemy);
	}
	for (Platform* pPlatform : mpPlatforms)
	{
		entities.push_back(pPlatform);
	}
	for (Platform* pPlatform : mpActiveNonStaticPlatforms)
	{
		entities.push_back(pPlatform);
	}
	for (Platform* pPlatform : mpAreaEffectPlatforms)
	{
		entities.push_back(pPlatform);
	}

	return entities;
}

Hitbox& Level::getHitbox() { return mHitbox; }

void Level::setUp(SDL_Renderer* pRenderer)
{
	int x1     = std::numeric_limits<int>::max();
	int y1     = std::numeric_limits<int>::max();
	int x2 = 0;
	int y2 = 0;
	for (Platform* pPlatform : getAllPlatforms())
	{
		x1 = std::min( x1, pPlatform->getMovementManager().getHitbox().getTopLeft().getX() );
		x2 = std::max( x2, pPlatform->getMovementManager().getHitbox().getBottomRight().getX() );
		y1 = std::min( y1, pPlatform->getMovementManager().getHitbox().getTopLeft().getY() );
		y2 = std::max( y2, pPlatform->getMovementManager().getHitbox().getBottomRight().getY() );
	}

	mArtFileX = x1;
	mArtFileY = y1;
	mArtFileWidth  = x2  - x1;
	mArtFileHeight = y2  - y1;
	mTrueLevelX2 = x2;
	mTrueLevelY2 = y2;
	mLevelX2  = x2 + 50;
	mLevelY2  = y2 + 50;

	mStaticEntities.SetHitbox(	Hitbox(x1, mTrueLevelX2, y1, mTrueLevelY2));
	mDynamicEntities.SetHitbox(	Hitbox(x1, mTrueLevelX2, y1, mTrueLevelY2));

	// set up static quad tree
	for (Platform* pPlatform : mpPlatforms)
	{
		mStaticEntities.AddItem(pPlatform);
	}

	setUpArtFileTexture(pRenderer);
	mHitbox = Hitbox(Vect2(0, 0), mLevelX2, mLevelY2);
}

void Level::setUpArtFileTexture(SDL_Renderer* pRenderer)
{
	if (mArtFileName != "")
	{
		std::string curName = "./ArtFiles/" + mArtFileName;
		SDL_Surface* curSurface = AssetManager::getSurfaceFromFile(curName);
		mpArtFileSurface = curSurface;
		mpArtFileTexture = AssetManager::getTextureFromSurface(pRenderer, curSurface);
		curSurface = nullptr;
	}
}


LevelChunk::LevelChunk(const CoordsX1Y1WidthHeight& coords) { mHitbox = Hitbox(coords); }

LevelChunk::LevelChunk() { mHitbox = {}; }

void LevelChunk::updateCoords(const Vect2& newVect2) { mHitbox.setTopLeft(newVect2); }

Hitbox& LevelChunk::getHitbox() { return mHitbox; }


World::World(int worldNumber) { mWorldNumber = worldNumber; }