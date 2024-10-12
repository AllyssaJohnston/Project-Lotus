#pragma once
#include <string>
#include "combatAttackHelper.h"

std::string getAttackName(Attack attack);

std::string getAttackType(Attack attack);

std::string getAttackDamage(Attack attack, int characterDamage);

std::string getSpecialEffect(Attack attack);