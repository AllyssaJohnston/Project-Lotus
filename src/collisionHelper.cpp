#include "collisionHelper.h"

Collision::Collision(Entity * entity1, Entity * entity2)
{
    mpEntity1 = entity1;
    mpEntity2 = entity2;
}

Collision::~Collision()
{
    mpEntity1 = nullptr;
    mpEntity2 = nullptr;
}




RidingIsland::RidingIsland(Entity* pRidingObject, Entity* pObjectToRide)
{
    mpTop  = pRidingObject;
    mpBase = pObjectToRide;
    mpRidingContacts.push_back(pRidingObject);
    mpRidingContacts.push_back(pObjectToRide);
}

RidingIsland::~RidingIsland()
{
    mpTop  = nullptr;
    mpBase = nullptr;
    for (Entity* ridingContact : mpRidingContacts)
    {
        ridingContact = nullptr;
    }
}

void RidingIsland::moveRidingContacts()
{
    if (mpBase->getMovementManager().getPath() == EEntityMovementPath_HORIZONTAL or mpBase->getMovementManager().getPath() == EEntityMovementPath_HORIZONTAL_CAN_FALL)
    {
        int ridingContactMove = mpBase->getMovementManager().getXChange();
        if (ridingContactMove != 0)
        {
            for (Entity* pCurContact : mpRidingContacts)
            {
                if (pCurContact != mpBase)
                {
                    pCurContact->getMovementManager().getHitbox().updateTopLeft(Vect2(ridingContactMove, 0));
                }
            }
        }
    } 
    else
    {
        if (mpBase->getMovementManager().getCurDirectionY() == EDirection_UP)
        {
            for (Entity* pCurContact : mpRidingContacts)
            {
                if (pCurContact != mpBase)
                {
                    pCurContact->getMovementManager().getHitbox().updateTopLeftY(-mpBase->getMovementManager().getMovementVect2().getY());
                }
            }
        }
    }
    separateRidingContacts();
}

void RidingIsland::separateRidingContacts()
{
    EBoxSide axis;
    for (int count = (int)mpRidingContacts.size() - 1; count > 0; count--)
    {
        Entity* entity1 = mpRidingContacts[count];
        Entity* entity2 = mpRidingContacts[count - 1];
        bool doSeparate = true;
        bool above = entity1->getMovementManager().getHitbox().getCenter().getY() < entity2->getMovementManager().getHitbox().getCenter().getY();
        if (above)
        {
            axis = entity1->getMovementManager().getHitbox().separate(entity2->getMovementManager().getHitbox(), doSeparate);
        }
        else
        {
            axis = entity2->getMovementManager().getHitbox().separate(entity1->getMovementManager().getHitbox(), doSeparate);
        }
    }
}

void RidingIsland::addRidingContact(Entity* entity)
{
    for (int count = 0; count < mpRidingContacts.size(); count++)
    {
        Entity& curContact = *(mpRidingContacts[count]);
        if (&curContact == entity)
        {
            return;
        }
    }
    if (entity->getMovementManager().getHitbox().getBottomRight().getY() > mpBase->getMovementManager().getHitbox().getBottomRight().getY())
    {
        mpBase = entity;
    }
    if (entity->getMovementManager().getHitbox().getTopLeft().getY() < mpTop->getMovementManager().getHitbox().getTopLeft().getY())
    {
        mpTop = entity;
    }
    for (int count = 0; count < mpRidingContacts.size(); count++)
    {
        if (entity->getMovementManager().getHitbox().getCenter().getY() <= mpRidingContacts[count]->getMovementManager().getHitbox().getCenter().getY())
        {
            mpRidingContacts.insert(mpRidingContacts.cbegin() + count, entity);
            return;
        }
    }
    mpRidingContacts.push_back(entity);

}




CollisionManager::CollisionManager(){}

CollisionManager::~CollisionManager()
{
    for (Entity* crate : mpCrateContacts)
    {
        delete crate;
    }
}

void CollisionManager::postTick()
{   
    bool moveCrates = true;
    for (int count = 0; count < mCollisionsToSeparate.size(); count++)
    {
        Collision & curCollision = mCollisionsToSeparate[count];
        bool doSeparate = true;
        EBoxSide separationPath = curCollision.mpEntity1->getMovementManager().getHitbox().separate(curCollision.mpEntity2->getMovementManager().getHitbox(), doSeparate);
        if (separationPath == EBoxSide_TOP or separationPath == EBoxSide_BOTTOM)
        {
            moveCrates = false;
        }
    }

    mCollisionsToSeparate.clear();

    if ((mCrateContactMovementIncrement != 0) and moveCrates)
    {
        mCrateInactiveFrames = 0;
        for (Entity* pCurCrate : mpCrateContacts)
        {
            pCurCrate->getMovementManager().getHitbox().updateTopLeftX(mCrateContactMovementIncrement);
        }
    } 
    else
    {
        mCrateInactiveFrames += 1;
        if (mCrateInactiveFrames > 5)
        {
            mpCrateContacts.clear();
            mCrateInactiveFrames = 0;
        }
    }

    mCrateContactMovementIncrement = 0;
    moveRidingIslands();
    updateLastFrameRidingIslands();
    mpRidingIslands.clear();
    mLastFrameCollisions = mThisFrameCollisions;
    mThisFrameCollisions.clear();
}

bool CollisionManager::isInThisFrameCollisions(Entity* pCurEntity) const
{
    for (const Collision& curCollision : mThisFrameCollisions)
    {
        if (curCollision.mpEntity1 == pCurEntity or curCollision.mpEntity2 == pCurEntity)
        {
            return true;
        }
    }
    return false;
}

void CollisionManager::entitiesCollidedHorizontal(Entity* pCurEntity1, Entity* pCurEntity2)
{
    Entity* pMovingEntity;
    if      (pCurEntity1->getClassType() == EEntityClassTypes_ENEMY or pCurEntity1->getCharacterType() == EEntityCharacterTypes_P_MOVING_PLATFORM)
    {
        pMovingEntity = pCurEntity1;
    }
    else if (pCurEntity2->getClassType() == EEntityClassTypes_ENEMY or pCurEntity2->getCharacterType() == EEntityCharacterTypes_P_MOVING_PLATFORM)
    {
        pMovingEntity = pCurEntity2;
    }
    else
    {
        return;
    }

    if (isInThisFrameCollisions(pMovingEntity))
    {
        return;
    }
    mThisFrameCollisions.push_back(Collision(pCurEntity1, pCurEntity2));


    EDirection entity1DirectionToSetTo;
    EDirection entity2DirectionToSetTo;
    EDirection entity1DirectionOfCollision;
    EDirection entity2DirectionOfCollision;
    if (pCurEntity1->getMovementManager().getHitbox().getTopLeft().getX() < pCurEntity2->getMovementManager().getHitbox().getTopLeft().getX())
    {
        //1 is to the left of 2
        entity1DirectionOfCollision = EDirection_RIGHT;
        entity2DirectionOfCollision = EDirection_LEFT;
        entity1DirectionToSetTo = EDirection_LEFT;
        entity2DirectionToSetTo = EDirection_RIGHT;
    }
    else
    {
        //2 is to the left of 1
        entity1DirectionOfCollision = EDirection_LEFT;
        entity2DirectionOfCollision = EDirection_RIGHT;
        entity1DirectionToSetTo = EDirection_RIGHT;
        entity2DirectionToSetTo = EDirection_LEFT;
    }

    if (pCurEntity1->getClassType() == EEntityClassTypes_ENEMY  or pCurEntity1->getCharacterType() == EEntityCharacterTypes_P_MOVING_PLATFORM)
    {
        if (pCurEntity1->getMovementManager().getCurDirection() != entity1DirectionToSetTo)
        {
            pCurEntity1->getMovementManager().collided(entity1DirectionOfCollision);
			if (pCurEntity1->getMovementManager().getSwitchedDir())
            {
                pCurEntity1->isPathBlocked();
            }
        }
    }
    if (pCurEntity2->getClassType() == EEntityClassTypes_ENEMY  or pCurEntity2->getCharacterType() == EEntityCharacterTypes_P_MOVING_PLATFORM)
    {
        if (pCurEntity2->getMovementManager().getCurDirection() != entity2DirectionToSetTo)
        {
            pCurEntity2->getMovementManager().collided(entity2DirectionOfCollision);
			if (pCurEntity2->getMovementManager().getSwitchedDir())
            {
                pCurEntity2->isPathBlocked();
            }
        }
    }
}

void CollisionManager::entitiesCollidedVertical(Entity* pCurEntity1, Entity* pCurEntity2)
{
    EDirection entity1DirectionToSetTo;
    EDirection entity2DirectionToSetTo;
    EDirection entity1DirectionOfCollision;
    EDirection entity2DirectionOfCollision;
    if (pCurEntity1->getMovementManager().getHitbox().getTopLeft().getY() < pCurEntity2->getMovementManager().getHitbox().getTopLeft().getY())
    {
        RidingIsland* pEntity1Island = returnLastFrameRidingIsland(pCurEntity1);
        RidingIsland* pEntity2Island = returnLastFrameRidingIsland(pCurEntity2);
        if (pEntity2Island != nullptr and (pEntity1Island == pEntity2Island))
        {
            //in riding island don't collide
            return;
        }
        //1 is above 2
        entity1DirectionOfCollision = EDirection_DOWN;
        entity2DirectionOfCollision = EDirection_UP;
        entity1DirectionToSetTo = EDirection_UP;
        entity2DirectionToSetTo = EDirection_DOWN;
    }
    else
    {
        RidingIsland* pEntity1Island = returnLastFrameRidingIsland(pCurEntity1);
        RidingIsland* pEntity2Island = returnLastFrameRidingIsland(pCurEntity2);
        if (pEntity1Island != nullptr and (pEntity1Island == pEntity2Island))
        {
            //in riding island don't collide
            return;
        }
        //2 is above 1
        entity1DirectionOfCollision = EDirection_UP;
        entity2DirectionOfCollision = EDirection_DOWN;
        entity1DirectionToSetTo = EDirection_DOWN;
        entity2DirectionToSetTo = EDirection_UP;
    }

    if (pCurEntity1->getClassType() == EEntityClassTypes_ENEMY or pCurEntity1->getCharacterType() == EEntityCharacterTypes_P_MOVING_PLATFORM)
    {
        if (pCurEntity1->getMovementManager().getCurDirectionY() != entity1DirectionToSetTo)
        {
            pCurEntity1->getMovementManager().collided(entity1DirectionOfCollision);
        }
    }
    if (pCurEntity2->getClassType() == EEntityClassTypes_ENEMY or pCurEntity2->getCharacterType() == EEntityCharacterTypes_P_MOVING_PLATFORM)
    {
        if (pCurEntity2->getMovementManager().getCurDirectionY() != entity2DirectionToSetTo)
        {
            pCurEntity2->getMovementManager().collided(entity2DirectionOfCollision);
        }
    }
}

void CollisionManager::moveRidingIslands()
{
    for (RidingIsland& curRidingIsland : mpRidingIslands)
    {
        curRidingIsland.moveRidingContacts();
    }
}

void CollisionManager::moveLastFrameRidingIslands()
{
    for (RidingIsland& curRidingIsland : mpLastFrameRidingIslands)
    {
        curRidingIsland.moveRidingContacts();
    }
}

void CollisionManager::addCrateContact(Entity* crate)
{
    for (Entity* curCrateContact : mpCrateContacts)
    {
        if (curCrateContact == crate)
        {
            return;
        }
    }
    mpCrateContacts.push_back(crate);
}

void CollisionManager::setCrateContactMovementIncrement(int movementIncrementInput)
{
    mCrateContactMovementIncrement = movementIncrementInput;
}

void CollisionManager::addRidingContact(Entity* pRidingObject, Entity* pObjectToRide)
{
    Hitbox ridingObjectHitbox = pRidingObject->getMovementManager().getHitbox();
    Hitbox objectToRideHitbox = pObjectToRide->getMovementManager().getHitbox();
    int xOverlap = rangeOverlapDistance(ridingObjectHitbox.getTopLeft().getX(), ridingObjectHitbox.getBottomRight().getX(), objectToRideHitbox.getTopLeft().getX(), objectToRideHitbox.getBottomRight().getX());
    int ridingObjectMovement = pRidingObject->getMovementManager().getMovementVect2().getX();
    int objectToRideMovement = pObjectToRide->getMovementManager().getMovementVect2().getX();
    int greatestMovementIncrement = std::max(ridingObjectMovement, objectToRideMovement);
    if (xOverlap >= greatestMovementIncrement)
    {
        RidingIsland* ridingIsland = returnRidingIslandEntityIn(pRidingObject);
        if (ridingIsland != nullptr)
        {
            ridingIsland->addRidingContact(pObjectToRide);
            return;
        }
        ridingIsland = returnRidingIslandEntityIn(pObjectToRide);
        if (ridingIsland != nullptr)
        {
            ridingIsland->addRidingContact(pRidingObject);
            return;
        }
        mpRidingIslands.push_back(RidingIsland(pRidingObject, pObjectToRide));
    }
}

RidingIsland* CollisionManager::returnRidingIslandEntityIn(Entity* entity)
{
    for (RidingIsland& curIsland : mpRidingIslands)
    {      
        for (int countContact = 0; countContact < curIsland.mpRidingContacts.size(); countContact++)
        {
            Entity& curContact = *(curIsland.mpRidingContacts[countContact]);
            if (entity == &curContact)
            {
                return &curIsland;
            }
        }
    }
    return nullptr;
}

RidingIsland* CollisionManager::returnLastFrameRidingIsland(Entity* entity)
{
    for (RidingIsland& curIsland : mpLastFrameRidingIslands)
    {
        for (int countContact = 0; countContact < curIsland.mpRidingContacts.size(); countContact++)
        {
            Entity & curContact = *(curIsland.mpRidingContacts[countContact]);
            if (entity == &curContact)
            {
                return &curIsland;
            }
        }
    }
    return nullptr;
}

void CollisionManager::updateLastFrameRidingIslands()
{
    mpLastFrameRidingIslands = mpRidingIslands;
}