// dice.hpp
#ifndef DICE_HPP
#define DICE_HPP

#include <string>
#include <random>

// A dice‐roller that takes the format "C+NdF",
// e.g. "3+2d6" means 3 + (roll 2 six‑sided dice).
class Dice {
public:
    Dice() = default;
    ~Dice() = default;

    /// Parse and roll the given expression.
    /// @param expr A string like "0+1d6" or "3+2d8"
    /// @return constant + sum of N uniform distro [1..F] rolls
    int rollDice(const std::string &expr) const;
};

#endif // DICE_HPP

