#ifndef OBJECT_DESCRIPTION_HPP
#define OBJECT_DESCRIPTION_HPP
#include <string>
#include <vector>

struct ObjectDescription {
    std::string name;
    std::string desc;
    char        symbol;
    std::vector<int> colors;
    std::string weightDice, speedDice, hitDice, damageDice, dodgeDice,
                defenseDice, attributeDice, valueDice;   // keep as text
    int  rarity = 0;
    bool is_artifact = false;
};

#endif
