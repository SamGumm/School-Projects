# The Reaches of Nagazod

A simple, terminal-based roguelike dungeon crawler. Explore three floors of interconnected rooms, collect loot, equip gear, and battle monsters and bosses.

---

## Features

- **Hand-Built Dungeon** with three pre-built levels  
- **Room types**: corridors, treasure vaults, stash rooms, monster lairs, boss chambers, stairs  
- **One-time events**: guaranteed high-tier loot, random low-tier loot, random monster encounters, boss fights  
- **ASCII mini-maps**: each room is drawn to scale in your terminal  
- **Stats & equipment**: collect and equip armor and weapons to boost HP, STR, DEX, etc.  
- **Turn-based combat**: simple hit/dodge rolls, multi-dice damage, RNG driven by `std::mt19937`  
- **C++20 code**: smart pointers, `enum class`, `unique_ptr`, lambdas, standalone Makefile
---
# Implementation Details (some)
- Uses a BFS traversal method for various reasons
- A Dungeon is a graph of Room objects
- Fully OOP (for the most part)
- Attempted to make things mostly bulletproof (kind of lol)
---
# Future Plans
- Implement ncurses and panel.h to make a terminal-based game screen
- Add more monsters
- Implement more dice rolling within the actual gameloop

---

## Prerequisites

- A C++20-compatible compiler (e.g. `g++` 10 or newer)  
- GNU Make  

---

## Building

From the project root:

```bash
make td
./td
