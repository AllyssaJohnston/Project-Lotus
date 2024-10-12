#pragma once
#include "combatHelperClass.h"

class SpecialEffectPreset
{
public:
    EMiniGameCombatSpecialEffectTypes mType;
    std::string mName;
};

class SpecialEffectStun : public SpecialEffectPreset
{
public:
    SpecialEffectStun();
};




class AttackPreset
{
public:
    EMiniGameCombatMoveAttackTypes      mType;
    float                               mDamagePercent;
    bool                                mDamageDistanceDependent;
    std::vector <SpecialEffectPreset>   mSpecialEffectPresets;
    bool                                mRequiresDirectionInput;
    std::string                         mName;
    std::string                         mDescription;
};

class AttackSwordOverheadStrike : public AttackPreset
{
public:
    AttackSwordOverheadStrike();
};


class AttackSwordSwipe : public AttackPreset
{
public:
    AttackSwordSwipe();
};


/*
class AttackSpearStrike(Attack):
def __init__(self):
self.type                    = EMiniGameCombatMoveAttackTypes.CROSS1_1UNIT_OUT.value
self.name                    = "STRIKE"
self.description             = "Spear Strike: CROSS 1 1 UNIT OUT"
self.damagePercent           = 100
self.damageDistanceDependent = False
self.requiresDirectionInput  = False
self.specialEffects          = list()
self.specialEffects.append(SpecialEffectKnockback())
Attack.__init__(self, self.type, self.name, self.description, self.damagePercent, self.damageDistanceDependent, self.requiresDirectionInput, self.specialEffects)

class AttackSpearToss(Attack):
def __init__(self):
self.type                    = EMiniGameCombatMoveAttackTypes.CROSS4.value
self.name                    = "TOSS"
self.description             = "Spear Toss: CROSS 4"
self.damagePercent           = 100
self.damageDistanceDependent = False
self.requiresDirectionInput  = False
self.specialEffects          = list()
self.specialEffects.append(SpecialEffectLoseTurn())
Attack.__init__(self, self.type, self.name, self.description, self.damagePercent, self.damageDistanceDependent, self.requiresDirectionInput, self.specialEffects)

class AttackBowClose(Attack):
def __init__(self):
self.type                    = EMiniGameCombatMoveAttackTypes.SQUARE2_1UNIT_OUT.value
self.name                    = "CLOSE"
self.description             = "Bow Shoot: SQUARE 2 1 UNIT OUT"
self.damagePercent           = 100
self.damageDistanceDependent = True
self.requiresDirectionInput  = True
self.specialEffects          = list()
Attack.__init__(self, self.type, self.name, self.description, self.damagePercent, self.damageDistanceDependent, self.requiresDirectionInput, self.specialEffects)

class AttackBowFar(Attack):
def __init__(self):
self.type                    = EMiniGameCombatMoveAttackTypes.SQUARE3_2UNITS_OUT.value
self.name                    = "FAR"
self.description             = "Bow Shoot: SQUARE 2 1 UNITS OUT"
self.damagePercent           = 75
self.damageDistanceDependent = True
self.requiresDirectionInput  = True
self.specialEffects          = list()
Attack.__init__(self, self.type, self.name, self.description, self.damagePercent, self.damageDistanceDependent, self.requiresDirectionInput, self.specialEffects)

class AttackSpell(Attack):
def __init__(self):
self.type                    = EMiniGameCombatMoveAttackTypes.CHECKERBOARD2UNITS.value
self.name                    = "STORM"
self.description             = "Storm Spell: CHECKERBOARD 2 UNITS"
self.damagePercent           = 100
self.damageDistanceDependent = True
self.requiresDirectionInput  = False
self.specialEffects          = list()
Attack.__init__(self, self.type, self.name, self.description, self.damagePercent, self.damageDistanceDependent, self.requiresDirectionInput, self.specialEffects)

class SpecialEffectKnockback():
def __init__(self):
self.type = EMiniGameCombatSpecialEffectTypes.KNOCKBACK.value
self.name = "KNOCKBACK"

class SpecialEffectStun():
def __init__(self):
self.type = EMiniGameCombatSpecialEffectTypes.STUN.value
self.name = "STUN"

class SpecialEffectLoseTurn():
def __init__(self):
self.type = EMiniGameCombatSpecialEffectTypes.LOSE_TURN.value
self.name = "LOSE TURN"

*/