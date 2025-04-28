#ifndef GENERATE_DUNGEON_HPP
#define GENERATE_DUNGEON_HPP

#include <climits>
#include <ctime>

#define INF INT_MAX
#define DUNGEON_WIDTH 80
#define DUNGEON_HEIGHT 21
#define DEFAULT_NMON 10
#define MIN_ROOM_WIDTH 4
#define MIN_ROOM_HEIGHT 3
#define MAX_ROOM_WIDTH 12
#define MAX_ROOM_HEIGHT 8
#define MIN_ROOMS 6
#define MAX_ROOMS 20
#define MAX_ROOM_ATTEMPTS 2000

typedef struct Position {
    int x, y;
} position_t;

typedef struct DungeonMap {
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

class Dungeon;

#endif