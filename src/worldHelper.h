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
	int  mLevelNumber	= -1;
	int  mTrueLevelX2	= -1; // x2 of furthest item in level (for the art file)
	int  mLevelX2		= -1; // might be past the 'true' x2
	int  mTrueLevelY2	= -1; // y2 of furthest item in level (for the art file)
	int  mLevelY2		= -1; // might be past the 'true' y2
	Hitbox mHitbox;

	int mArtFileX = -1;
	int mArtFileY = -1;
	int mArtFileWidth	= -1;
	int mArtFileHeight	= -1;
	bool mDoubleJumpAllowed			= false;
	bool mThrowProjectileAllowed	= false;
	bool mThrowDownwardProjectileAllowed = false;
	bool mSlashAllowed				= false;
	bool mMustKillAllEnemies		= false;

	Vect2 mPlayerStartingPosition;

	std::vector <Platform*>				mpPlatforms;
	std::vector <Platform*>				mpAllNonStaticPlatforms;
	std::vector <Platform*>				mpActiveNonStaticPlatforms;
	std::vector <AreaEffectPlatform*>	mpAreaEffectPlatforms;
	std::vector <Enemy*>				mpAllEnemies;
	std::vector <Enemy*>				mpActiveEnemies;
	std::vector <Collectible*>			mpAllCollectibles;
	std::vector <Collectible*>			mpActiveCollectibles;
	std::vector <CircleEffect*>			mpBackgroundEffects;

	EntityQuadTree mStaticEntities = EntityQuadTree();  // static platforms, area effect platforms
	EntityQuadTree mDynamicEntities = EntityQuadTree(); // active nonstatic platforms, enemies, collectibles, projectiles, + player

	std::string  mArtFileName;
	SDL_Surface* mpArtFileSurface = nullptr;
	SDL_Texture* mpArtFileTexture = nullptr;

	LevelData mNextLevelData;

	Level(int levelNumber, Vect2 playerStartingPosition, LevelInfo levelInfo, LevelData nextLevelData);

	~Level();

	void resetStats();

	void resetToCheckpoint();

	void setUpActiveEntitiesVectors();

	void resetDynamicQuadTree();

	void removeInactiveEntitiesFromActiveEntitiesVector();

	std::vector <Platform*> getAllPlatforms() const;

	std::vector <Platform*> getAllActivePlatforms() const;

	std::vector <Entity*> getAllActiveEntities() const;

	Hitbox& getHitbox();

	void setUp(SDL_Renderer* pRenderer);

private:
	void setUpArtFileTexture(SDL_Renderer* pRenderer);
};

class LevelChunk
{
public:
	Hitbox mHitbox;

	LevelChunk(const CoordsX1Y1WidthHeight& coords);

	LevelChunk();

	void updateCoords(const Vect2& newVect2);

	Hitbox& getHitbox();
};

class World
{
public:
	int mWorldNumber;
	std::vector <Level*> mpLevels;

	World(int worldNumber);
};