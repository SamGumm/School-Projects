// dice.cpp
#include "dice.hpp"

int Dice::rollDice(const std::string &rollDice) const {
    // Copy input
    std::string s = rollDice;
    std::string constantStr, diceRollStr1, diceRollStr2;
    int constant, numDice, numFaces;
    int result = 0;

    // Split on '+'
    auto p = s.find('+');
    constantStr = s.substr(0, p);
    s.erase(0, p + 1);

    // Split on 'd'
    p = s.find('d');
    diceRollStr1 = s.substr(0, p);
    s.erase(0, p + 1);
    diceRollStr2 = s;

    // Convert to ints
    constant  = std::stoi(constantStr);
    numDice   = std::stoi(diceRollStr1);
    numFaces  = std::stoi(diceRollStr2);

    // Roll the dice
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, numFaces);
    for (int i = 0; i < numDice; ++i) {
        result += dist(gen);
    }

    return constant + result;
}

