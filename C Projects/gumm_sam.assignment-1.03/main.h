#ifndef DUNGEON_H
#define DUNGEON_H

#include <limits.h>  /* for INT_MAX */

#define DUNGEON_WIDTH 80
#define DUNGEON_HEIGHT 21
#define INF INT_MAX

/* Dungeon structure: holds the map, hardness values, and PC coordinates */
typedef struct dungeon {
    int hardness[DUNGEON_HEIGHT][DUNGEON_WIDTH];
    char map[DUNGEON_HEIGHT][DUNGEON_WIDTH];
    struct {
        int x, y;
    } pc;
} dungeon_t;

/* Cell structure used by Dijkstra's algorithm */
typedef struct cell {
    int x, y;
    int distance;
} cell_t;

/* Function prototypes for the dungeon and pathfinding routines */
void generate_dungeon(dungeon_t *d);
void place_pc(dungeon_t *d);
void render_dungeon(dungeon_t *d);
void render_distance_map(dungeon_t *d, int dist[DUNGEON_HEIGHT][DUNGEON_WIDTH]);
void dijkstra(dungeon_t *d, int tunnel, int dist[DUNGEON_HEIGHT][DUNGEON_WIDTH]);

#endif

