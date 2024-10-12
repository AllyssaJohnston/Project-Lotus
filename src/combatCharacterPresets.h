#pragma once
#include "combatAttackHelper.h"
#include "combatAttackPreset.h"
#include "combatHelperClass.h"
#include <vector>

class CombatCharacterPreset
{
public:
    EMiniGameCombatCharacterType mType          = EMiniGameCombatCharacterType_INVALID;
    std::vector <Attack> mAttacks;

    EMiniGameCombatMoveAttackTypes mMoveType    = EMiniGameCombatMoveAttackTypes_INVALID;

    int mAttackDamage                           = -1;
    int mHealthCapacity                         = -1;
    int mDefenseCapacity                        = -1;
};

class PlayerCombatPreset : public CombatCharacterPreset
{
public:
    PlayerCombatPreset();
};

class SwordCombatPreset : public CombatCharacterPreset
{
public:
    SwordCombatPreset();
};


/*
class SpearCombatEnemyPreset():
    def __init__(self):
        self.type          = EMiniGameCombatCharacterType.ENEMY.value
        self.attackTypes   = list()
        self.attackTypes.append(AttackSpearStrike())
        self.attackTypes.append(AttackSpearToss())
        self.moveType      = EMiniGameCombatMoveAttackTypes.CROSS2.value

        self.attack  = 10
        self.health  = 36
        self.defense = 6

class BowCombatEnemyPreset():
    def __init__(self):
        self.type          = EMiniGameCombatCharacterType.ENEMY.value
        self.attackTypes   = list()
        self.attackTypes.append(AttackBowClose())
        self.attackTypes.append(AttackBowFar())
        self.moveType      = EMiniGameCombatMoveAttackTypes.CROSS2.value

        self.attack  = 10
        self.health  = 36
        self.defense = 6

class SpellCombatEnemyPreset():
    def __init__(self):
        self.type          = EMiniGameCombatCharacterType.ENEMY.value
        self.attackTypes   = list()
        self.attackTypes.append(AttackSpell())
        self.attackTypes.append(AttackSpell())
        self.moveType      = EMiniGameCombatMoveAttackTypes.CROSS2.value

        self.attack  = 10
        self.health  = 36
        self.defense = 6*/