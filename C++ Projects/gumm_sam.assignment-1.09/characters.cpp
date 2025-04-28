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
Player::Player() {
    NAME     = "The PC";
    DESC     = "An adventurer looking for fame and glory";
    SYMB     = '@';
    SPEED    = 10;
    HP       = 10;
    position = {0,0};
};

Player::Player(position_t start, const std::string& name) {
    NAME     = name;
    DESC     = "The PC";
    SYMB     = '@';
    SPEED    = 10;
    HP       = 20;
    DAM = "0+1d4";
    position = start;
}

int Player::attack() {
    Dice d;
    int total = d.rollDice(DAM);
    for (auto *o : equip_)
      if (o) total += (o->damage); 
    return total;
}
  

