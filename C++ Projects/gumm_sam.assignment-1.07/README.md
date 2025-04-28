# Monster Parser and Display Utility

This project is a C++ utility designed to parse a specially formatted file containing monster descriptions and display each monster's attributes. It demonstrates file I/O, string parsing, dynamic memory allocation, and basic object-oriented programming in C++.

## Features

- **File Parsing:** Reads a file containing monster data in a custom format.
- **Monster Representation:** Uses a `Monster` class to represent the individual monsters with various attributes such as name, description, color, speed, abilities, health, damage, symbol, and appearance probability.
- **Dice Roll Simulation:** Implements a `rollDice` function to compute monster attributes using dice notation (e.g., `10+2d6`).
- **Dynamic Memory Management:** Allocates memory dynamically for storing monsters and demonstrates proper clean-up.
- **Console Display:** Each monster can display its attributes via the `display()` method.

## Code Structure

### Monster Class

The `Monster` class encapsulates all attributes and behaviors of a monster. Key features include:

- **Attributes:**
  - `NAME`: Name of the monster.
  - `DESC`: Textual description (multiline supported).
  - `COLOR`: List of color keywords.
  - `SPEED`: Speed value calculated using dice notation.
  - `ABIL`: Abilities of the monster.
  - `HP`: Health points (determined via dice rolls).
  - `DAM`: Damage value (determined via dice rolls).
  - `SYMB`: A character symbol representing the monster in-game.
  - `RRTY`: A numerical value (1–100) representing the monster's rarity.
  - `speedDice`, `hpDice`, `damDice`: Strings used for displaying the dice notation.

- **Member Functions:**
  - `rollDice(std::string rollDice)`: Parses and simulates dice rolls based on a string input.
  - `display()`: Outputs the monster's attributes to standard output.
  - Placeholder methods such as `attack()` and `move()` for future behavior expansion.
  - Constructors and a destructor to manage initialization and clean-up.

### Parser Class

The `Parser` class is responsible for reading the monster description file and converting its contents into `Monster` objects.

- **Key Methods:**
  - `checkAmountOfMonstersFromFile(std::string fileName)`: Scans the file and returns the count of monster entries (identified by "BEGIN MONSTER").
  - `extractor(std::string &line, std::string &attr, std::string marker)`: Helper function that extracts and processes a single attribute from a line.
  - `fieldExtractor(std::string &startingLinePos, std::ifstream &file)`: Processes a block of lines representing one monster and creates a new `Monster` object.
  - `readMonstersFromFile(std::string fileName)`: Reads the entire file, collects monster descriptions, and returns a dynamically allocated array of `std::vector<Monster>` containing the monsters.

### Main Function

- Prompts the user for the file name (which must be in the same folder as the executable).
- Uses the `Parser` to load and parse the monster file.
- Iterates through the parsed monsters and calls each monster's `display()` method to show its details.
- Manages dynamic memory by deallocating the monster list after use.

## File Format

The parser expects an input file formatted according to these specifications:

- **Header:** The file begins with a header line like `RLG327 MONSTER DESCRIPTION 1` (or a case-insensitive variant).
- **Monster Block:** Each monster description starts with the line `BEGIN MONSTER` and ends with a line containing `END`.
- **Fields:** Each monster block contains fields (e.g., `NAME`, `SYMB`, `COLOR`, `DESC`, `SPEED`, `DAM`, `HP`, `RRTY`, and `ABIL`) followed by their values.  
  - The `DESC` field may span multiple lines and terminates with a line containing only a period (`.`).
  - Numeric fields such as `SPEED`, `HP`, and `DAM` are represented in dice notation (e.g., `10+2d6`).

## Build Instructions

### Prerequisites

- A C++ compiler that supports C++11 or later (e.g., g++ version 4.8+).

### Compilation

Since the code uses `#include <bits/stdc++.h>`, it is geared toward GNU compilers. To compile using g++, run:

```bash
g++ -std=c++11 -o parser parser.cpp parser.h
```

Replace `parser.cpp` with the actual filename containing your code.

### Running

After successful compilation, run the executable:

```bash
./parser
```

You will be prompted for a file name. Ensure the file is in the same directory as the executable.

## Notes and Future Improvements

- **Non-Standard Header:** The project uses `#include <bits/stdc++.h>`, which is non-standard and primarily available with GNU compilers. 
- **Memory Management:** The current implementation manually allocates and deallocates memory.
- **Parser Robustness:** The parser assumes the file strictly adheres to the specified format. Adding more error handling or format validation would improve robustness.
- **Extend Functionality:** Placeholder methods such as `attack()` and `move()` in the `Monster` class can be further developed to implement actual game logic.
