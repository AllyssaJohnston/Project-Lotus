#include "combatCharacterPresets.h"

void CombatCharacterPreset::setMove(EMiniGameCombatMoveAttackTypes type, int num, int out)
{
    mMoveType = type;
    mMoveNum = num;
    mMoveOut = out;
}

// LOTUS
LotusCombatPreset::LotusCombatPreset() : CombatCharacterPreset()
{
    mType = EMiniGameCombatCharacterType_PLAYER;

    // OVERHEAD STRIKE
    Attack attack1 = Attack(EMiniGameCombatMoveAttackTypes_CROSS, 1, 0, EMiniGameCombatAttackCategoryType_ATTACK, 1.0f, "Overhead Strike"); 
    attack1.mDamageDistanceDependent = false;
    attack1.mRequiresDirectionInput = false;
    attack1.mSpecialEffects.push_back(SpecialEffectStun(1));
    attack1.mSpecialEffects.push_back(SpecialEffectSelfStun(1));
    mAttacks.push_back(attack1);

    // SWORD SWIPE
    Attack attack2 = Attack(EMiniGameCombatMoveAttackTypes_SQUARE, 1, 0, EMiniGameCombatAttackCategoryType_ATTACK, 0.5f, "Sword Swipe"); 
    attack2.mDamageDistanceDependent = false;
    attack2.mRequiresDirectionInput = true;
    mAttacks.push_back(attack2);

    mHealAmount = 6;

    setMove(EMiniGameCombatMoveAttackTypes_CROSS, 2, 0);

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

    // SUPPORT
    // ATTACK UP
    Attack attack1 = Attack(EMiniGameCombatMoveAttackTypes_WHOLE_GRID, EMiniGameCombatAttackCategoryType_SUPPORT, 0.0f, "Attack Up", "Increase all player attacks");
    attack1.mDamageDistanceDependent = false;
    attack1.mRequiresDirectionInput = false;
    attack1.mSpecialEffects.push_back(SpecialEffectAttackMultiplier(1.25f, INT_MAX, EAttackTargetType_ALL_PLAYERS));
    mAttacks.push_back(attack1);

    // ATTACK UP ++
    Attack attack2 = Attack(EMiniGameCombatMoveAttackTypes_WHOLE_GRID, EMiniGameCombatAttackCategoryType_SUPPORT, 0.0f, "Attack Up ++", "Increase all player attacks");
    attack2.mDamageDistanceDependent = false;
    attack2.mRequiresDirectionInput = false;
    attack2.mSpecialEffects.push_back(SpecialEffectAttackMultiplier(1.5f, INT_MAX, EAttackTargetType_ALL_PLAYERS));
    attack2.mSpecialEffects.push_back(SpecialEffectSelfStun(2));
    mAttacks.push_back(attack2);

    // DEFENSE CAPACITY UP
    Attack attack3 = Attack(EMiniGameCombatMoveAttackTypes_WHOLE_GRID, EMiniGameCombatAttackCategoryType_SUPPORT, 0.0f, "Defense Capacity Up", "Increase all player defense capacity"); 
    attack3.mDamageDistanceDependent = false;
    attack3.mRequiresDirectionInput = false;
    attack3.mSpecialEffects.push_back(SpecialEffectDefenseCapacityMultiplier(1.3f, INT_MAX, EAttackTargetType_ALL_PLAYERS));
    mAttacks.push_back(attack3);

    // FULL CHARACTER HEAL
    Attack attack4 = Attack(EMiniGameCombatMoveAttackTypes_WHOLE_GRID, EMiniGameCombatAttackCategoryType_SUPPORT, 0.0f, "Full Heal");
    attack4.mDamageDistanceDependent = false;
    attack4.mRequiresDirectionInput = false;
    attack4.mSpecialEffects.push_back(SpecialEffectFullHeal(EAttackTargetType_ONE_PLAYER));
    mAttacks.push_back(attack4);

    // ATTACKS
    // FULL CHARACTER HEAL
    Attack attack5 = Attack(EMiniGameCombatMoveAttackTypes_CHECKERBOARD, 2, 0, EMiniGameCombatAttackCategoryType_ATTACK, 1.0f, "Grow Roots");
    attack5.mDamageDistanceDependent = false;
    attack5.mRequiresDirectionInput = false;
    mAttacks.push_back(attack5);

    
    setMove(EMiniGameCombatMoveAttackTypes_CROSS, 3, 0);

    mAttackDamage = 10;
    mHealthCapacity = 25;
    mDefenseCapacity = 25;
    mHealAmount = 10;

    mModelFileName = "MiniGameModels/Earth/MiniGameModelsEzra.bmp";
    mIconFileName = "MiniGameUI/Earth/EzraPortrait.bmp";
}


// DRAGAUR BLOSSOM
RatCombatPreset::RatCombatPreset() : CombatCharacterPreset()
{
    mType = EMiniGameCombatCharacterType_ENEMY;

    // SLASH
    Attack attack1 = Attack(EMiniGameCombatMoveAttackTypes_SQUARE, 2, 0, EMiniGameCombatAttackCategoryType_ATTACK, 0.6f, "Slash"); 
    attack1.mDamageDistanceDependent = false;
    attack1.mRequiresDirectionInput = true;
    mAttacks.push_back(attack1);

    // SPIT THORNS
    Attack attack2 = Attack(EMiniGameCombatMoveAttackTypes_CROSS, 3, 1, EMiniGameCombatAttackCategoryType_ATTACK, 1.0f, "Spit Thorns");
    attack2.mDamageDistanceDependent = false;
    attack2.mRequiresDirectionInput = false;
    mAttacks.push_back(attack2);

    // DRAGAUR GROWL
    Attack attack3 = Attack(EMiniGameCombatMoveAttackTypes_WHOLE_GRID, EMiniGameCombatAttackCategoryType_SUPPORT, 0.0f, "Dragaur Growl", "Increase all enemies's attack damage");
    attack3.mDamageDistanceDependent = false;
    attack3.mRequiresDirectionInput = false;
    attack3.mSpecialEffects.push_back(SpecialEffectAttackMultiplier(1.25, 3, EAttackTargetType_ALL_ENEMIES));
    attack3.mSpecialEffects.push_back(SpecialEffectSelfStun(1));
    mAttacks.push_back(attack3);

    setMove(EMiniGameCombatMoveAttackTypes_CROSS, 2, 0);

    mAttackDamage       = 8;
    mHealthCapacity     = 36;
    mDefenseCapacity    = 8;

    mModelFileName = "MiniGameModels/Earth/MiniGameModelsDragaurBlossom.bmp";
    mIconFileName = "MiniGameUI/Earth/DragaurBlossomPortrait.bmp";
}

// SHROOM VIPER
BouncyRatCombatPreset::BouncyRatCombatPreset() : CombatCharacterPreset()
{
    mType = EMiniGameCombatCharacterType_ENEMY;

    // CHOMP
    Attack attack1 = Attack(EMiniGameCombatMoveAttackTypes_CROSS, 2, 0, EMiniGameCombatAttackCategoryType_ATTACK, 1.0f, "Chomp");
    attack1.mDamageDistanceDependent = false;
    attack1.mRequiresDirectionInput = false;
    mAttacks.push_back(attack1);

    // LICK
    Attack attack2 = Attack(EMiniGameCombatMoveAttackTypes_SQUARE, 1, 0, EMiniGameCombatAttackCategoryType_ATTACK, 0.5f, "Lick");
    attack2.mDamageDistanceDependent = false;
    attack2.mRequiresDirectionInput = true;
    mAttacks.push_back(attack2);

    setMove(EMiniGameCombatMoveAttackTypes_CROSS, 3, 0);

    mAttackDamage = 8;
    mHealthCapacity = 48;
    mDefenseCapacity = 4;

    mModelFileName = "MiniGameModels/Earth/MiniGameModelsShroomViper.bmp";
    mIconFileName = "MiniGameUI/Earth/ShroomViperPortrait.bmp";
}

// FROCODILE
FrogCombatPreset::FrogCombatPreset() : CombatCharacterPreset()
{
    mType = EMiniGameCombatCharacterType_ENEMY;

    // JUMP STRIKE
    Attack attack1 = Attack(EMiniGameCombatMoveAttackTypes_CROSS, 5, 0, EMiniGameCombatAttackCategoryType_ATTACK, 0.5f, "Jump Strike");
    attack1.mDamageDistanceDependent = true;
    attack1.mRequiresDirectionInput = false;
    mAttacks.push_back(attack1);

    // SPIT
    Attack attack2 = Attack(EMiniGameCombatMoveAttackTypes_CROSS, 2, 2, EMiniGameCombatAttackCategoryType_ATTACK, 1.0f, "Spit");
    attack2.mDamageDistanceDependent = false;
    attack2.mRequiresDirectionInput = false;
    attack2.mSpecialEffects.push_back(SpecialEffect(SpecialEffectStun(1)));
    mAttacks.push_back(attack2);

    setMove(EMiniGameCombatMoveAttackTypes_CROSS, 3, 0);

    mAttackDamage = 8;
    mHealthCapacity = 48;
    mDefenseCapacity = 4;

    mModelFileName = "MiniGameModels/Earth/MiniGameModelsFrocodile.bmp";
    mIconFileName = "MiniGameUI/Earth/FrocodilePortrait.bmp";
}

// FROG SHROOM
BouncyFrogCombatPreset::BouncyFrogCombatPreset() : CombatCharacterPreset()
{
    mType = EMiniGameCombatCharacterType_ENEMY;

    // TODO JUMP (teleport)
    // 3 turn cooldown

    // CRUSH
    Attack attack1 = Attack(EMiniGameCombatMoveAttackTypes_CROSS, 2, 1, EMiniGameCombatAttackCategoryType_ATTACK, 0.67f, "Crush");
    attack1.mDamageDistanceDependent = false;
    attack1.mRequiresDirectionInput = false;
    mAttacks.push_back(attack1);

    // CRUSH ++
    Attack attack2 = Attack(EMiniGameCombatMoveAttackTypes_CROSS, 3, 0, EMiniGameCombatAttackCategoryType_ATTACK, 1.0f, "Crush++");
    attack2.mDamageDistanceDependent = false;
    attack2.mRequiresDirectionInput = false;
    attack2.mSpecialEffects.push_back(SpecialEffect(SpecialEffectStun(1)));
    mAttacks.push_back(attack2);

    setMove(EMiniGameCombatMoveAttackTypes_CROSS, 2, 1);

    mAttackDamage = 12;
    mHealthCapacity = 24;
    mDefenseCapacity = 8;

    mModelFileName = "MiniGameModels/Earth/MiniGameModelsFrogShroom.bmp";
    mIconFileName = "MiniGameUI/Earth/FrogShroomPortrait.bmp";
}

// SPIDERING
HidingRatCombatPreset::HidingRatCombatPreset() : CombatCharacterPreset()
{
    mType = EMiniGameCombatCharacterType_ENEMY;

    // WEB
    Attack attack1 = Attack(EMiniGameCombatMoveAttackTypes_CROSS, 2, 0, EMiniGameCombatAttackCategoryType_SUPPORT, 0.0f, "Web");
    attack1.mDamageDistanceDependent = false;
    attack1.mRequiresDirectionInput = false;
    attack1.mSpecialEffects.push_back(SpecialEffectStun(1));
    mAttacks.push_back(attack1);

    // POISON
    // TODO status effect take damage for 2 rounds
    Attack attack2 = Attack(EMiniGameCombatMoveAttackTypes_CROSS, 1, 0, EMiniGameCombatAttackCategoryType_ATTACK, 1.0f, "Poison");
    attack2.mDamageDistanceDependent = false;
    attack2.mRequiresDirectionInput = false;
    attack2.mSpecialEffects.push_back(SpecialEffect(SpecialEffectStun(1)));
    mAttacks.push_back(attack2);

    setMove(EMiniGameCombatMoveAttackTypes_SQUARE, 1, 0);

    mAttackDamage = 6;
    mHealthCapacity = 20;
    mDefenseCapacity = 12;

    mModelFileName = "MiniGameModels/Earth/MiniGameModelsSpiderig.bmp";
    mIconFileName = "MiniGameUI/Earth/SpiderigPortrait.bmp";
}

// SEEDLING
FastRatCombatPreset::FastRatCombatPreset() : CombatCharacterPreset()
{
    mType = EMiniGameCombatCharacterType_ENEMY;

    // Poke
    Attack attack1 = Attack(EMiniGameCombatMoveAttackTypes_ONE_PLAYER, EMiniGameCombatAttackCategoryType_ATTACK, 1.0f, "Poke");
    attack1.mDamageDistanceDependent = false;
    attack1.mRequiresDirectionInput = false;
    mAttacks.push_back(attack1);

    // Heal
    Attack attack2 = Attack(EMiniGameCombatMoveAttackTypes_ONE_ENEMY, EMiniGameCombatAttackCategoryType_SUPPORT, 0.0f, "Heal");
    attack2.mDamageDistanceDependent = false;
    attack2.mRequiresDirectionInput = false;
    attack2.mSpecialEffects.push_back(SpecialEffect(SpecialEffectHeal(15, EAttackTargetType_ONE_ENEMY)));
    attack2.mSpecialEffects.push_back(SpecialEffect(SpecialEffectStun(3)));
    mAttacks.push_back(attack2);

    // Revive
    // TODO Revive one enemy
    // TODO limit 1 per battle
    /*Attack attack3 = Attack(EMiniGameCombatMoveAttackTypes_ONE_ENEMY, EMiniGameCombatAttackCategoryType_SUPPORT, 0.0f, "Revive", "Revive: Revive one enemy");
    attack3.mDamageDistanceDependent = false;
    attack3.mRequiresDirectionInput = false;
    mAttacks.push_back(attack3);*/

    setMove(EMiniGameCombatMoveAttackTypes_SQUARE, 2, 0);

    mAttackDamage = 6;
    mHealthCapacity = 30;
    mDefenseCapacity = 6;

    mModelFileName = "MiniGameModels/Earth/MiniGameModelsSeedling.bmp";
    mIconFileName = "MiniGameUI/Earth/SeedlingPortrait.bmp";
}
















