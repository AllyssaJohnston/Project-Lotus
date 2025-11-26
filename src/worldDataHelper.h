#pragma once
#include "globals.h"
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
#if DEMO == 0
    int							mCurWorldNumber = 0;
    int                         mCurLevelNumber = 0;
#else
    int							mCurWorldNumber = 0;
    int                         mCurLevelNumber = 0;
#endif
    LevelChunk					mCurLevelChunk;
    std::vector <Projectile *>  mpProjectiles;
    int                         mProjectileLimit      = 3;
    const int					mMaxProjectileLimit   = 7;
    int                         mNumLotusCollectibles = 0;
    ScreenObject&               mScreen;
    bool                        mGoToNextLevel = false;
    LevelData*                  mpNextLevelData = nullptr;

    WorldData(ScreenObject& screen);

    ~WorldData();

    void getAllDynamicEntities(std::vector<Entity*>& dynamicEntities, Hitbox hitbox);

    void getAllStaticEntities(std::vector<Entity*>& staticEntities, Hitbox hitbox);

    void entityPreTickUpdateMovement(SlashManager& slashManager);

    void createLevelChunk();

    void updateCurLevelChunk();

    void updateBackgroundEffects();

#if COLLISION_SYSTEM == 0

    void updateNonstaticMovement(std::vector<Entity*> nonstaticEntities);

    void entityCollisions(CollisionManager& collisionManager, DamageManager& damageManager, SlashManager& slashManager, KeyboardData& keyboardData);

    void updatePlayerCollisions(CollisionManager& collisionManager, DamageManager& damageManager);

    void collideWithWorld(Entity* pCurEntity);

    void collideWithPlatforms(CollisionManager& collisionManager, DamageManager& damageManager, Entity* pCurEntity);

    void collideWithNonStaticPlatforms(CollisionManager& collisionManager, DamageManager& damageManager, Entity* pCurEntity);

    void collideWithEnemies(CollisionManager& collisionManager, DamageManager& damageManager, Entity* pCurEntity);

    void checkIfOnEdgeOfPlatform(CollisionManager& collisionManager, Enemy* pCurEnemy);

    void collectedCollectible(Collectible* curCollectible);

#elif COLLISION_SYSTEM == 1
    void entityCollisions(CollisionManager& collisionManager, DamageManager& damageManager, SlashManager& slashManager, KeyboardData& keyboardData);

    void collideWithWorld(Entity* curEntity);

    void updateNonstaticCollisions(CollisionManager& collisionManager, DamageManager& damageManager);

    void runNonstaticCollisions(CollisionManager& collisionManager, DamageManager& damageManager, std::vector<Entity*> pNonstaticEntities, bool& interrupted);

    void updateNonstaticMovement(std::vector<Entity*> pEntitiesToMove);

    bool updateNonstaticCollisionEffects(CollisionManager& collisionManager, DamageManager& damageManager, std::vector<Entity*> pNonstaticEntities);

    void collideWithPlayer(CollisionManager& collisionManager, DamageManager& damageManager, Collision& curCollision);

    void collideWithPlatform(CollisionManager& collisionManager, DamageManager& damageManager, Collision& curCollision);

    void collideWithNonStaticPlatform(CollisionManager& collisionManager, DamageManager& damageManager, Collision& curCollision);

    void collideWithEnemy(CollisionManager& collisionManager, DamageManager& damageManager, Collision& curCollision);

    void collideWithProjectile(Collision& curCollision);

    void slashCollisions(SlashManager& slashManager);

    void checkIfOnEdgeOfPlatform(CollisionManager& collisionManager, Collision& curCollision);

    bool collectedCollectible(Collectible* curCollectible);
#endif

    void playerShootProjectile(EEntityMovementPath path);

    void playerSwordSlash(SlashManager& slashManager);

    void enemyShootProjectile(Enemy* pCurEnemy);

    void entityPostTick();

    int  getNumPlayerProjectiles();

    void clearDeadProjectiles();

    void updatePermanentCollectibles();

    void killedCharacter(Entity* pCharacterKilled, bool instantDeath = false);

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
