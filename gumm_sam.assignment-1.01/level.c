/*
 * dungeon.c
 *
 * A simple dungeon generator for a roguelike game.
 * Dungeon dimensions: 80 (width) x 21 (height).
 * Rooms: at least 6, minimum room size: 4 (x) x 3 (y).
 * Corridors connect rooms (using L-shaped paths) and do not override room floor.
 * Staircases: one up (<) and one down (>) placed in room floor cells.
 *
 * Compile with: gcc -o dungeon dungeon.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define DUNGEON_WIDTH 80
#define DUNGEON_HEIGHT 21

#define MIN_ROOM_WIDTH 4
#define MIN_ROOM_HEIGHT 3
#define MAX_ROOM_WIDTH 12
#define MAX_ROOM_HEIGHT 8

#define MIN_ROOMS 6
#define MAX_ROOMS 20
#define MAX_ROOM_ATTEMPTS 2000

/* A room is represented by its top-left corner and dimensions */
typedef struct Room {
    int x;
    int y;
    int width;
    int height;
} Room;

/*
 * rooms_overlap
 *
 * Returns true if room a and room b “touch” or overlap when considering that
 * there must be at least 1 cell of gap between them. In other words, we expand each
 * room by one cell in every direction and then test for intersection.
 */
bool rooms_overlap(Room a, Room b) {
    if ((a.x - 1) > (b.x + b.width) || (b.x - 1) > (a.x + a.width))
        return false;
    if ((a.y - 1) > (b.y + b.height) || (b.y - 1) > (a.y + a.height))
        return false;
    return true;
}

/*
 * carve_corridor
 *
 * Carves a corridor between (x1, y1) and (x2, y2) in an L-shaped path.
 * A random decision determines whether to go horizontal then vertical or vice versa.
 * Only rock cells (represented by ' ') are overwritten with corridor cells ('#').
 * Room floor cells ('.') are left unchanged.
 */
void carve_corridor(char dungeon[DUNGEON_HEIGHT][DUNGEON_WIDTH],
                      int x1, int y1, int x2, int y2) {
    int x, y;
    bool horizontal_first = rand() % 2;

    if (horizontal_first) {
        /* Horizontal move */
        if (x1 <= x2) {
            for (x = x1; x <= x2; x++) {
                if (dungeon[y1][x] == ' ')
                    dungeon[y1][x] = '#';
            }
        } else {
            for (x = x1; x >= x2; x--) {
                if (dungeon[y1][x] == ' ')
                    dungeon[y1][x] = '#';
            }
        }
        /* Vertical move */
        if (y1 <= y2) {
            for (y = y1; y <= y2; y++) {
                if (dungeon[y][x2] == ' ')
                    dungeon[y][x2] = '#';
            }
        } else {
            for (y = y1; y >= y2; y--) {
                if (dungeon[y][x2] == ' ')
                    dungeon[y][x2] = '#';
            }
        }
    } else {
        /* Vertical move */
        if (y1 <= y2) {
            for (y = y1; y <= y2; y++) {
                if (dungeon[y][x1] == ' ')
                    dungeon[y][x1] = '#';
            }
        } else {
            for (y = y1; y >= y2; y--) {
                if (dungeon[y][x1] == ' ')
                    dungeon[y][x1] = '#';
            }
        }
        /* Horizontal move */
        if (x1 <= x2) {
            for (x = x1; x <= x2; x++) {
                if (dungeon[y2][x] == ' ')
                    dungeon[y2][x] = '#';
            }
        } else {
            for (x = x1; x >= x2; x--) {
                if (dungeon[y2][x] == ' ')
                    dungeon[y2][x] = '#';
            }
        }
    }
}

int main(void) {
    /* Seed the random number generator */
    srand((unsigned) time(NULL));

    /* Clear the terminal before each iteration */
    system("clear");

    /* The dungeon is represented by a 2D array of characters.
       ' ' (space) represents rock.
       '.' represents room floor.
       '#' represents corridor floor.
       '<' represents an upward staircase.
       '>' represents a downward staircase.
    */
    char dungeon[DUNGEON_HEIGHT][DUNGEON_WIDTH];
    /*Placeholder for eventual use*/
    int hardness[DUNGEON_HEIGHT][DUNGEON_WIDTH];

    Room rooms[MAX_ROOMS];
    int num_rooms = 0;
    int i, j;

    /* Initialize dungeon cells:
       - The border (outermost cells) remains rock and is immutable.
       - Interior cells start as rock with a random hardness.
    */
    for (int y = 0; y < DUNGEON_HEIGHT; y++) {
        for (int x = 0; x < DUNGEON_WIDTH; x++) {
            if (y == 0 || y == DUNGEON_HEIGHT - 1 || x == 0 || x == DUNGEON_WIDTH - 1) {
                dungeon[y][x] = ' ';  // immutable rock
                hardness[y][x] = 255;
            } else {
                dungeon[y][x] = ' ';  // rock
                hardness[y][x] = (rand() % 254) + 1;  // hardness between 1 and 254
            }
        }
    }

    /* Attempt to place at least MIN_ROOMS rooms.
       We try up to MAX_ROOM_ATTEMPTS times.
       Each room is randomly sized and placed such that it fits inside the borders.
       The room must not contact (or overlap) any existing room (with a 1-cell gap).
    */
    int attempts = 0;
    while (attempts < MAX_ROOM_ATTEMPTS && num_rooms < MIN_ROOMS) {
        int room_width = MIN_ROOM_WIDTH + rand() % (MAX_ROOM_WIDTH - MIN_ROOM_WIDTH + 1);
        int room_height = MIN_ROOM_HEIGHT + rand() % (MAX_ROOM_HEIGHT - MIN_ROOM_HEIGHT + 1);
        int room_x = 1 + rand() % (DUNGEON_WIDTH - room_width - 2);
        int room_y = 1 + rand() % (DUNGEON_HEIGHT - room_height - 2);

        Room new_room = { room_x, room_y, room_width, room_height };
        bool overlap = false;
        for (i = 0; i < num_rooms; i++) {
            if (rooms_overlap(new_room, rooms[i])) {
                overlap = true;
                break;
            }
        }
        if (!overlap) {
            /* Place the room by marking its cells as floor ('.') and setting hardness to 0 */
            for (j = new_room.y; j < new_room.y + new_room.height; j++) {
                for (i = new_room.x; i < new_room.x + new_room.width; i++) {
                    dungeon[j][i] = '.';
                    hardness[j][i] = 0;
                }
            }
            rooms[num_rooms++] = new_room;
        }
        attempts++;
    }

    if (num_rooms < MIN_ROOMS) {
        fprintf(stderr, "Error: Only %d rooms could be placed after %d attempts.\n",
                num_rooms, attempts);
        return 1;
    }

    /* Connect the rooms with corridors.
       For each room (starting with the second), find the closest room among those already
       placed and carve an L-shaped corridor.
    */
    for (int r = 1; r < num_rooms; r++) {
        int room1_center_x = rooms[r].x + rooms[r].width / 2;
        int room1_center_y = rooms[r].y + rooms[r].height / 2;
        int closest_room_index = 0;
        int min_dist_sq = 999999;
        for (int k = 0; k < r; k++) {
            int room2_center_x = rooms[k].x + rooms[k].width / 2;
            int room2_center_y = rooms[k].y + rooms[k].height / 2;
            int dx = room1_center_x - room2_center_x;
            int dy = room1_center_y - room2_center_y;
            int dist_sq = dx * dx + dy * dy;
            if (dist_sq < min_dist_sq) {
                min_dist_sq = dist_sq;
                closest_room_index = k;
            }
        }
        int room2_center_x = rooms[closest_room_index].x + rooms[closest_room_index].width / 2;
        int room2_center_y = rooms[closest_room_index].y + rooms[closest_room_index].height / 2;
        carve_corridor(dungeon, room1_center_x, room1_center_y,
                       room2_center_x, room2_center_y);
    }

    /* Place staircases.
       - Up staircase: '<'
       - Down staircase: '>'
       Both must be placed on a floor cell (inside a room).
    */
    int up_stair_x, up_stair_y, down_stair_x, down_stair_y;
    {
        /* Choose a random room for the up staircase */
        int room_index = rand() % num_rooms;
        up_stair_x = rooms[room_index].x + rand() % rooms[room_index].width;
        up_stair_y = rooms[room_index].y + rand() % rooms[room_index].height;
        dungeon[up_stair_y][up_stair_x] = '<';
    }
    {
        /* Choose a random room for the down staircase (ensure a different cell) */
        int room_index;
        do {
            room_index = rand() % num_rooms;
            down_stair_x = rooms[room_index].x + rand() % rooms[room_index].width;
            down_stair_y = rooms[room_index].y + rand() % rooms[room_index].height;
        } while (down_stair_x == up_stair_x && down_stair_y == up_stair_y);
        dungeon[down_stair_y][down_stair_x] = '>';
    }

    /* Print the dungeon to standard output */
    for (int y = 0; y < DUNGEON_HEIGHT; y++) {
        for (int x = 0; x < DUNGEON_WIDTH; x++) {
            putchar(dungeon[y][x]);
        }
        putchar('\n');
    }

    return 0;
}
