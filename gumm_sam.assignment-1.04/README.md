COMS 3270 - Assignment 1.04

Overview:
This assignment is step 4 of a roguelike dungeon crawler. There is both 
a player character (PC) and monsters. Discrete Event Simulation is used,
with a priority queue, to manage character moves based on speed. The PC
is represented with '@', moves randomly within a room (does not move down
corridors yet) while monsters (represented by hexadecimal digits derived
from their characteristics bit field) move around with simple logic and 
some randomness.

Compilation:
gcc -Wall -Wextra -g -o start dungeon.c heap.c

Usage:
<./start [n]> where n is num of monsters to initialize (optional)

Notes:
Monsters attack both the PC and each other, (unless theres a certain type of monster?)
    - in the future maybe
Attacks work by the new arrival to a tile kills the current occupant
    - maybe have speed play into this?
    - health added soon in the structs, along with a damage int?
There is no intelligent pathfinding currently
    - use dijkstra here