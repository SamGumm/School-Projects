#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>

//------------------------------------------------------------------------------
// Monster Class Declaration
//------------------------------------------------------------------------------
// Represents a monster and its attributes as defined by your file format.
class Monster {
public:
    // Attributes
    std::string NAME;         // Monster name (terminated by newline)
    std::string DESC;         // Monster description (max 77 bytes, newline terminated)
    std::vector<std::string> COLOR; // List of color keywords (space-delineated, newline terminated)
    int SPEED;                // Speed value determined by dice rolls (newline terminated)
    std::string ABIL;         // Monster abilities (newline terminated)
    int HP;                   // Health points (newline terminated)
    int DAM;                  // Damage value (newline terminated)
    char SYMB;                // In-game symbol for the monster
    int RRTY;                 // Rarity: inclusive probability (1 to 100) of this monster appearing
    std::string speedDice;    // String representation of the dice roll for speed
    std::string hpDice;       // String representation of the dice roll for HP
    std::string damDice;      // String representation of the dice roll for damage

    // Constructors / Destructor
    Monster();
    Monster(std::string name, std::string desc, std::string color, std::string speed, 
            std::string abil, std::string hp, std::string dam, std::string symb, 
            std::string rrty);
    ~Monster();

    // Member Functions
    void attack();            // Placeholder for attack functionality
    void move();              // Placeholder for movement functionality
    int rollDice(std::string rollDice); // Parses dice notation and simulates dice rolls
    void display();           // Prints out the monster's attributes
};

//------------------------------------------------------------------------------
// Parser Class Declaration
//------------------------------------------------------------------------------
// Provides functions for reading, parsing, and extracting monster data from a file.
class Parser {
public:
    // Returns the number of monsters found in the file.
    int checkAmountOfMonstersFromFile(std::string fileName);

    // Extracts a particular attribute from a line using the provided marker.
    void extractor(std::string &line, std::string &attr, std::string marker);

    // Processes a block of text starting at 'startingLinePos' from the file and
    // returns a pointer to a new Monster object.
    Monster* fieldExtractor(std::string &startingLinePos, std::ifstream &file);

    // Reads monsters from the specified file and returns a pointer to a dynamically
    // allocated vector of Monster objects.
    std::vector<Monster>* readMonstersFromFile(std::string fileName);
};

#endif // PARSER_H
