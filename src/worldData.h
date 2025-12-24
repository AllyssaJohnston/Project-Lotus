#pragma once
#include <corecrt_math_defines.h>
#include "globals.h"
#include "player.h"
#include "world.h"
#include "projectile.h"
#include "screen.h"
#include "collisionManager.h"
#include "damageManager.h"
#include "platform.h"
#include "entity.h"
#include "enemy.h"
#include "levelEffects.h"
#include "movementManager.h"
#include "movementHelpers.h"
#include "slashManager.h"


class WorldData
{
public:
    Player						mPlayer;
    std::vector <World*>       mpWorlds;
#if DEMO == 0
    int							mCurWorldNumber = 0;
    int                         mCurLevelNumber = 6;
#else
    int							mCurWorldNumber = 0;
    int                         mCurLevelNumber = 0;
#endif
    LevelChunk					mCurLevelChunk;
    std::vector <Projectile*>   mpProjectiles;
    int                         mProjectileLimit      = 3;
    const int					mMaxProjectileLimit   = 7;
    int                         mNumLotusCollectibles = 0;
    ScreenObject&               mScreen;
    bool                        mGoToNextLevel = false;
    LevelData*                  mpNextLevelData = nullptr;

private:
    SlashManager&       mSlashManager;
    CollisionManager&   mCollisionManager;

public:


    WorldData(ScreenObject& screen, SlashManager& slashManager, CollisionManager& collisionManager);

    ~WorldData();

    void getAllDynamicEntities(std::vector<Entity*>& pDynamicEntities, const Hitbox& hitbox);

    void getAllStaticEntities(std::vector<Entity*>& pStaticEntities, const Hitbox& hitbox);

    void entityPreTickCalcMovement();

    void createLevelChunk();

    void updateCurLevelChunk();

    void updateBackgroundEffects();

// old collision system
#if COLLISION_SYSTEM == 0

    void updateNonstaticMovement(std::vector<Entity*>& pNonstaticEntities);

    void entityCollisions();

    void updatePlayerCollisions();

    void collideWithWorld(Entity& curEntity);

    void collideWithPlatforms(Entity& curEntity);

    void collideWithNonStaticPlatforms(Entity& curEntity);

    void collideWithEnemies(Entity& curEntity);

    void checkIfOnEdgeOfPlatform(Enemy& curEnemy);

    void collectedCollectible(Collectible& collectible);

// new collision system
#elif COLLISION_SYSTEM == 1
    void entityCollisions();

    void collideWithWorld(Entity& curEntity);

    void updateNonstaticCollisions();

    void runNonstaticCollisions(std::vector<Entity*>& pNonstaticEntities, bool& interrupted);

    void updateNonstaticMovement(std::vector<Entity*>& pEntitiesToMove);

    bool updateNonstaticCollisionEffects(std::vector<Entity*>& pNonstaticEntities);

    void collideWithPlayer(Collision& curCollision);

    void collideWithPlatform(Collision& curCollision);

    void collideWithNonStaticPlatform(Collision& curCollision);

    void collideWithEnemy(Collision& curCollision);

    void collideWithProjectile(Collision& curCollision);

    void slashCollisions();

    void checkIfOnEdgeOfPlatform(Collision& curCollision);

    bool collectedCollectible(Collectible& curCollectible);
#endif

    void playerShootProjectile(EEntityMovementPath path);

    void playerSwordSlash();

    void enemyShootProjectile(Enemy& curEnemy);

    void entityPostTick();

    int  getNumPlayerProjectiles();

    void clearDeadProjectiles();

    void updatePermanentCollectibles();

    void killedCharacter(Entity& characterKilled, bool instantDeath = false);

    void saveInGameStats();

    void resetStats();

    void resetToCheckpoint();

    void resetBaseStats();

    CanGoToNextLevelResults canGoToNextLevel();

    void setNextLevel(int nextWorldNumber, int nextLevelNumber);

    void renderTexture(SDL_Texture* pHitboxTexture, const Hitbox& hitbox, const Vect2& imageOffset);
    void renderTexture(SDL_Texture* pHitboxTexture, const Hitbox& hitbox, EImageOffset offsetType, const Vect2& imageOffset, EDirection entityFacingDirection, bool rotating, float rotation);

    void renderEntityWithHitbox(Entity& curEntity);
    void renderEntityWithHitbox(const ImageObject& curImageObject, SDL_Texture* pHitboxTexture, EImageOffset offsetType, EDirection entityFacingDirection, const Hitbox& entityHitbox, 
            bool rotating, float rotation);

    void renderEntityViaChunk(Entity& curEntity);
    void renderEntityViaChunk(const ImageObject& curImageObject, const Hitbox& entityHitbox);

    void renderEntityViaSplice(Entity& curEntity);
    void renderEntityViaSplice(const AnimationManager& animationManager, const Hitbox& entityHitbox);

    void renderEntity(Entity& curEntity);
    void renderEntity(const ImageObject& curImageObject, EImageOffset offsetType, EDirection entityFacingDirection, const Hitbox& entityHitbox);
    void renderEntity(const ImageObject& curImageObject, EImageOffset offsetType, EDirection entityFacingDirection, const Hitbox& entityHitbox, bool rotating, float rotation);

    void renderBackgrounds();

    void renderBackgroundEffects();

    void renderCircleGradient(const SDL_Color& color, const Vect2& center, int radius);

};
