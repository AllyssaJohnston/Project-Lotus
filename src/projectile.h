#pragma once
#include <vector>
#include "entity.h"
#include "projectilePresets.h"

class Projectile : public Entity
{
private:
    std::string mHostName = "";
public:
    Projectile(const Vect2 positionInput, const ProjectilePreset& preset, EDirection curDirection, std::string hostName);

    Projectile(const Vect2 positionInput, const Vect2 movementVect, const ProjectilePreset& preset, EDirection curDirectionX, EDirection curDirectionY, const std::string hostName);

    ~Projectile();

	void preTick() override;

    void tick() override;

	void postTick() override;

	void updateAnimationManager() override;

    void takeDamage() override;

    void died() override;

    void setCheckpointStats() override { ; } // no projectile checkpoint

    std::string getHostName() const;
};