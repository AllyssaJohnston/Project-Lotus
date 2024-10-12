#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include "combatCharacterHelper.h"
#include "combatAttackHelper.h"
#include "tileHelperClass.h"

class CombatManager
{
public:
    std::vector <CombatCharacter*> mpCurCombatCharacters;
    std::vector <CombatCharacter*> mpAllCombatCharacters;
    //std::vector <CombatEntity*>    mpObjects;

    CombatManager(){}

    ~CombatManager();

    void preTick();
    void postTick();

    void createCurCharacterList();

    std::vector <CombatCharacter*> getCurCharactersThatCanPlay() const;

    CombatCharacter* returnNextCharacter(CombatCharacter* pCurCharacter);

    int returnCurCharacterIndex(CombatCharacter* pGivenCharacter) const;

    void updateAllCharactersPostTurn();

    void updateCharacterPostTurn(CombatCharacter* pOldCharacter, CombatCharacter* pNextCharacter);

    //return attackedCharacterChanges
    void attack(CombatCharacter* pAttackingCharacter, Tile* pGivenTile, const Attack & attack);

    //NOTHING IN HERE YET
    /*void specialEffect(GameInstance& gameInstance, CombatCharacter* pAttackingCharacter, CombatCharacter* pAttackedCharacter, std::vector<SpecialEffect>& pSpecialEffects)
    {

    }*/

    bool checkIfWonGame();

    void resetStats();

    /*

    def specialEffect(self, gameInstance, attackingCharacter, attackedCharacter, specialEffects, curAttackingCharacterChanges, curAttackedCharacterChanges):
    for count in range(0, len(specialEffects)):
    curSpecialEffect = specialEffects[count]
    grid = gameInstance.miniGameLevels[gameInstance.miniGameLevelNumber].grid
    if   curSpecialEffect.type == EMiniGameCombatSpecialEffectTypes.NONE.value:
    pass
    elif curSpecialEffect.type == EMiniGameCombatSpecialEffectTypes.KNOCKBACK.value:
    directionBetweenCharacters = getDirectionBetweenTiles(attackingCharacter.movementManager.curTile, attackedCharacter.movementManager.curTile)
    rowMove = 0
    colMove = 0
    if   directionBetweenCharacters.rowDirection == EDirection.NEG.value:
    #CAN CHANGE VALUE OF ROW MOVE
    rowMove = -1
    elif directionBetweenCharacters.rowDirection == EDirection.POS.value:
    rowMove = 1
    if   directionBetweenCharacters.colDirection == EDirection.NEG.value:
    colMove = -1
    elif directionBetweenCharacters.colDirection == EDirection.POS.value:
    colMove = 1
    formerTile = attackedCharacter.movementManager.curTile
    if grid.legalCoords(formerTile.row + rowMove, formerTile.col + colMove):
    tileToMoveTo = grid.tiles[formerTile.row + rowMove][formerTile.col + colMove]
    moveChange = calculateMoveChange(attackedCharacter.movementManager.curTile, tileToMoveTo)
    attackedCharacter.move(evaluateTileTypeAndTakeAction(gameInstance, tileToMoveTo, attackedCharacter))
    actionTaken = ActionTakenMovePreset(attackedCharacter, moveChange)
    gameInstance.miniGameStateManager.addActionTaken(ActionTaken(actionTaken))
    elif curSpecialEffect.type == EMiniGameCombatSpecialEffectTypes.STUN.value:
    curAttackedCharacterChanges.turnsToPassChange = attackedCharacter.stun()
    elif curSpecialEffect.type == EMiniGameCombatSpecialEffectTypes.LOSE_TURN.value:
    curAttackingCharacterChanges.turnsToPassChange = attackingCharacter.stun()


    */
};
