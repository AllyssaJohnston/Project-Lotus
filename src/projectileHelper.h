#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "entityHelper.h"
#include "hitboxHelper.h"
#include "projectilePresets.h"

class Projectile : public Entity
{
private:
    ProjectilePreset* mpPreset  = nullptr;
public:
    std::string mHostName = "";

    Projectile(Vect2 positionInput, ProjectilePreset* preset, EDirection curDirection, std::string hostName);

    Projectile(Vect2 positionInput, Vect2 movementVect, ProjectilePreset* preset, EDirection curDirectionX, EDirection curDirectionY, 
            std::string hostName);

    ~Projectile();

	void preTick() override;

    void tick() override;

	void postTick() override;

	void updateAnimationManager() override;

    void takeDamage() override;

    void died() override;

    void setCheckpointStats() override { ; } //no projectile checkpoint

	ProjectilePreset* getPreset();

    std::string getHostName() const override;
};