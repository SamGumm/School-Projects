#ifndef MONSTER_FACTORY_HPP
#define MONSTER_FACTORY_HPP

#include "monster_description.hpp"
#include "characters.hpp"
#include "dice.hpp"
#include <sstream>

inline Monster make_monster(const MonsterDescription& d)
{
    Monster m;
    Dice roller;

    m.NAME   = d.name;
    m.SYMB   = d.symbol;
    m.COLORS = d.colors;

    m.SPEED  = roller.rollDice(d.speedDice);
    m.HP     = roller.rollDice(d.hpDice);
    m.DAM    = d.damageDice;

    m.ABIL   = d.abilities;
    m.RRTY   = d.rarity;
    m.is_unique = d.is_unique;
    m.alive  = true;
    return m;
}

#endif
