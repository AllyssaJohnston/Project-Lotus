#include "projectileHelper.h"

Projectile::Projectile(Vect2 positionInput, ProjectilePreset* preset, EDirection curDirection, std::string hostName) : Entity()
{
    setUpBaseStats(preset);

    mAnimationManager.setupAnimationManager(preset->mAnimationPresets, EHowToDetermineWidthHeight_GET_BEST_IMAGE_RATIO);
    mMovementManager.setupMovementManager(positionInput, preset, curDirection);
    mMovementManager.setCurFacingDirection(curDirection);
    mImageObjectHitbox.setupImageObject("blue.bmp", preset->mWidth, preset->mHeight, EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT);

    mpPreset = preset;
    hostName = mHostName;

}

Projectile::Projectile(Vect2 positionInput, Vect2 movementVect, ProjectilePreset* preset, EDirection curDirectionX, EDirection curDirectionY, 
        std::string hostName) : Entity()
{
    setUpBaseStats(preset);

    mAnimationManager.setupAnimationManager(preset->mAnimationPresets, EHowToDetermineWidthHeight_GET_BEST_IMAGE_RATIO);
    mMovementManager.setupMovementManager(positionInput, movementVect, preset, curDirectionX, curDirectionY);
    mMovementManager.setCurFacingDirection(curDirectionX);
    mImageObjectHitbox.setupImageObject("blue.bmp", preset->mWidth, preset->mHeight, EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT);

    mpPreset = preset;
    mHostName = hostName;

}

Projectile::~Projectile()
{
    mpPreset = nullptr;
    Entity::~Entity();
}

void Projectile::preTick()
{
    Entity::preTick();
    mMovementManager.preTick();
}

void Projectile::tick()
{
    //Entity:tick();
    mMovementManager.updateMovement();
}

void Projectile::postTick()
{
    Entity::postTick();
    updateAnimationManager();
    mMovementManager.postTick();
    mMovementManager.setMovementStateToCharacterMode();
}

void Projectile::updateAnimationManager()
{
    mAnimationManager.updateAnimation(mAnimationManager.getCurAnimation()->mAnimationType);
}

bool Projectile::isAlive() const
{
    return mAmAlive;
}

void Projectile::takeDamage()
{
    died();
}

void Projectile::died()
{
    mAmAlive = false;
}

void Projectile::setCheckpointStats()
{
    //No projectile checkpoint
}

CCharacterPreset* Projectile::getPreset()
{
    return mpPreset;
}

std::string Projectile::getHostName()
{
    return mHostName;
}