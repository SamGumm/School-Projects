#ifndef DUNGEON_H
#define DUNGEON_H

#include <climits>
#include <stdbool.h>

#define INF INT_MAX
#define DUNGEON_WIDTH 80
#define DUNGEON_HEIGHT 21
#define DEFAULT_NMON 10
#define PC_SPEED 10
#define INF INT_MAX
#define MIN_ROOM_WIDTH 4
#define MIN_ROOM_HEIGHT 3
#define MAX_ROOM_WIDTH 12
#define MAX_ROOM_HEIGHT 8
#define MIN_ROOMS 6
#define MAX_ROOMS 20
#define MAX_ROOM_ATTEMPTS 2000


// Position structure.
typedef struct {
    int x, y;
} position_t;

// Dungeon structure.
typedef struct dungeon {
    char map[DUNGEON_HEIGHT][DUNGEON_WIDTH];
    int hardness[DUNGEON_HEIGHT][DUNGEON_WIDTH];
    position_t char_pos;
} dungeon_t;

// Room structure.
typedef struct room {
    int x, y;
    int width, height;
} room_t;

// Cell structure for Dijkstra's algorithm.
typedef struct cell {
    position_t position;
    int distance;
} cell_t;

// Characteristics for monsters (and possibly PC).
typedef struct characteristics {
    int intelligence;
    int telepathy;
    int tunneling;
    int erratic;
} characteristics_t;

// Character structure for PC and monsters.
typedef struct character {
    bool alive;
    int speed;
    char symbol;
    position_t pos;
    characteristics_t c;
    char memory[DUNGEON_HEIGHT][DUNGEON_WIDTH];
    bool fullVisibility;
    bool teleportMode;
    position_t target;
} character_t;

#endif // DUNGEON_H
