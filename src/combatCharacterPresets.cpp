#include "combatCharacterPresets.h"

PlayerCombatPreset::PlayerCombatPreset() : CombatCharacterPreset()
{
    mType = EMiniGameCombatCharacterType_PLAYER;
    mAttacks.push_back( Attack(new AttackSwordOverheadStrike()));
    mAttacks.push_back( Attack(new AttackSwordSwipe()));

    mMoveType = EMiniGameCombatMoveAttackTypes_CROSS2;

    mAttackDamage       = 12;
    mHealthCapacity     = 30;
    mDefenseCapacity    = 20;

}

SwordCombatPreset::SwordCombatPreset() : CombatCharacterPreset()
{
    mType = EMiniGameCombatCharacterType_ENEMY;
    mAttacks.push_back( Attack(new AttackSwordOverheadStrike()));
    mAttacks.push_back( Attack(new AttackSwordSwipe()));

    mMoveType = EMiniGameCombatMoveAttackTypes_CROSS2;

    mAttackDamage       = 10;
    mHealthCapacity     = 36;
    mDefenseCapacity    = 6;

}