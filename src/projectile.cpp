#include "projectile.h"

Projectile::Projectile(const Vect2 positionInput, const ProjectilePreset& preset, EDirection curDirection,
        std::string hostName) : Entity(), mHostName(hostName)
{
    setUpBaseStats(preset);

    mAnimationManager.setupAnimationManager(preset.mAnimationPresets, EHowToDetermineWidthHeight_GET_BEST_IMAGE_RATIO);
    mMovementManager.setupMovementManager(positionInput, preset, curDirection);
    mMovementManager.setCurFacingDirection(curDirection);
    if (DEBUG)
    {
        mImageObjectHitbox.setupImageObject("blue.bmp", preset.mWidth, preset.mHeight, EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT);
    }
}

Projectile::Projectile(const Vect2 positionInput, const Vect2 movementVect, const ProjectilePreset& preset,
        EDirection curDirectionX, EDirection curDirectionY, std::string hostName) : Entity(), mHostName(hostName)
{
    setUpBaseStats(preset);

    mAnimationManager.setupAnimationManager(preset.mAnimationPresets, EHowToDetermineWidthHeight_GET_BEST_IMAGE_RATIO);
    mMovementManager.setupMovementManager(positionInput, movementVect, preset, curDirectionX, curDirectionY);
    mMovementManager.setCurFacingDirection(curDirectionX);
    if (DEBUG)
    {
        mImageObjectHitbox.setupImageObject("blue.bmp", preset.mWidth, preset.mHeight, EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT);
    }
}

Projectile::~Projectile() { Entity::~Entity(); }

void Projectile::preTick()
{
    Entity::preTick();
    mMovementManager.preTick();
}

void Projectile::tick()
{
    Entity::tick();
    mMovementManager.calcMovement();
}

void Projectile::postTick()
{
    Entity::postTick();
    updateAnimationManager();
    mMovementManager.postTick();
    mMovementManager.setMovementStateToCharacterMode();
}

void Projectile::updateAnimationManager() { mAnimationManager.updateAnimation(mAnimationManager.getCurAnimation()->mAnimationType); }

void Projectile::takeDamage() { died(); }

void Projectile::died() { mAmAlive = false; }

std::string Projectile::getHostName() const { return mHostName; }