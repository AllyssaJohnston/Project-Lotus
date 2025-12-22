#include "combatCharacterPresets.h"

// LOTUS
LotusCombatPreset::LotusCombatPreset() : CombatCharacterPreset()
{
    mType = EMiniGameCombatCharacterType_PLAYER;

    // OVERHEAD STRIKE
    Attack attack1 = Attack(EMiniGameCombatMoveAttackTypes_CROSS1, EMiniGameCombatAttackCategoryType_ATTACK, 1.0f, "Overhead Strike", "Overhead Strike: CROSS 1"); 
    attack1.mDamageDistanceDependent = false;
    attack1.mRequiresDirectionInput = false;
    attack1.mSpecialEffects.push_back(SpecialEffectStun(1));
    attack1.mSpecialEffects.push_back(SpecialEffectSelfStun(1));
    mAttacks.push_back(attack1);

    // SWORD SWIPE
    Attack attack2 = Attack(EMiniGameCombatMoveAttackTypes_SQUARE1, EMiniGameCombatAttackCategoryType_ATTACK, 0.5f, "Sword Swipe", "Sword Swipe: SQUARE 1"); 
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

// EZRA
EzraCombatPreset::EzraCombatPreset() : CombatCharacterPreset()
{
    mType = EMiniGameCombatCharacterType_PLAYER;

    // ATTACKS
    // FULL CHARACTER HEAL
    Attack attack5 = Attack(EMiniGameCombatMoveAttackTypes_CHECKERBOARD2, EMiniGameCombatAttackCategoryType_ATTACK, 1.0f, "Grow Roots", "Grow Roots: CHECKERBOARD 2");
    attack5.mDamageDistanceDependent = false;
    attack5.mRequiresDirectionInput = false;
    mAttacks.push_back(attack5);

    // SUPPORT
    // ATTACK UP
    Attack attack1 = Attack(EMiniGameCombatMoveAttackTypes_WHOLE_GRID, EMiniGameCombatAttackCategoryType_SUPPORT, 0.0f, "Attack Up", "Attack Up: Increase all player attacks");
    attack1.mDamageDistanceDependent = false;
    attack1.mRequiresDirectionInput = false;
    attack1.mSpecialEffects.push_back(SpecialEffectAttackMultiplier(1.25f, INT_MAX, EAttackTargetType_ALL_PLAYERS));
    mAttacks.push_back(attack1);

    // ATTACK UP ++
    Attack attack2 = Attack(EMiniGameCombatMoveAttackTypes_WHOLE_GRID, EMiniGameCombatAttackCategoryType_SUPPORT, 0.0f, "Attack Up ++", "Attack Up++ : Increase all player attacks");
    attack2.mDamageDistanceDependent = false;
    attack2.mRequiresDirectionInput = false;
    attack2.mSpecialEffects.push_back(SpecialEffectAttackMultiplier(1.5f, INT_MAX, EAttackTargetType_ALL_PLAYERS));
    attack2.mSpecialEffects.push_back(SpecialEffectSelfStun(2));
    mAttacks.push_back(attack2);

    // DEFENSE CAPACITY UP ++
    Attack attack3 = Attack(EMiniGameCombatMoveAttackTypes_WHOLE_GRID, EMiniGameCombatAttackCategoryType_SUPPORT, 0.0f, "Defense Up", "Defense Up: Increase all player defense capacity"); 
    attack3.mDamageDistanceDependent = false;
    attack3.mRequiresDirectionInput = false;
    attack3.mSpecialEffects.push_back(SpecialEffectDefenseCapacityMultiplier(1.3f, INT_MAX, EAttackTargetType_ALL_PLAYERS));
    mAttacks.push_back(attack3);

    // HEAL
    mHealAmount = 10;

    // FULL CHARACTER HEAL
    Attack attack4 = Attack(EMiniGameCombatMoveAttackTypes_ONE_PLAYER, EMiniGameCombatAttackCategoryType_SUPPORT, 0.0f, "Full Heal", "Full Heal: Fully heal one player"); 
    attack4.mDamageDistanceDependent = false;
    attack4.mRequiresDirectionInput = false;
    attack4.mSpecialEffects.push_back(SpecialEffectFullHeal(EAttackTargetType_ONE_PLAYER));
    mAttacks.push_back(attack4);

    
    mMoveType = EMiniGameCombatMoveAttackTypes_CROSS3;

    mAttackDamage = 10;
    mHealthCapacity = 25;
    mDefenseCapacity = 25;

    mModelFileName = "MiniGameModels/Earth/MiniGameModelsEzra.bmp";
    mIconFileName = "MiniGameUI/Earth/EzraPortrait.bmp";
}


// DRAGAUR BLOSSOM
RatCombatPreset::RatCombatPreset() : CombatCharacterPreset()
{
    mType = EMiniGameCombatCharacterType_ENEMY;

    // SLASH
    Attack attack1 = Attack(EMiniGameCombatMoveAttackTypes_SQUARE2, EMiniGameCombatAttackCategoryType_ATTACK, 0.6f, "Slash", "Slash: Square 2"); 
    attack1.mDamageDistanceDependent = false;
    attack1.mRequiresDirectionInput = true;
    mAttacks.push_back(attack1);

    // SPIT THORNS
    Attack attack2 = Attack(EMiniGameCombatMoveAttackTypes_CROSS3, EMiniGameCombatAttackCategoryType_ATTACK, 1.0f, "Spit Thorns", "Spit Thorns: Cross 3");
    attack2.mDamageDistanceDependent = false;
    attack2.mRequiresDirectionInput = false;
    mAttacks.push_back(attack2);

    // DRAGAUR GROWL (Inc attack)
    Attack attack3 = Attack(EMiniGameCombatMoveAttackTypes_WHOLE_GRID, EMiniGameCombatAttackCategoryType_SUPPORT, 0.0f, "Dragaur Growl", "Dragaur Growl: Increase enmey attack by 25%");
    attack3.mDamageDistanceDependent = false;
    attack3.mRequiresDirectionInput = false;
    attack3.mSpecialEffects.push_back(SpecialEffectAttackMultiplier(1.25, 3, EAttackTargetType_ALL_ENEMIES));
    attack3.mSpecialEffects.push_back(SpecialEffectSelfStun(1));
    mAttacks.push_back(attack3);

    mMoveType = EMiniGameCombatMoveAttackTypes_CROSS2;

    mAttackDamage       = 8;
    mHealthCapacity     = 36;
    mDefenseCapacity    = 8;

    mModelFileName = "MiniGameModels/Earth/MiniGameModelsDragaurBlossom.bmp";
    mIconFileName = "MiniGameUI/Earth/DragaurBlossomPortrait.bmp";
}

// FROCODILE
FrogCombatPreset::FrogCombatPreset() : CombatCharacterPreset()
{
    mType = EMiniGameCombatCharacterType_ENEMY;

    // JUMP STRIKE
    Attack attack1 = Attack(EMiniGameCombatMoveAttackTypes_CROSS5, EMiniGameCombatAttackCategoryType_ATTACK, 0.5f, "Jump Strike", "Jump Strike: CROSS 5");
    
    attack1.mDamageDistanceDependent = true;
    attack1.mRequiresDirectionInput = false;
    mAttacks.push_back(attack1);

    // SPIT
    Attack attack2 = Attack(EMiniGameCombatMoveAttackTypes_CROSS2_2UNIT_OUT, EMiniGameCombatAttackCategoryType_ATTACK, 1.0f, "Spit", "Spit: CROSS 2, 2 OUT");
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










