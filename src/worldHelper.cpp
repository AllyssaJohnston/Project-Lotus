#include <vector>

#include "worldHelper.h"
#include "collectibleHelper.h"
#include "platformHelper.h"
#include "enemyHelper.h"
#include "imageHelper.h"

Level::Level(int levelNumber, Vect2 playerStartingPosition, LevelInfo levelInfo)
{
	mLevelNumber					= levelNumber;
	mDoubleJumpAllowed				= levelInfo.mDoubleJumpAllowed;
	mThrowProjectileAllowed			= levelInfo.mThrowProjectileAllowed;
	mThrowDownwardProjectileAllowed = levelInfo.mThrowDownwardProjectileAllowed;
	mSlashAllowed					= levelInfo.mSlashAllowed;
	mMustKillAllEnemies		        = levelInfo.mMustKillAllEnemies;

	if (true == mThrowDownwardProjectileAllowed and false == mThrowProjectileAllowed)
	{
		SDL_assert(false);
	}

	mPlayerStartingPosition = playerStartingPosition;
}

Level::~Level()
{
	for (Platform* plat : mpPlatforms)
	{
		if (plat)
			delete plat;
	}

	for (Platform* plat : mpAllNonStaticPlatforms)
	{
		if (plat)
			delete plat;
	}

	for (Platform* plat : mpAreaEffectPlatforms)
	{
		if (plat)
			delete plat;
	}

	for (Enemy* enemy : mpAllEnemies)
	{
		if (enemy)
			delete enemy;
	}

	for (Collectible* collect : mpAllCollectibles)
	{
		if (collect)
			delete collect;
	}

	for (CircleEffect* circle : mpBackgroundEffects)
	{
		if (circle)
			delete circle;
	}
}

void Level::resetStats()
{
	for (Platform* platform : mpPlatforms)
	{
		platform->resetStats();
	}
	for (Platform* platform : mpAllNonStaticPlatforms)
	{
		platform->resetStats();
	}
	for (Enemy* enemy : mpAllEnemies)
	{
		enemy->resetStats();
	}
	for (Collectible* collectible : mpAllCollectibles)
	{
		collectible->resetStats();
	}
	setUpActiveEntitiesVectors();

	resetDynamicQuadTree();
}

void Level::resetToCheckpoint()
{
	for (int count = 0; count < mpPlatforms.size(); count++)
	{
		mpPlatforms[count]->resetToCheckpoint();
	}
	for (int count = 0; count < mpAllNonStaticPlatforms.size(); count++)
	{
		mpAllNonStaticPlatforms[count]->resetToCheckpoint();
	}
	for (int count = 0; count < mpAllEnemies.size(); count++)
	{
		mpAllEnemies[count]->resetToCheckpoint();
	}
	for (int count = 0; count < mpAllCollectibles.size(); count++)
	{
		mpAllCollectibles[count]->resetToCheckpoint();
	}
	setUpActiveEntitiesVectors();

	resetDynamicQuadTree();
}

void Level::setUpActiveEntitiesVectors()
{
	mpActiveNonStaticPlatforms.clear();
	for (int count = 0; count < mpAllNonStaticPlatforms.size(); count++)
	{
		if (mpAllNonStaticPlatforms[count]->getAmAlive())
		{
			mpActiveNonStaticPlatforms.push_back(mpAllNonStaticPlatforms[count]);
		}
	}
	mpActiveEnemies.clear();
	for (int count = 0; count < mpAllEnemies.size(); count++)
	{
		if (mpAllEnemies[count]->getAmAlive())
		{
			mpActiveEnemies.push_back(mpAllEnemies[count]);
		}
	}
	mpActiveCollectibles.clear();
	for (int count = 0; count < mpAllCollectibles.size(); count++)
	{
		if (mpAllCollectibles[count]->isAmPickedUp() == false)
		{
			mpActiveCollectibles.push_back(mpAllCollectibles[count]);
		}
	}
}

void Level::resetDynamicQuadTree()
{
	for (Platform* platform : mpAllNonStaticPlatforms)
	{
		if (platform->getAmAlive())
		{
			mDynamicEntities.AddItem(platform);
		}
	}
	for (Enemy* enemy : mpAllEnemies)
	{
		if (enemy->getAmAlive())
		{
			mDynamicEntities.AddItem(enemy);
		}
	}
	for (Collectible* collectible : mpAllCollectibles)
	{
		if (!collectible->isAmPickedUp())
		{
			mDynamicEntities.AddItem(collectible);
		}
	}
}

void Level::removeInactiveEntitiesFromActiveEntitiesVector()
{
	for (int count = (int)mpActiveNonStaticPlatforms.size() - 1; count >= 0; count--)
	{
		if (mpActiveNonStaticPlatforms[count]->getAmAlive() == false)
		{
			mDynamicEntities.RemoveItem(mpActiveNonStaticPlatforms[count]);
			mpActiveNonStaticPlatforms.erase(mpActiveNonStaticPlatforms.begin() + count);
		}
	}
	for (int count = (int)mpActiveEnemies.size() - 1; count >= 0; count--)
	{
		if (mpActiveEnemies[count]->getAmAlive() == false)
		{
			mDynamicEntities.RemoveItem(mpActiveEnemies[count]);
			mpActiveEnemies.erase(mpActiveEnemies.begin() + count);
		}
	}
	for (int count = (int)mpActiveCollectibles.size() - 1; count >= 0; count--)
	{
		if (mpActiveCollectibles[count]->isAmPickedUp() == true)
		{
			mDynamicEntities.RemoveItem(mpActiveCollectibles[count]);
			mpActiveCollectibles.erase(mpActiveCollectibles.begin() + count);
		}
	}

}

std::vector <Platform*> Level::getAllPlatforms() const
{
	std::vector <Platform* > platforms;
	for (int count = 0; count < mpPlatforms.size(); count++)
	{
		platforms.push_back(mpPlatforms[count]);
	}
	for (int count = 0; count < mpAllNonStaticPlatforms.size(); count++)
	{
		platforms.push_back(mpAllNonStaticPlatforms[count]);
	}
	for (int count = 0; count < mpAreaEffectPlatforms.size(); count++)
	{
		platforms.push_back(mpAreaEffectPlatforms[count]);
	}
	return platforms;
}

std::vector <Platform*> Level::getAllActivePlatforms() const
{
	std::vector <Platform* > platforms;
	for (int count = 0; count < mpPlatforms.size(); count++)
	{
		platforms.push_back(mpPlatforms[count]);
	}
	for (int count = 0; count < mpActiveNonStaticPlatforms.size(); count++)
	{
		platforms.push_back(mpActiveNonStaticPlatforms[count]);
	}
	for (int count = 0; count < mpAreaEffectPlatforms.size(); count++)
	{
		platforms.push_back(mpAreaEffectPlatforms[count]);
	}

	return platforms;
}

//PLATFORMS + ENEMIES
std::vector <Entity*> Level::getAllActiveEntities() const
{
	std::vector <Entity* > entities;
	for (int count = 0; count < mpActiveEnemies.size(); count++)
	{
		entities.push_back(mpActiveEnemies[count]);
	}
	std::vector <Platform*> platforms = getAllActivePlatforms();
	for (int count = 0; count < platforms.size(); count++)
	{
		entities.push_back(platforms[count]);
	}

	return entities;
}

void Level::setUp(SDL_Renderer* pRenderer)
{
	int x1     = std::numeric_limits<int>::max();
	int y1     = std::numeric_limits<int>::max();
	int x2 = 0;
	int y2 = 0;
	for (int count = 0; count < mpPlatforms.size(); count++)
	{
		x1 = std::min( x1, mpPlatforms[count]->getMovementManager().getHitbox().getTopLeft().getX() );
		x2 = std::max( x2, mpPlatforms[count]->getMovementManager().getHitbox().getBottomRight().getX() );
		y1 = std::min( y1, mpPlatforms[count]->getMovementManager().getHitbox().getTopLeft().getY() );
		y2 = std::max( y2, mpPlatforms[count]->getMovementManager().getHitbox().getBottomRight().getY() );
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

	//set up static quad tree
	for (Platform* platform : mpPlatforms)
	{
		mStaticEntities.AddItem(platform);
	}

	setUpArtFileTexture(pRenderer);
}

void Level::setUpArtFileTexture(SDL_Renderer* pRenderer)
{
	if (mArtFileName != "")
	{
		std::string curName = "./ArtFiles/" + mArtFileName;
		SDL_Surface* curSurface = AssetManager::getSurfaceFromFile(curName);
		SDL_Texture* curTexture = AssetManager::getTextureFromSurface(pRenderer, curSurface);
		mArtFileSurface = curSurface;
		mArtFileTexture = curTexture;
	}
}

