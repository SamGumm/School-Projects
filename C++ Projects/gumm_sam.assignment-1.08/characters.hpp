#ifndef CHARACTERS_HPP
#define CHARACTERS_HPP

#include <string>
#include <vector>
#include <algorithm>
#include "generateDungeon.hpp"  // position_t, map size
#include "dice.hpp"
#include "object_factory.hpp"
#include "object_description.hpp"
#include "objects.hpp"

// ───── Base abstract Character ───────────────────────────────
class Character {
public:
    std::string              NAME, DESC;
    std::vector<int>         COLORS;   // curses COLOR_* values
    int                      SPEED = 0;
    std::vector<std::string> ABIL;     // ability tokens
    int                      HP   = 0;
    std::string              DAM;      // damage dice (keep string)
    char                     SYMB = '?';
    int                      RRTY = 0; // rarity (monsters only)
    position_t               position{0,0};
    bool                     alive = true;

    bool has_ability(const std::string& ab) const {
        return std::find(ABIL.begin(), ABIL.end(), ab) != ABIL.end();
    }

    virtual int  attack()                = 0;
    virtual void move(position_t newPos) = 0;
    virtual ~Character() = default;
};

// ───── Monster ───────────────────────────────────────────────
class Monster : public Character {
public:
    bool is_unique = false;

    int  attack() override;                   // characters.cpp
    void move(position_t newPos) override;    //  "
    void display() const;                     //  "
};

// ───── Player ────────────────────────────────────────────────
class Player : public Character {
public:
    bool fullVisibility = false;
    bool teleportMode   = false;
    position_t target{0,0};
    char memory[DUNGEON_HEIGHT][DUNGEON_WIDTH]{};
    std::vector<Object> inventory;

    Player();                                 // default
    Player(position_t start, const std::string& name);

    int  attack() override { return 0; }      // PC attack handled elsewhere
    void move(position_t newPos) override { position = newPos; }
};

#endif  // CHARACTERS_HPP
