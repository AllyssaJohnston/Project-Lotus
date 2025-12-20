#include "combatCharacterPresets.h"

PlayerCombatPreset::PlayerCombatPreset() : CombatCharacterPreset()
{
    mType = EMiniGameCombatCharacterType_PLAYER;

    Attack attack1 = Attack(); // OVERHEAD STRIKE
    attack1.mType = EMiniGameCombatMoveAttackTypes_CROSS1;
    attack1.mName = "Overhead Strike";
    attack1.mDescription = "Overhead Strike: CROSS 1";
    attack1.mDamagePercent = 1.0f;
    attack1.mDamageDistanceDependent = false;
    attack1.mRequiresDirectionInput = false;
    attack1.mSpecialEffects.push_back(SpecialEffectStun(1));
    attack1.mSpecialEffects.push_back(SpecialEffectSelfStun(1));
    mAttacks.push_back(attack1);

    Attack attack2 = Attack(); // SWORD SWIPE
    attack2.mType = EMiniGameCombatMoveAttackTypes_SQUARE1;
    attack2.mName = "Sword Swipe";
    attack2.mDescription = "Sword Swipe: Square 1";
    attack2.mDamagePercent = .5f;
    attack2.mDamageDistanceDependent = false;
    attack2.mRequiresDirectionInput = true;
    mAttacks.push_back(attack2);

    mHealAmount = 6;

    mMoveType = EMiniGameCombatMoveAttackTypes_CROSS2;

    mAttackDamage       = 12;
    mHealthCapacity     = 30;
    mDefenseCapacity    = 20;

    mModelFileName = "MiniGameModels/MiniGameModelsLotus.bmp";
    mIconFileName = "MiniGameUI/LotusPortrait.bmp";
}

// DRAGAUR BLOSSOM
RatCombatPreset::RatCombatPreset() : CombatCharacterPreset()
{
    mType = EMiniGameCombatCharacterType_ENEMY;

    Attack attack1 = Attack(); // SLASH
    attack1.mType = EMiniGameCombatMoveAttackTypes_SQUARE2;
    attack1.mName = "Slash";
    attack1.mDescription = "Slash: Square 2";
    attack1.mDamagePercent = .5f;
    attack1.mDamageDistanceDependent = false;
    attack1.mRequiresDirectionInput = true;
    mAttacks.push_back(attack1);

    Attack attack2 = Attack(); // SPIT THORNS
    attack2.mType = EMiniGameCombatMoveAttackTypes_CROSS3;
    attack2.mName = "Spit Thorns";
    attack2.mDescription = "Spit Thorns: Cross 3";
    attack2.mDamagePercent = 1.0f;
    attack2.mDamageDistanceDependent = false;
    attack2.mRequiresDirectionInput = false;
    attack2.mSpecialEffects.push_back(SpecialEffectSelfStun(1));
    mAttacks.push_back(attack2);

    Attack attack3 = Attack(); // DRAGAUR GROWL (Inc attack)
    attack3.mType = EMiniGameCombatMoveAttackTypes_WHOLE_GRID;
    attack3.mName = "Dragaur Growl";
    attack3.mDescription = "Dragaur Growl: Increase enmey attack by 25% for 3 turns";
    attack3.mDamagePercent = 0.0f;
    attack3.mDamageDistanceDependent = false;
    attack3.mRequiresDirectionInput = false;
    attack3.mSpecialEffects.push_back(SpecialEffectAttackMultiplier(1.25, 3, EAttackTargetType_ALL_ENEMIES));
    attack3.mSpecialEffects.push_back(SpecialEffectSelfStun(1));
    mAttacks.push_back(attack3);

    mMoveType = EMiniGameCombatMoveAttackTypes_CROSS2;

    mAttackDamage       = 10;
    mHealthCapacity     = 36;
    mDefenseCapacity    = 8;

    mModelFileName = "MiniGameModels/Earth/MiniGameModelsDragaurBlossom.bmp";
    mIconFileName = "MiniGameUI/Earth/DragaurBlossomPortrait.bmp";
}

// FROCODILE
FrogCombatPreset::FrogCombatPreset() : CombatCharacterPreset()
{
    mType = EMiniGameCombatCharacterType_ENEMY;

    Attack attack1 = Attack(); // JUMP STRIKE
    attack1.mType = EMiniGameCombatMoveAttackTypes_CROSS5;
    attack1.mName = "STRIKE 5";
    attack1.mDescription = "Jump Strike: CROSS 5";
    attack1.mDamagePercent = .5f;
    attack1.mDamageDistanceDependent = true;
    attack1.mRequiresDirectionInput = false;
    mAttacks.push_back(attack1);

    Attack attack2 = Attack(); // SPIT
    attack2.mType = EMiniGameCombatMoveAttackTypes_CROSS2_2UNIT_OUT;
    attack2.mName = "STRIKE 2, 2 OUT";
    attack2.mDescription = "Spit: CROSS 2_2";
    attack2.mDamagePercent = 1.0f;
    attack2.mDamageDistanceDependent = false;
    attack2.mRequiresDirectionInput = false;
    attack2.mSpecialEffects.push_back(SpecialEffect(SpecialEffectStun(1)));
    mAttacks.push_back(attack2);

    mMoveType = EMiniGameCombatMoveAttackTypes_CROSS3;

    mAttackDamage = 8;
    mHealthCapacity = 48;
    mDefenseCapacity = 4;

    mModelFileName = "MiniGameModels/Earth/MiniGameModelsFrocodile.bmp";
    mIconFileName = "MiniGameUI/Earth/FrocodilePortrait.bmp";
}