#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include <string>
#include <vector>
#include "generateDungeon.hpp"
#include "dice.hpp"


enum class ObjectType {
    WEAPON, OFFHAND, RANGED,
    ARMOR, HELMET, CLOAK,
    GLOVES, BOOTS, AMULET,
    LIGHT, RING, UNKNOWN
};

class Object {
public:
    std::string name;
    char        symbol;
    int         color;          // curses COLOR_*
    int         weight;         // rolled on instantiation
    int         speed;
    int         hit;
    int         damage;
    int         dodge;
    int         defense;
    int         attribute;
    int         value;
    bool        is_artifact = false;

    position_t  pos;

    /* drawn glyph */
    char glyph() const { return symbol; }
    ObjectType slot = ObjectType::UNKNOWN;
    ObjectType type() const { return slot; }
    std::string equip_slot_name() const;
};

#endif
