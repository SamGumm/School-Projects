#ifndef OBJECT_FACTORY_HPP
#define OBJECT_FACTORY_HPP

#include "object_description.hpp"
#include "objects.hpp"
#include "dice.hpp"
#include <ncurses.h>

inline Object make_object(const ObjectDescription& d)
{
    Dice roll;
    Object o;

    // Basic identity
    o.name   = d.name;
    o.symbol = d.symbol;
    // Use the first specified color, or white if none
    o.color  = d.colors.empty() ? COLOR_WHITE : d.colors.front();

    // Roll each of the numeric properties
    o.weight    = roll.rollDice(d.weightDice);
    o.speed     = roll.rollDice(d.speedDice);
    o.hit       = roll.rollDice(d.hitDice);
    o.damage    = roll.rollDice(d.damageDice);
    o.dodge     = roll.rollDice(d.dodgeDice);
    o.defense   = roll.rollDice(d.defenseDice);
    o.attribute = roll.rollDice(d.attributeDice);
    o.value     = roll.rollDice(d.valueDice);

    // Artifacts only appear once
    o.is_artifact = d.is_artifact;

    // Position is set later by the dungeon when placing objects:
    //    get_random_open_cell(o.pos.x, o.pos.y);

    return o;
}

#endif  // OBJECT_FACTORY_HPP
