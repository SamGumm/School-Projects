#pragma once

#include <random>
#include <string>
#include "dungeon_creation/nagazod_room.hpp"

struct MONSTER_STATS {
    int HP{0}, DEX{0};
    int DAMAGE_DICE{0}, HIT_DICE{0}, DODGE{0};
    std::string desc{};   // name/description
};

class Monster {
public:
    virtual ~Monster() = default;

    /// Make one attack roll (total damage)
    virtual int attack() = 0;

    /// Try to move in direction d (if there's a door)
    virtual void move(DIRECTION d) = 0;

    /// Called when monster dies, to spawn loot
    virtual void drop_loot() = 0;

    void take_damage(int dmg) { stat_block_.HP = std::max(0, stat_block_.HP - dmg); }

    /// Read‐only accessors
    int    hp()     const { return stat_block_.HP; }
    int    dex()    const { return stat_block_.DEX; }
    const std::string& description() const { return stat_block_.desc; }
    Room*  room()   const { return currentRoom_; }

protected:
    /// Only derived types can construct
    Monster(const MONSTER_STATS& stats, Room* startRoom)
      : stat_block_(stats),
        rng_(std::random_device{}()),
        currentRoom_(startRoom)
    {}

    MONSTER_STATS     stat_block_;
    std::mt19937      rng_;
    Room*             currentRoom_;
};

/// A weak but numerous enemy
class Slime : public Monster {
public:
    explicit Slime(Room* startRoom);

    int  attack()     override;
    void move(DIRECTION d) override;
    void drop_loot()  override;
};

/// A standard melee enemy
class Skeleton : public Monster {
public:
    explicit Skeleton(Room* startRoom);

    int  attack()     override;
    void move(DIRECTION d) override;
    void drop_loot()  override;
};

/// A powerful boss
class Lich_King : public Monster {
public:
    explicit Lich_King(Room* startRoom);

    int  attack()     override;
    void move(DIRECTION d) override;
    void drop_loot()  override;
};
