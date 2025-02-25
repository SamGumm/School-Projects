#ifndef DUNGEON_H
#define DUNGEON_H

#include <stdbool.h>

//dungeon dimensions
#define DUNGEON_WIDTH 80
#define DUNGEON_HEIGHT 21

//room dimensions
#define MIN_ROOM_WIDTH 4
#define MIN_ROOM_HEIGHT 3
#define MAX_ROOM_WIDTH 12
#define MAX_ROOM_HEIGHT 8

//room and attempts limits
#define MIN_ROOMS 6
#define MAX_ROOMS 20
#define MAX_ROOM_ATTEMPTS 2000

//room structure definition
typedef struct Room {
    int x;
    int y;
    int width;
    int height;
} Room;

//function prototypes
/*
 * rooms_overlap
 *
 * Returns true if room a and room b “touch” or overlap when considering that
 * there must be at least 1 cell of gap between them. In other words, we expand each
 * room by one cell in every direction and then test for intersection.
 */
bool rooms_overlap(Room a, Room b);

/*
 * carve_corridor
 *
 * Carves a corridor between (x1, y1) and (x2, y2) in an L-shaped path.
 * A random decision determines whether to go horizontal then vertical or vice versa.
 * Only rock cells (represented by ' ') are overwritten with corridor cells ('#').
 * Room floor cells ('.') are left unchanged.
 */
void carve_corridor(char dungeon[DUNGEON_HEIGHT][DUNGEON_WIDTH], int x1, int y1, int x2, int y2);

#endif /* DUNGEON_H*/