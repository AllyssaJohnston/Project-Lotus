#include "combatCharacterPresets.h"

void CombatCharacterPreset::setMove(const ECombatActionGridPattern type, const int num, const int out)
{
    mMoveType = type;
    mMoveNum = num;
    mMoveOut = out;
}

// LOTUS
LotusCombatPreset::LotusCombatPreset() : CombatCharacterPreset()
{
    mType = ECombatCharacterType_PLAYER;
    mSpecies = ECombatCharacterSpecies_LOTUS;
    mName = "LOTUS";

    // OVERHEAD STRIKE
    Attack attack1 = Attack(ECombatActionGridPattern_CROSS,     ECombatCharacterType_ENEMY, 1, 0, ECombatAttackCategoryType_ATTACK, 1.0f, 1, "Overhead Strike");
    attack1.mCharacterTileSpecialEffects.push_back(SpecialEffectStun(1));
    mAttacks.push_back(attack1);

    // CUT
    Attack attack2 = Attack(ECombatActionGridPattern_CROSS,     ECombatCharacterType_ENEMY, 1, 0, ECombatAttackCategoryType_ATTACK, .67f, 0, "Cut");
    mAttacks.push_back(attack2);

    // SWORD SWIPE
    Attack attack3 = Attack(ECombatActionGridPattern_SQUARE,    ECombatCharacterType_ENEMY, 1, 0, ECombatAttackCategoryType_ATTACK, 0.5f, 0, "Sword Swipe");
    attack3.mNumTilesToAttack = ECombatNumTilesToAttack_DIRECTION;
    mAttacks.push_back(attack3);

    mHealAmount = 6;

    setMove(ECombatActionGridPattern_CROSS, 2, 0);

    mAttackDamage       = 12;
    mHealthCapacity     = 30;
    mDefenseCapacity    = 20;

    mModelFileName = "MiniGameModels/MiniGameModelsLotus.bmp";
    mIconFileName = "MiniGameUI/LotusPortrait.bmp";
}

// EZRA
EzraCombatPreset::EzraCombatPreset() : CombatCharacterPreset()
{
    mType = ECombatCharacterType_PLAYER;
    mSpecies = ECombatCharacterSpecies_EZRA;
    mName = "EZRA";

    // SUPPORT
    // ATTACK UP
    Attack attack1 = Attack(ECombatActionGridPattern_WHOLE_GRID, ECombatCharacterType_PLAYER, ECombatAttackCategoryType_SUPPORT, 0.0f, 1, "Attack Up", "Increase all player attacks");
    attack1.mNumTilesToAttack = ECombatNumTilesToAttack_ALL;
    attack1.mGenericSpecialEffects.push_back(SpecialEffectAttackMultiplier(1.25f, INT_MAX, ECombatCharacterType_PLAYER));
    mAttacks.push_back(attack1);

    // ATTACK UP ++
    Attack attack2 = Attack(ECombatActionGridPattern_WHOLE_GRID, ECombatCharacterType_PLAYER, ECombatAttackCategoryType_SUPPORT, 0.0f, 0, "Attack Up ++", "Increase all player attacks");
    attack2.mNumTilesToAttack = ECombatNumTilesToAttack_ALL;
    attack2.mGenericSpecialEffects.push_back(SpecialEffectAttackMultiplier(1.5f, INT_MAX, ECombatCharacterType_PLAYER));
    attack2.mGenericSpecialEffects.push_back(SpecialEffectSelfStun(1));
    mAttacks.push_back(attack2);

    // DEFENSE CAPACITY UP
    Attack attack3 = Attack(ECombatActionGridPattern_WHOLE_GRID, ECombatCharacterType_PLAYER, ECombatAttackCategoryType_SUPPORT, 0.0f, 0, "Defense Capacity Up", "Increase all player defense capacity");
    attack3.mNumTilesToAttack = ECombatNumTilesToAttack_ALL;
    attack3.mGenericSpecialEffects.push_back(SpecialEffectDefenseCapacityMultiplier(1.3f, INT_MAX, ECombatCharacterType_PLAYER));
    mAttacks.push_back(attack3);

    // FULL CHARACTER HEAL
    Attack attack4 = Attack(ECombatActionGridPattern_WHOLE_GRID, ECombatCharacterType_PLAYER, ECombatAttackCategoryType_SUPPORT, 0.0f, 5, "Full Heal");
    attack4.mNumTilesToAttack = ECombatNumTilesToAttack_ONE;
    attack4.mCharacterTileSpecialEffects.push_back(SpecialEffectFullHeal(ECombatCharacterType_PLAYER));
    mAttacks.push_back(attack4);

    // ATTACKS
    // GROW ROOTS
    Attack attack5 = Attack(ECombatActionGridPattern_CHECKERBOARD, ECombatCharacterType_ENEMY, 2, 0, ECombatAttackCategoryType_ATTACK, 1.0f, 0, "Grow Roots");
    attack5.mNumTilesToAttack = ECombatNumTilesToAttack_ALL;
    mAttacks.push_back(attack5);

    
    setMove(ECombatActionGridPattern_CROSS, 3, 0);

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
    mType = ECombatCharacterType_ENEMY;
    mSpecies = ECombatCharacterSpecies_RAT;
    mName = "DRAGAUR";

    // SLASH
    Attack attack1 = Attack(ECombatActionGridPattern_SQUARE, ECombatCharacterType_PLAYER, 2, 0, ECombatAttackCategoryType_ATTACK, 0.6f, 0, "Slash");
    attack1.mNumTilesToAttack = ECombatNumTilesToAttack_DIRECTION;
    mAttacks.push_back(attack1);

    // SPIT THORNS
    Attack attack2 = Attack(ECombatActionGridPattern_CROSS, ECombatCharacterType_PLAYER, 3, 1, ECombatAttackCategoryType_ATTACK, 1.0f, 0, "Spit Thorns");
    attack2.mNumTilesToAttack = ECombatNumTilesToAttack_ALL;
    mAttacks.push_back(attack2);

    // DRAGAUR GROWL
    Attack attack3 = Attack(ECombatActionGridPattern_WHOLE_GRID, ECombatCharacterType_ENEMY, ECombatAttackCategoryType_SUPPORT, 0.0f, 1, "Dragaur Growl", "Increase all enemies's attack damage");
    attack3.mNumTilesToAttack = ECombatNumTilesToAttack_ALL;
    attack3.mGenericSpecialEffects.push_back(SpecialEffectAttackMultiplier(1.25, 3, ECombatCharacterType_ENEMY));
    mAttacks.push_back(attack3);

    setMove(ECombatActionGridPattern_CROSS, 2, 0);

    mAttackDamage       = 8;
    mHealthCapacity     = 36;
    mDefenseCapacity    = 8;

    mModelFileName = "MiniGameModels/Earth/MiniGameModelsDragaurBlossom.bmp";
    mIconFileName = "MiniGameUI/Earth/DragaurBlossomPortrait.bmp";
}

// SHROOM VIPER
BouncyRatCombatPreset::BouncyRatCombatPreset() : CombatCharacterPreset()
{
    mType = ECombatCharacterType_ENEMY;
    mSpecies = ECombatCharacterSpecies_BOUNCY_RAT;
    mName = "SHROOM VIPER";

    // CHOMP
    Attack attack1 = Attack(ECombatActionGridPattern_CROSS, ECombatCharacterType_PLAYER, 2, 0, ECombatAttackCategoryType_ATTACK, 1.0f, 0, "Chomp");
    attack1.mNumTilesToAttack = ECombatNumTilesToAttack_ALL;
    mAttacks.push_back(attack1);

    // LICK
    Attack attack2 = Attack(ECombatActionGridPattern_SQUARE, ECombatCharacterType_PLAYER, 1, 0, ECombatAttackCategoryType_ATTACK, 0.5f, 0, "Lick");
    attack2.mNumTilesToAttack = ECombatNumTilesToAttack_DIRECTION;
    mAttacks.push_back(attack2);

    setMove(ECombatActionGridPattern_CROSS, 3, 0);

    mAttackDamage = 8;
    mHealthCapacity = 48;
    mDefenseCapacity = 4;

    mModelFileName = "MiniGameModels/Earth/MiniGameModelsShroomViper.bmp";
    mIconFileName = "MiniGameUI/Earth/ShroomViperPortrait.bmp";
}

// FROCODILE
FrogCombatPreset::FrogCombatPreset() : CombatCharacterPreset()
{
    mType = ECombatCharacterType_ENEMY;
    mSpecies = ECombatCharacterSpecies_FROG;
    mName = "FROCODILE";

    // JUMP STRIKE
    Attack attack1 = Attack(ECombatActionGridPattern_CROSS, ECombatCharacterType_PLAYER, 5, 0, ECombatAttackCategoryType_ATTACK, 0.6f, 1, "Jump Strike");
    attack1.mDamageDistanceDependent = true;
    attack1.mDamageDropOff           = .25f;
    attack1.mNumTilesToAttack        = ECombatNumTilesToAttack_ALL;
    mAttacks.push_back(attack1);

    // SPIT
    Attack attack2 = Attack(ECombatActionGridPattern_SQUARE, ECombatCharacterType_PLAYER, 1, 1, ECombatAttackCategoryType_ATTACK, 1.0f, 0, "Spit");
    attack2.mNumTilesToAttack = ECombatNumTilesToAttack_ALL;
    mAttacks.push_back(attack2);

    setMove(ECombatActionGridPattern_CROSS, 3, 1);

    mAttackDamage = 5;
    mHealthCapacity = 40;
    mDefenseCapacity = 4;

    mModelFileName = "MiniGameModels/Earth/MiniGameModelsFrocodile.bmp";
    mIconFileName = "MiniGameUI/Earth/FrocodilePortrait.bmp";
}

// FROG SHROOM
BouncyFrogCombatPreset::BouncyFrogCombatPreset() : CombatCharacterPreset()
{
    mType = ECombatCharacterType_ENEMY;
    mSpecies = ECombatCharacterSpecies_BOUNCY_FROG;
    mName = "FROG SHROOM";

    // TODO JUMP (teleport)
    // 5 turn cooldown

    // CRUSH
    Attack attack1 = Attack(ECombatActionGridPattern_CROSS, ECombatCharacterType_PLAYER, 2, 1, ECombatAttackCategoryType_ATTACK, 0.67f, 0, "Crush");
    attack1.mNumTilesToAttack = ECombatNumTilesToAttack_ALL;
    mAttacks.push_back(attack1);

    // CRUSH ++
    Attack attack2 = Attack(ECombatActionGridPattern_CROSS, ECombatCharacterType_PLAYER, 3, 0, ECombatAttackCategoryType_ATTACK, 1.0f, 0, "Crush++");
    attack2.mNumTilesToAttack = ECombatNumTilesToAttack_ALL;
    attack2.mCooldownAmount             = 3;
    attack2.mCharacterTileSpecialEffects.push_back(SpecialEffect(SpecialEffectStun(1)));
    mAttacks.push_back(attack2);

    setMove(ECombatActionGridPattern_CROSS, 2, 1);

    mAttackDamage = 12;
    mHealthCapacity = 24;
    mDefenseCapacity = 8;

    mModelFileName = "MiniGameModels/Earth/MiniGameModelsFrogShroom.bmp";
    mIconFileName = "MiniGameUI/Earth/FrogShroomPortrait.bmp";
}

// SPIDERING
HidingRatCombatPreset::HidingRatCombatPreset() : CombatCharacterPreset()
{
    mType = ECombatCharacterType_ENEMY;
    mSpecies = ECombatCharacterSpecies_HIDING_RAT;
    mName = "SPIDERIG";

    // WEB
    Attack attack1 = Attack(ECombatActionGridPattern_CROSS, ECombatCharacterType_PLAYER,  2, 0, ECombatAttackCategoryType_SUPPORT, 0.0f, 0, "Web");
    attack1.mNumTilesToAttack = ECombatNumTilesToAttack_ONE;
    attack1.mCharacterTileSpecialEffects.push_back(SpecialEffectStun(1));
    mAttacks.push_back(attack1);

    // POISON
    Attack attack2 = Attack(ECombatActionGridPattern_CROSS, ECombatCharacterType_PLAYER, 1, 0, ECombatAttackCategoryType_ATTACK, 1.0f, 2, "Poison");
    attack2.mNumTilesToAttack = ECombatNumTilesToAttack_ONE;
    attack2.mCharacterTileSpecialEffects.push_back(SpecialEffect(SpecialEffectPoison(2, 2)));
    mAttacks.push_back(attack2);

    setMove(ECombatActionGridPattern_SQUARE, 1, 0);

    mAttackDamage = 6;
    mHealthCapacity = 20;
    mDefenseCapacity = 12;

    mModelFileName = "MiniGameModels/Earth/MiniGameModelsSpiderig.bmp";
    mIconFileName = "MiniGameUI/Earth/SpiderigPortrait.bmp";
}

// SEEDLING
FastRatCombatPreset::FastRatCombatPreset() : CombatCharacterPreset()
{
    mType = ECombatCharacterType_ENEMY;
    mSpecies = ECombatCharacterSpecies_FAST_RAT;
    mName = "SEEDLING";

    // Poke
    Attack attack1 = Attack(ECombatActionGridPattern_WHOLE_GRID, ECombatCharacterType_PLAYER, ECombatAttackCategoryType_ATTACK, 1.0f, 0, "Poke");
    attack1.mNumTilesToAttack = ECombatNumTilesToAttack_ONE;
    mAttacks.push_back(attack1);

    // Heal
    Attack attack2 = Attack(ECombatActionGridPattern_WHOLE_GRID, ECombatCharacterType_ENEMY, ECombatAttackCategoryType_SUPPORT, 0.0f, 5, "Heal");
    attack2.mNumTilesToAttack = ECombatNumTilesToAttack_ONE;
    attack2.mCharacterTileSpecialEffects.push_back(SpecialEffect(SpecialEffectHeal(15, ECombatCharacterType_ENEMY)));
    mAttacks.push_back(attack2);

    // REVIVE (limit once)
    Attack attack3 = Attack(ECombatActionGridPattern_WHOLE_GRID, ECombatCharacterType_ENEMY, ECombatAttackCategoryType_SUPPORT, 0.0f, 5, "Revive");
    attack3.mNumTilesToAttack   = ECombatNumTilesToAttack_ONE;
    attack3.mAttackTargetAlive  = ECombatAttackTargetAlive_DEAD;
    attack3.mLimit                      = 1;
    attack3.mCharacterTileSpecialEffects.push_back(SpecialEffectRevive(ECombatCharacterType_ENEMY));
    mAttacks.push_back(attack3);

    setMove(ECombatActionGridPattern_SQUARE, 2, 0);

    mAttackDamage = 2;
    mHealthCapacity = 24;
    mDefenseCapacity = 6;

    mModelFileName = "MiniGameModels/Earth/MiniGameModelsSeedling.bmp";
    mIconFileName = "MiniGameUI/Earth/SeedlingPortrait.bmp";
}

