// src/characters/nagazod_monster.cpp
#include "characters/nagazod_monster.hpp"
#include "loot/nagazod_object_factory.hpp" 
#include <iostream>
#include <algorithm>

// ———————————————————————————————————————————————
//  Slime
// ———————————————————————————————————————————————

Slime::Slime(Room* start)
  : Monster(
        MONSTER_STATS {
            /*HP*/            10,
            /*DEX*/            4,
            /*DAMAGE_DICE*/    4,  // 1d4 damage
            /*HIT_DICE*/       1,
            /*DODGE*/          2,
            /*desc*/          "green slime"
        },
      start
    )
{
    // optionally mark the room as occupied
}

int Slime::attack() {
    // roll stat_block_.HIT_DICE d stat_block_.DAMAGE_DICE
    std::uniform_int_distribution<> die(1, stat_block_.DAMAGE_DICE);
    int total = 0;
    for (int i = 0; i < stat_block_.HIT_DICE; ++i)
        total += die(rng_);
    std::cout << "The slime oozes forward and hits for " << total << " damage!\n";
    return total;
}

void Slime::move(DIRECTION d) {
    Room* next = currentRoom_->neighbor(d);
    if (next && next->has_been_visited()) {
        currentRoom_ = next;
        std::cout << "The slime slithers " << static_cast<int>(d) << " into room #" 
                  << currentRoom_->id() << "\n";
    }
}

void Slime::drop_loot() {
    // slimes rarely drop anything
    if (std::uniform_int_distribution<>(1, 4)(rng_) == 1) {
        std::cout << "The slime leaves behind a slimy goo. You collect it as loot.\n";
        // e.g. dungeon.spawn_item(currentRoom_, Object_ID::SLIME_GOO);
    } else {
        std::cout << "The slime dissolves into nothing.\n";
    }
}

// ———————————————————————————————————————————————
//  Skeleton
// ———————————————————————————————————————————————

Skeleton::Skeleton(Room* start)
  : Monster(
      MONSTER_STATS{
        /*HP*/            20,
        /*DEX*/            6,
        /*DAMAGE_DICE*/    6,  // 1d6 damage
        /*HIT_DICE*/       1,
        /*DODGE*/          4,
        /*desc*/          "rattling skeleton"
      },
      start
    )
{}

int Skeleton::attack() {
    std::uniform_int_distribution<> die(1, stat_block_.DAMAGE_DICE);
    int dmg = die(rng_);
    std::cout << "The skeleton swings its sword for " << dmg << " damage!\n";
    return dmg;
}

void Skeleton::move(DIRECTION d) {
    // skeleton only chases visited rooms (so it follows your trail)
    Room* next = currentRoom_->neighbor(d);
    if (next && next->has_been_visited()) {
        currentRoom_ = next;
        std::cout << "The skeleton rattles " << static_cast<int>(d)
                  << " into room #" << currentRoom_->id() << "\n";
    }
}

void Skeleton::drop_loot() {
    std::cout << "The skeleton drops a cracked bone you can keep.\n";
    // e.g. dungeon.spawn_item(currentRoom_, Object_ID::CRACKED_BONE);
}

// ———————————————————————————————————————————————
//  Lich King (boss)
// ———————————————————————————————————————————————

Lich_King::Lich_King(Room* start)
  : Monster(
      MONSTER_STATS {
        /*HP*/            100,
      /*DEX*/           10,
      /*DAMAGE_DICE*/   8,   // 1d8 damage
      /*HIT_DICE*/      2,   // 2 dice per swing
      /*DODGE*/         8,
      /*desc*/          "The Lich King, Lord of Death",
      },
      start
    )
{}

int Lich_King::attack() {
    std::uniform_int_distribution<> die(1, stat_block_.DAMAGE_DICE);
    int total = 0;
    for (int i = 0; i < stat_block_.HIT_DICE; ++i)
        total += die(rng_);
    std::cout << "The Lich King casts a shadow bolt for " 
              << total << " damage!\n";
    return total;
}

void Lich_King::move(DIRECTION d) {
    // the Lich King can teleport randomly
    std::vector<Room*> nbrs;
    if (auto n = currentRoom_->north()) nbrs.push_back(n);
    if (auto s = currentRoom_->south()) nbrs.push_back(s);
    if (auto e = currentRoom_->east())  nbrs.push_back(e);
    if (auto w = currentRoom_->west())  nbrs.push_back(w);

    if (!nbrs.empty()) {
        std::uniform_int_distribution<> pick(0, nbrs.size()-1);
        currentRoom_ = nbrs[pick(rng_)];
        std::cout << "The Lich King teleports to room #" 
                  << currentRoom_->id() << "\n";
    }
}

void Lich_King::drop_loot() {
    std::cout << "The Lich King collapses and drops a phylactery!\n";
    // dungeon.spawn_item(currentRoom_, Object_ID::PHYLACTERY);
}
