#pragma once

#include <vector>
#include "entityHelper.h"
#include "hitboxHelper.h"
#include "worldHelperClass.h"
#include "entityQuadTree.h"

class Platform;
class AreaEffectPlatform;
class Enemy;
class Collectible;

class Level
{
public:
	int  mLevelNumber;
	int  mTrueLevelX2;
	int  mLevelX2;
	int  mTrueLevelY2;
	int  mLevelY2;

	int mArtFileX;
	int mArtFileY;
	int mArtFileWidth;
	int mArtFileHeight;
	bool mDoubleJumpAllowed;
	bool mThrowProjectileAllowed;
	bool mThrowDownwardProjectileAllowed;
	bool mSlashAllowed;
	bool mMustKillAllEnemies;

	Vect2 mPlayerStartingPosition;

	std::vector <Platform* >			mpPlatforms;
	std::vector <Platform* >			mpAllNonStaticPlatforms;
	std::vector <Platform* >			mpActiveNonStaticPlatforms;
	std::vector <AreaEffectPlatform*>	mpAreaEffectPlatforms;
	std::vector <Enemy*>				mpAllEnemies;
	std::vector <Enemy*>				mpActiveEnemies;
	std::vector <Collectible*>			mpAllCollectibles;
	std::vector <Collectible*>			mpActiveCollectibles;
	std::vector <CircleEffect*>			mpBackgroundEffects;

	EntityQuadTree mStaticEntities = EntityQuadTree(); //static platforms, area effect platforms
	EntityQuadTree mDynamicEntities = EntityQuadTree(); //active nonstatic platforms, enemies, collectibles

	std::string  mArtFileName;
	SDL_Surface* mArtFileSurface;
	SDL_Texture* mArtFileTexture;

	Level(int levelNumber, Vect2 playerStartingPosition, LevelInfo levelInfo);

	~Level();

	void resetStats();

	void resetToCheckpoint();

	void setUpActiveEntitiesVectors();

	void resetDynamicQuadTree();

	void removeInactiveEntitiesFromActiveEntitiesVector();

	std::vector <Platform*> getAllPlatforms() const;

	std::vector <Platform*> getAllActivePlatforms() const;

	std::vector <Entity*> getAllActiveEntities() const;

	void setUp(SDL_Renderer* pRenderer);

private:
	void setUpArtFileTexture(SDL_Renderer* pRenderer);
};

class LevelChunk
{
private:

public:
	Hitbox mHitbox;

	LevelChunk(CoordsX1Y1WidthHeight coords)
	{
		mHitbox = Hitbox(coords);
	}

	LevelChunk()
	{
		mHitbox = {};
	}

	void updateCoords(Vect2 newVect2)
	{
		mHitbox.setTopLeft(newVect2);
	}

	Hitbox getHitbox() const
	{
		return mHitbox;
	}
};

class World
{
public:
	int mWorldNumber;
	std::vector <Level*> mpLevels;

	World(int worldNumber)
	{
		mWorldNumber = worldNumber;
	}
};