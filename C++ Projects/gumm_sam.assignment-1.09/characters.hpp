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
#include <array>

enum class EquipSlot {
    WEAPON, OFFHAND, RANGED,
    ARMOR, HELMET, CLOAK, GLOVES,
    BOOTS, AMULET, LIGHT,
    RING_A, RING_B,
    SLOTS
};
  

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
    std::array<Object*,10> carry_;          // slots 0–9
    std::array<Object*,(size_t)EquipSlot::SLOTS> equip_;  // WEAPON…RING_B


    Player();                                 // default
    Player(position_t start, const std::string& name);

    int  attack() override;   // PC attack handled elsewhere
    void move(position_t newPos) override { position = newPos; }

    bool has_free_carry_slot() const;
    int  first_free_carry_index() const;
    bool add_to_inventory(Object* o, Dungeon& d);
    void drop_from_inventory(int idx, Dungeon& d);
    bool equip_item(int carry_idx, EquipSlot slot);
    bool unequip_item(EquipSlot slot, int carry_idx);
    void expunge_from_inventory(int idx);
    void list_inventory() const;
    void list_equipment() const;
    void inspect_inventory(int idx) const;
};

#endif  // CHARACTERS_HPP
