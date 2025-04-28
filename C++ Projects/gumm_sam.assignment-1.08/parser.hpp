#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include "characters.hpp"
#include "object_description.hpp"

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

    std::vector<MonsterDescription> readMonsterDescriptions(const std::string& filename);
    std::vector<ObjectDescription> readObjectDescriptions(const std::string& filename);
};

#endif // PARSER_H
