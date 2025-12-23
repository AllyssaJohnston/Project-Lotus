#include "collisionManager.h"

Collision::Collision(Entity& entity1, Entity& entity2) : mEntity1(entity1), mEntity2(entity2) { ; }

bool Collision::operator== (const Collision& other) const { return (&mEntity1 == &other.mEntity1 && &mEntity2 == &other.mEntity2) or (&mEntity1 == &other.mEntity2 && &mEntity2 == &other.mEntity1); }

bool Collision::operator!= (const Collision& other) const { return !(*this == other); }



RidingIsland::RidingIsland(Entity& ridingObject, Entity& objectToRide)
{
    mpTop  = &ridingObject;
    mpBase = &objectToRide;
    mpRidingContacts.push_back(&ridingObject);
    mpRidingContacts.push_back(&objectToRide);
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

void RidingIsland::addRidingContact(Entity& entity)
{
    if (std::find(mpRidingContacts.begin(), mpRidingContacts.end(), &entity) != mpRidingContacts.end())
    {
        // already in list
        return;
    }
    
    if (entity.getMovementManager().getHitbox().getBottomRight().getY() > mpBase->getMovementManager().getHitbox().getBottomRight().getY())
    {
        mpBase = &entity;
    }
    if (entity.getMovementManager().getHitbox().getTopLeft().getY() < mpTop->getMovementManager().getHitbox().getTopLeft().getY())
    {
        mpTop = &entity;
    }
    for (int count = 0; count < mpRidingContacts.size(); count++)
    {
        if (entity.getMovementManager().getHitbox().getCenter().getY() <= mpRidingContacts[count]->getMovementManager().getHitbox().getCenter().getY())
        {
            mpRidingContacts.insert(mpRidingContacts.cbegin() + count, &entity);
            return;
        }
    }
    mpRidingContacts.push_back(&entity);
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
    for (Collision& curCollision : mCollisionsToSeparate)
    {
        bool doSeparate = true;
        EBoxSide separationPath = curCollision.mEntity1.getMovementManager().getHitbox().separate(curCollision.mEntity2.getMovementManager().getHitbox(), doSeparate);
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
    mRidingIslands.clear();
    mLastFrameCollisions.clear();
    for (Collision collision : mThisFrameCollisions) {
        mLastFrameCollisions.push_back(collision);
    }
    mThisFrameCollisions.clear();
}

bool CollisionManager::isInThisFrameCollisions(const Entity& curEntity) const
{
    for (const Collision& curCollision : mThisFrameCollisions)
    {
        if (&curCollision.mEntity1 == &curEntity or &curCollision.mEntity2 == &curEntity)
        {
            return true;
        }
    }
    return false;
}

void CollisionManager::entitiesCollidedHorizontal(Entity& curEntity1, Entity& curEntity2)
{
    Entity* pMovingEntity = nullptr;
    if      (curEntity1.getClassType() == EEntityClassTypes_ENEMY or curEntity1.getCharacterType() == EEntityCharacterTypes_P_MOVING_PLATFORM)
    {
        pMovingEntity = &curEntity1;
    }
    else if (curEntity2.getClassType() == EEntityClassTypes_ENEMY or curEntity2.getCharacterType() == EEntityCharacterTypes_P_MOVING_PLATFORM)
    {
        pMovingEntity = &curEntity2;
    }
    else
    {
        return;
    }

    if (!isInThisFrameCollisions(*pMovingEntity))
    {
        mThisFrameCollisions.push_back(Collision(curEntity1, curEntity2));
    }
    
    pMovingEntity = nullptr;


    EDirection entity1DirectionToSetTo;
    EDirection entity2DirectionToSetTo;
    EDirection entity1DirectionOfCollision;
    EDirection entity2DirectionOfCollision;
    if (curEntity1.getMovementManager().getHitbox().getTopLeft().getX() < curEntity2.getMovementManager().getHitbox().getTopLeft().getX())
    {
        // 1 is to the left of 2
        entity1DirectionOfCollision = EDirection_RIGHT;
        entity2DirectionOfCollision = EDirection_LEFT;
        entity1DirectionToSetTo = EDirection_LEFT;
        entity2DirectionToSetTo = EDirection_RIGHT;
    }
    else
    {
        // 2 is to the left of 1
        entity1DirectionOfCollision = EDirection_LEFT;
        entity2DirectionOfCollision = EDirection_RIGHT;
        entity1DirectionToSetTo = EDirection_RIGHT;
        entity2DirectionToSetTo = EDirection_LEFT;
    }

    if (curEntity1.getClassType() == EEntityClassTypes_ENEMY  or curEntity1.getCharacterType() == EEntityCharacterTypes_P_MOVING_PLATFORM)
    {
        if (curEntity1.getMovementManager().getCurDirection() != entity1DirectionToSetTo)
        {
            curEntity1.getMovementManager().collided(entity1DirectionOfCollision);
			if (curEntity1.getMovementManager().getDidSwitchedDir())
            {
                curEntity1.setTrapped();
            }
        }
    }
    if (curEntity2.getClassType() == EEntityClassTypes_ENEMY  or curEntity2.getCharacterType() == EEntityCharacterTypes_P_MOVING_PLATFORM)
    {
        if (curEntity2.getMovementManager().getCurDirection() != entity2DirectionToSetTo)
        {
            curEntity2.getMovementManager().collided(entity2DirectionOfCollision);
			if (curEntity2.getMovementManager().getDidSwitchedDir())
            {
                curEntity2.setTrapped();
            }
        }
    }
}

void CollisionManager::entitiesCollidedVertical(Entity& curEntity1, Entity& curEntity2)
{
    EDirection entity1DirectionToSetTo;
    EDirection entity2DirectionToSetTo;
    EDirection entity1DirectionOfCollision;
    EDirection entity2DirectionOfCollision;
    if (curEntity1.getMovementManager().getHitbox().getTopLeft().getY() < curEntity2.getMovementManager().getHitbox().getTopLeft().getY())
    {
        RidingIsland* pEntity1Island = returnLastFrameRidingIsland(curEntity1);
        RidingIsland* pEntity2Island = returnLastFrameRidingIsland(curEntity2);
        if (pEntity2Island != nullptr and (pEntity1Island == pEntity2Island))
        {
            // in riding island don't collide
            return;
        }
        // 1 is above 2
        entity1DirectionOfCollision = EDirection_DOWN;
        entity2DirectionOfCollision = EDirection_UP;
        entity1DirectionToSetTo = EDirection_UP;
        entity2DirectionToSetTo = EDirection_DOWN;
    }
    else
    {
        RidingIsland* pEntity1Island = returnLastFrameRidingIsland(curEntity1);
        RidingIsland* pEntity2Island = returnLastFrameRidingIsland(curEntity2);
        if (pEntity1Island != nullptr and (pEntity1Island == pEntity2Island))
        {
            // in riding island don't collide
            return;
        }
        // 2 is above 1
        entity1DirectionOfCollision = EDirection_UP;
        entity2DirectionOfCollision = EDirection_DOWN;
        entity1DirectionToSetTo = EDirection_DOWN;
        entity2DirectionToSetTo = EDirection_UP;
    }

    if (curEntity1.getClassType() == EEntityClassTypes_ENEMY or curEntity1.getCharacterType() == EEntityCharacterTypes_P_MOVING_PLATFORM)
    {
        if (curEntity1.getMovementManager().getCurDirectionY() != entity1DirectionToSetTo)
        {
            curEntity1.getMovementManager().collided(entity1DirectionOfCollision);
        }
    }
    if (curEntity2.getClassType() == EEntityClassTypes_ENEMY or curEntity2.getCharacterType() == EEntityCharacterTypes_P_MOVING_PLATFORM)
    {
        if (curEntity2.getMovementManager().getCurDirectionY() != entity2DirectionToSetTo)
        {
           curEntity2.getMovementManager().collided(entity2DirectionOfCollision);
        }
    }
}

void CollisionManager::addCollision(Entity& curEntity1, Entity& curEntity2) 
{
    Collision collisionToAdd = Collision(curEntity1, curEntity2);
    if (std::find(mThisFrameCollisions.begin(), mThisFrameCollisions.end(), collisionToAdd) == mThisFrameCollisions.end())
    {
        // unique collision
        mThisFrameCollisions.push_back(collisionToAdd);
    }
}

void CollisionManager::moveRidingIslands()
{
    for (RidingIsland& curRidingIsland : mRidingIslands)
    {
        curRidingIsland.moveRidingContacts();
    }
}

void CollisionManager::moveLastFrameRidingIslands()
{
    for (RidingIsland& curRidingIsland : mLastFrameRidingIslands)
    {
        curRidingIsland.moveRidingContacts();
    }
}

void CollisionManager::addCrateContact(Entity& crate)
{
    if (std::find(mpCrateContacts.begin(), mpCrateContacts.end(), &crate) == mpCrateContacts.end())
    {
        // not already in the list
        mpCrateContacts.push_back(&crate);
    }
}

bool CollisionManager::isInCrateContacts(const Entity& crate) const { return std::find(mpCrateContacts.begin(), mpCrateContacts.end(), &crate) != mpCrateContacts.end(); }

void CollisionManager::setCrateContactMovementIncrement(int movementIncrementInput) { mCrateContactMovementIncrement = movementIncrementInput; }

void CollisionManager::addRidingContact(Entity& ridingObject, Entity& objectToRide)
{
    if (objectToRide.getType() != EEntityType_NON_STATIC or !objectToRide.mRideable) 
    {
        // can't ride this object
        return;
    }
    Hitbox ridingObjectHitbox = ridingObject.getMovementManager().getHitbox();
    Hitbox objectToRideHitbox = objectToRide.getMovementManager().getHitbox();
    int xOverlap = rangeOverlapDistance(ridingObjectHitbox.getTopLeft().getX(), ridingObjectHitbox.getBottomRight().getX(), objectToRideHitbox.getTopLeft().getX(), objectToRideHitbox.getBottomRight().getX());
    int ridingObjectMovement = ridingObject.getMovementManager().getMovementVect2().getX();
    int objectToRideMovement = objectToRide.getMovementManager().getMovementVect2().getX();
    int greatestMovementIncrement = std::max(ridingObjectMovement, objectToRideMovement);
    if (xOverlap >= greatestMovementIncrement)
    {
        RidingIsland* ridingIsland = returnRidingIslandEntityIn(ridingObject);
        if (ridingIsland != nullptr)
        {
            ridingIsland->addRidingContact(objectToRide);
            return;
        }
        ridingIsland = returnRidingIslandEntityIn(objectToRide);
        if (ridingIsland != nullptr)
        {
            ridingIsland->addRidingContact(ridingObject);
            return;
        }
        mRidingIslands.push_back(RidingIsland(ridingObject, objectToRide));
    }
}

RidingIsland* CollisionManager::returnRidingIslandEntityIn(const Entity& entity)
{
    for (RidingIsland& curIsland : mRidingIslands)
    {      
        for (Entity* pCurContact : curIsland.mpRidingContacts)
        {
            if (&entity == pCurContact)
            {
                return &curIsland;
            }
        }
    }
    return nullptr;
}

RidingIsland* CollisionManager::returnLastFrameRidingIsland(const Entity& entity)
{
    for (RidingIsland& curIsland : mLastFrameRidingIslands)
    {
        for (Entity* pCurContact : curIsland.mpRidingContacts)
        {
            if (&entity == pCurContact)
            {
                return &curIsland;
            }
        }
    }
    return nullptr;
}

void CollisionManager::updateLastFrameRidingIslands() { mLastFrameRidingIslands = mRidingIslands; }