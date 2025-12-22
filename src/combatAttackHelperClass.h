#pragma once
#include "tileHelper.h"
#include "tileHelperClass.h"
#include "tileCoordsHelper.h"
#include "combatHelperClass.h"
#include "combatAttackHelper.h"

struct AttackTile
{
    Tile&  mTile;
    Attack& mAttack;

    AttackTile(Tile& tile, Attack& attack);
};

std::string getAttackType(const Attack& attack);

std::string getAttackDamage(const Attack& attack, int characterDamage);

std::string getSpecialEffect(const Attack& attack);



