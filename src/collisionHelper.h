#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <algorithm>
#include "helperClass.h"
#include "entityHelper.h"

class Collision
{
public:
    Entity* mpEntity1;
    Entity* mpEntity2;

    Collision(Entity * entity1, Entity * entity2);

    ~Collision();
};

class RidingIsland
{
public:
    Entity* mpBase;
    Entity* mpTop;
    std::vector <Entity*> mpRidingContacts;

    RidingIsland(Entity* ridingObject, Entity* objectToRide);

    ~RidingIsland();

    void moveRidingContacts();

    void separateRidingContacts();

    void addRidingContact(Entity* entity);

};

class CollisionManager
{
private:
    int mCrateContactMovementIncrement  = 0;
    int mCrateInactiveFrames            = 0;

public:
    std::vector <Entity* >       mpCrateContacts;
    std::vector <RidingIsland >  mpLastFrameRidingIslands;
    std::vector <RidingIsland >  mpRidingIslands;
    std::vector <Collision>      mCollisionsToSeparate;
    std::vector <Collision>      mThisFrameCollisions;
    std::vector <Collision>      mLastFrameCollisions;

    CollisionManager();

    ~CollisionManager();

    void postTick();

    void entitiesCollidedHorizontal(Entity* pCurEntity1, Entity* pCurEntity2);

    void entitiesCollidedVertical(Entity* pCurEntity1, Entity* pCurEntity2);

    bool isInThisFrameCollisions(Entity* pCurEntity) const;

    void moveRidingIslands();

    void moveLastFrameRidingIslands();

    void addCrateContact(Entity* crate);

    void setCrateContactMovementIncrement(int movementIncrementInput);

    void addRidingContact(Entity* ridingObject, Entity* objectToRide);

    RidingIsland* returnRidingIslandEntityIn(Entity* entity);

    RidingIsland* returnLastFrameRidingIsland(Entity* entity);

    void updateLastFrameRidingIslands();

};