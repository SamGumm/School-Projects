#pragma once

#include "characters/nagazod_player.hpp"
#include "characters/nagazod_monster.hpp"
#include <iostream>

/// Manages a fight between one Player and one Monster.
class Encounter {
public:
    /// Construct an encounter. Both references must remain valid for the
    /// duration of engage().
    Encounter(Player& player, Monster& monster);

    /// Runs the fight loop, printing to std::cout.
    /// Returns true if the player survives; false if the player dies.
    bool engage();

private:
    Player&  player_;
    Monster& monster_;
};
