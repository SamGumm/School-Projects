#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include <string>
#include <vector>
#include "generateDungeon.hpp"
#include "dice.hpp"

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
};

#endif
