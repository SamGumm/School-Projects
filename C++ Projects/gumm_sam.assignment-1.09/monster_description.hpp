#ifndef MONSTER_DESCRIPTION_HPP
#define MONSTER_DESCRIPTION_HPP

#include <string>
#include <vector>

struct MonsterDescription {
    std::string name;
    std::string desc;
    char        symbol;
    std::vector<int>  colors;        // curses COLOR_* ints
    std::string speedDice;           // keep dice as text
    std::string hpDice;
    std::string damageDice;
    std::vector<std::string> abilities;
    int         rarity     = 0;      // 0‑99
    bool        is_unique  = false;
};

#endif
