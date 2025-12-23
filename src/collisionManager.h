#pragma once
#include <vector>
#include "helpers.h"
#include "entity.h"

struct Collision
{
    Entity& mEntity1;
    Entity& mEntity2;

    Collision(Entity& entity1, Entity& entity2);

    bool operator== (const Collision& other) const;

    bool operator!= (const Collision& other) const;
};

struct RidingIsland
{
    Entity* mpBase;
    Entity* mpTop;
    std::vector <Entity*> mpRidingContacts;

    RidingIsland(Entity& ridingObject, Entity& objectToRide);

    ~RidingIsland();

    void moveRidingContacts();

    void separateRidingContacts();

    void addRidingContact(Entity& entity);
};

class CollisionManager
{
private:
    int mCrateContactMovementIncrement  = 0;
    int mCrateInactiveFrames            = 0;

public:
    std::vector <Entity* >       mpCrateContacts;
    std::vector <RidingIsland >  mLastFrameRidingIslands;
    std::vector <RidingIsland >  mRidingIslands;
    std::vector <Collision>      mCollisionsToSeparate;
    std::vector <Collision>      mThisFrameCollisions;
    std::vector <Collision>      mLastFrameCollisions;

    CollisionManager();

    ~CollisionManager();

    void postTick();

    void addCollision(Entity& curEntity1, Entity& curEntity2);

    void entitiesCollidedHorizontal(Entity& pCurEntity1, Entity& pCurEntity2);

    void entitiesCollidedVertical(Entity& curEntity1, Entity& curEntity2);

    bool isInThisFrameCollisions(const Entity& curEntity) const;

    void moveRidingIslands();

    void moveLastFrameRidingIslands();

    void addCrateContact(Entity& crate);

    bool isInCrateContacts(const Entity& crate) const;

    void setCrateContactMovementIncrement(int movementIncrementInput);

    void addRidingContact(Entity& ridingObject, Entity& objectToRide);

    RidingIsland* returnRidingIslandEntityIn(const Entity& entity);

    RidingIsland* returnLastFrameRidingIsland(const Entity& entity);

    void updateLastFrameRidingIslands();
};