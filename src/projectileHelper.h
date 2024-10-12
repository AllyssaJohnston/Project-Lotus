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

	void preTick();

    void tick();

	void postTick();

	void updateAnimationManager();

    bool isAlive() const;

    void takeDamage();

    void died();

	void setCheckpointStats();

	CCharacterPreset* getPreset();

    std::string getHostName();
};