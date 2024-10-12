#pragma once
#include "playerHelper.h"
#include "worldHelper.h"
#include "projectileHelper.h"
#include "screenHelper.h"
#include "collisionHelper.h"
#include "damageHelper.h"
#include "platformHelper.h"
#include "entityHelper.h"
#include "enemyHelper.h"
#include "movementHelper.h"
#include "movementHelperClass.h"
#include "movementJump.h"
#include "slashHelper.h"

struct WorldData
{
    Player						mPlayer;
    std::vector <World *>       mpWorlds;
    int							mCurWorldNumber = 0;
    int                         mCurLevelNumber = 0;
    LevelChunk					mCurLevelChunk;
    std::vector <Projectile *>  mpProjectiles;
    int                         mProjectileLimit      = 3;
    const int					mMaxProjectileLimit   = 7;
    int                         mNumLotusCollectibles = 0;
    ScreenObject&               mScreen;

    WorldData(ScreenObject& screen);

    ~WorldData();

    void entityPreTickUpdateMovement(SlashManager& slashManager);

    void createLevelChunk();

    void updateCurLevelChunk();

    void updateBackgroundEffects();

    void entityCollisions(CollisionManager& collisionManager, DamageManager& damageManager, SlashManager& slashManager, KeyboardData& keyboardData);

    void updatePlayerCollisions(CollisionManager& collisionManager, DamageManager& damageManager);

    bool canPlayerWallJump();

    void collideWithWorld(Entity* curEntity);

    void collideWithPlatforms(CollisionManager& collisionManager, DamageManager& damageManager, Entity* curEntity);

    void collideWithNonStaticPlatforms(CollisionManager& collisionManager, DamageManager& damageManager, Entity* curEntity);

    void collideWithEnemies(CollisionManager& collisionManager, DamageManager& damageManager, Entity* curEntity);

    void checkIfOnEdgeOfPlatform(CollisionManager& collisionManager, Enemy* curEnemy);

    void collectedCollectible(Collectible* curCollectible);

    void playerShootProjectile(EEntityMovementPath path);

    void playerSwordSlash(SlashManager& slashManager);

    void enemyShootProjectile(Enemy* pCurEnemy);

    void entityPostTick();

    int  getNumPlayerProjectiles();

    void clearDeadProjectiles();

    void updatePermanentCollectibles();

    void killedCharacter(Entity* pCharacterKilled, bool instantDeath);

    void saveInGameStats();

    void resetStats();

    void resetToCheckpoint();

    void resetBaseStats();

    CanGoToNextLevelResults canGoToNextLevel();

    void setNextLevel(int nextWorldNumber, int nextLevelNumber);

    void renderTexture(SDL_Texture* pHitboxTexture, Hitbox& hitbox, Vect2 imageOffset);
    void renderTexture(SDL_Texture* pHitboxTexture, Hitbox& hitbox, EImageOffset offsetType, Vect2 imageOffset, EDirection entityFacingDirection, bool rotating, float rotation);

    void renderEntityWithHitbox(Entity* pCurEntity);
    void renderEntityWithHitbox(ImageObject* pCurImageObject, SDL_Texture* pHitboxTexture, EImageOffset offsetType, EDirection entityFacingDirection, Hitbox& entityHitbox, bool rotating, float rotation);

    void renderEntityViaChunk(Entity* pCurEntity);
    void renderEntityViaChunk(ImageObject* pCurImageObject, Hitbox& entityHitbox);

    void renderEntityViaSplice(Entity* pCurEntity);
    void renderEntityViaSplice(AnimationManager& animationManager, Hitbox& entityHitbox);

    void renderEntity(Entity* pCurEntity);
    void renderEntity(ImageObject* pCurImageObject, EImageOffset offsetType, 
        EDirection entityFacingDirection, Hitbox& entityHitbox);
    void renderEntity(ImageObject* pCurImageObject, EImageOffset offsetType, EDirection entityFacingDirection, Hitbox& entityHitbox, bool rotating, float rotation);

    void renderBackgrounds();

    void renderBackgroundEffects();

    void renderCircleGradient(SDL_Color color, Vect2 center, int radius);

};
