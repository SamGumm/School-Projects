#include "characters.hpp"
#include <iostream>

//──────────────── Monster methods ─────────────────────────────
int Monster::attack() {
    Dice d;
    return d.rollDice(DAM);
}

void Monster::move(position_t newPos) {
    position = newPos;
}

void Monster::display() const {
    std::cout << NAME << '\n'
              << DESC << '\n'
              << "SYMB: " << SYMB << '\n'
              << "HP " << HP << "  SPD " << SPEED << '\n';
}

//──────────────── Player methods ──────────────────────────────
Player::Player() = default;

Player::Player(position_t start, const std::string& name) {
    NAME     = name;
    DESC     = "The PC";
    SYMB     = '@';
    SPEED    = 10;
    HP       = 10;
    position = start;
}
