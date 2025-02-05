#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define ROW 21
#define COL 80

typedef struct {
    int row;
    int col;
} Position;

typedef struct {
    int hardness;
    char rep;
} Rock;

typedef struct {
    Position start;
    Position end;
} RoomArea;

// Swap two Position values.
void swap(Position *a, Position *b) {
    Position temp = *a;
    *a = *b;
    *b = temp;
}

// shuffle for an array of Position.
void shufflePositions(Position positions[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(&positions[i], &positions[j]);
    }
}

// generateBoundaries()
// Set every cell in the map to a rock (represented as a space).
void generateBoundaries(char map[][COL]) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            // For now, every cell becomes rock.
            map[i][j] = ' '; 
        }
    }
}

// generateRooms()
// Attempts to place at least 6 rooms into the map.
// Rooms are 3 rows tall and 4 columns wide. A 1-cell border around each room is enforced.
void generateRooms(char map[][COL]) {
    int numPositions = (ROW - 3 + 1) * (COL - 4 + 1);
    int roomCount = 0;
    const int desiredRooms = 6;

    // Allocate an array for all potential top-left positions.
    Position *positions = malloc(numPositions * sizeof(Position));
    if (positions == NULL) {
        fprintf(stderr, "Memory Allocation in generateRooms() failed.\n");
        exit(1);
    }

    // Fill the array with all possible top-left positions.
    int index = 0;
    for (int r = 0; r <= ROW - 3; r++) {
        for (int c = 0; c <= COL - 4; c++) {
            positions[index].row = r;
            positions[index].col = c;
            index++;
        }
    }

    // Seed the random number generator.
    srand((unsigned int)time(NULL));

    // Shuffle the positions to randomize room placement.
    shufflePositions(positions, numPositions);

    // Try to place rooms from the shuffled positions.
    for (int k = 0; k < numPositions && roomCount < desiredRooms; k++) {
        int r = positions[k].row;
        int c = positions[k].col;
        int canPlace = 1;  // flag indicating if a room can be placed here

        // Check the room area plus a 1-cell padded border.
        // The room covers rows [r, r+2] and columns [c, c+3].
        // So we check from (r-1) to (r+3) and (c-1) to (c+4).
        for (int i = r - 1; i <= r + 3; i++) {
            for (int j = c - 1; j <= c + 4; j++) {
                // If (i,j) is out-of-bounds, skip.
                if (i < 0 || i >= ROW || j < 0 || j >= COL) {
                    continue;
                }
                // If any cell in the padded area is already a room cell ('.'), then we cannot place a room here.
                if (map[i][j] == '.') {
                    canPlace = 0;
                    break;
                }
            }
            if (!canPlace) {
                break;
            }
        }

        // If the area is clear, draw the room.
        if (canPlace) {
            for (int i = r; i < r + 3; i++) {
                for (int j = c; j < c + 4; j++) {
                    map[i][j] = '.';
                }
            }
            roomCount++;
        }
    }

    free(positions);
}

// generateCorridors()
// Detects the room areas by scanning for a top-left corner of a room (a '.' with no '.' to its left or above)
// and stores the room areas in an array. The current implementation simply prints the room areas.
void generateCorridors(char map[][COL]) {
    // Allocate memory for 6 rooms.
    RoomArea *rooms = malloc(6 * sizeof(RoomArea));
    if (rooms == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    int index = 0;
    // Loop over the map cells.
    for (int r = 0; r < ROW && index < 6; r++) {
        for (int c = 0; c < COL && index < 6; c++) {
            // Only check if the cell is a room cell.
            // A room’s top-left is assumed to have no room cell immediately above or to the left.
            if (map[r][c] == '.' &&
                (r == 0 || map[r - 1][c] != '.') &&
                (c == 0 || map[r][c - 1] != '.'))
            {
                // Verify that the room fits.
                if (r + 2 < ROW && c + 3 < COL) {
                    rooms[index].start.row = r;
                    rooms[index].start.col = c;
                    rooms[index].end.row = r + 2;
                    rooms[index].end.col = c + 3;
                    index++;
                    
                    // Skip the remainder of this room's columns so we don't detect the same room again.
                    c += 3;  // room width is 4 columns (c through c+3)
                }
            }
        }
    }

    // Print out the detected room areas.
    for (int i = 0; i < index; i++) {
        printf("Room %d: ([%d, %d], [%d, %d])\n",
               i,
               rooms[i].start.row, rooms[i].start.col,
               rooms[i].end.row, rooms[i].end.col);
    }

    free(rooms);
}

void generateStairs(char map[][COL]) {
    // Up staircases are represented with '<'
    // Down staircases with '>'
    // This function should ensure at least one of each is placed in a floor cell.
    // (This routine is currently a stub for further development.)
}

void generateRock() {
    // This function could be used to add details (like hardness and veins) to the rock.
    // Rock is represented with ' ' (spaces) in the current dungeon.
    // Implementation details can be added here.
}

int main() {
    // Initialize the map as a 2D array.
    char map[ROW][COL];

    // Generate the dungeon components.
    generateBoundaries(map);
    generateRooms(map);
    generateCorridors(map);

    // Print the resulting map.
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            putchar(map[i][j]);
        }
        putchar('\n');
    }

    return 0;
}
