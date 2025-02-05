#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
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

void swap(Position *a, Position *b) {
    Position temp = *a;
    *a = *b;
    *b = temp;
}

void shufflePositions(Position positions[], int n) {
    for(int i = n-1; i > 0; i--) {
        int j = rand() % (i+1);
        swap(&positions[i], &positions[j]);
    }
}

void generateBoundaries(char map[][COL]) {
    //The outermost cells of the dungeon are immutable, thus they must remain rock and cannot be part of any room or corridor
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            // if(i == 0 || j == 0 || i == ROW - 1 || j == COL - 1) {
            //     map[i][j] = '8'; //become a border
            // }
            map[i][j] = ' '; //become a rock
        }
    }
}

void generateRooms(char map[][COL]) {
    //Rooms cannot contact one another. There must be at least 1 cell of non-room between any two different rooms.
    //Room cells should be drawn with periods
    //Require at least 6 rooms per dungeon
    //Each room measures at least 4 units in the x direction and at least 3 units in the y direction.


    // Calculate the number of possible top-left positions.
    // For a room that is 3 rows tall and 4 columns wide,
    // the top-left row can be 0 to ROW - 3 (inclusive),
    // and the top-left col can be 0 to COL - 4 (inclusive).
    int numPositions = (ROW - 3 + 1) * (COL - 4 + 1);

    int roomCount = 0;
    const int desiredRooms = 6;

    //create positions map to contain all possible places to start a room
    Position *positions = malloc(numPositions * sizeof(Position));
    if(positions == NULL) {
        fprintf(stderr, "Memory Allocation in generateRooms() failed.\n");
        exit(1);
    }

    //fill in the array
    int index = 0;
    for(int r = 0; r <= ROW - 3; r++) {
        for (int c = 0; c <= COL - 4; c++) {
            positions[index].row = r;
            positions[index].col = c;
            index++;
        }
    }

    //seed the random generator
    srand((unsigned int)time(NULL));

    //shuffle positions for random placement
    shufflePositions(positions, numPositions);

    //placing the rooms
    for(int k = 0; k < numPositions && roomCount < desiredRooms; k++) {
        int r = positions[k].row;
        int c = positions[k].col;
        int canPlace = 1; //flag for determining if the room can be placed at current spot

        //check the area of the room plus a 1-cell padded border
        //room covers rows [r, r+2] and columns [c, c+3]
        //therefore we check from (r-1) to (r+3) and (c-1) to (c+4)
        for(int i = r - 1; i <= r + 3; i++) {
            for (int j = c - 1; j <= c + 4; j++) {
                //if (i,j) is out of bounds, then skip
                if (i < 0 || i >= ROW || j < 0 || j >= COL) {
                    continue;
                }
                if(map[i][j] == '.' || map[i][j] == '<' || map[i][j] == '>') {
                    canPlace = 0;
                    break;
                }
            }
            if(!canPlace) {
                break;
            }
        }

        if(canPlace) {
            for(int i = r; i < r + 3; i++) {
                for(int j = c; j < c + 4; j++) {
                    map[i][j] = '.';
                }
            }
            roomCount++;
        }
    }

    free(positions);
}

void drawCorridor(Position start, Position end, char map[][COL]) {\
    //TODO: have it not print on room cells
    int row = start.row;
    int col = start.col;
    // Mark the starting cell as corridor. *NEEDS to have it not on the room cells
    map[row][col] = '#';
    
    // First, move vertically until reaching the target row.
    while (row != end.row) {
        if (row < end.row)
            row++;
        else
            row--;
        map[row][col] = '#';
    }
    // Then, move horizontally until reaching the target column.
    while (col != end.col) {
        if (col < end.col)
            col++;
        else
            col--;
        map[row][col] = '#';
    }
}

void generateCorridors(char map[][COL]) {
    // corridors are marked with hashes and cannot encroach upon rooms
    // Allocate memory for 6 rooms.
    RoomArea *rooms = malloc(6 * sizeof(RoomArea));
    if (rooms == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    int index = 0;
    // Loop over the map cells to detect room top-left corners.
    for (int r = 0; r < ROW && index < 6; r++) {
        for (int c = 0; c < COL && index < 6; c++) {
            // A room’s top-left is assumed to have no room cell immediately above or to the left.
            if (map[r][c] == '.' &&
                (r == 0 || map[r - 1][c] != '.') &&
                (c == 0 || map[r][c - 1] != '.'))
            {
                // Verify that the room fits (3 rows, 4 columns).
                if (r + 2 < ROW && c + 3 < COL) {
                    rooms[index].start.row = r;
                    rooms[index].start.col = c;
                    rooms[index].end.row = r + 2;
                    rooms[index].end.col = c + 3;
                    index++;
                    
                    // Skip the remainder of this room's columns so we don't detect it again.
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

    // Determine the connection points for the stairs.
    Position leftStairsConn, rightStairsConn;
    // Check top-left vs bottom-left for the up stairs ('<').
    if (map[0][0] == '<') {
        // Top-left stairs block: choose a point on its right edge.
        leftStairsConn.row = 1;   // roughly centered vertically in the 4x4 block
        leftStairsConn.col = 3;   // rightmost column of the 4x4 block (columns 0-3)
    } else if (map[ROW - 1][0] == '<') {
        // Bottom-left stairs block.
        leftStairsConn.row = ROW - 2;
        leftStairsConn.col = 3;
    } else {
        // Fallback in case the stairs are not detected.
        leftStairsConn.row = 1;
        leftStairsConn.col = 3;
    }
    // For the right stairs ('>'), check top-right vs bottom-right.
    if (map[0][COL - 1] == '>') {
        // Top-right stairs block.
        rightStairsConn.row = 1;
        rightStairsConn.col = COL - 4;  // leftmost column of the right 4x4 block (columns COL-4 .. COL-1)
    } else if (map[ROW - 1][COL - 1] == '>') {
        // Bottom-right stairs block.
        rightStairsConn.row = ROW - 2;
        rightStairsConn.col = COL - 4;
    } else {
        rightStairsConn.row = 1;
        rightStairsConn.col = COL - 4;
    }

    // Now build the corridor chain:
    // 1. Connect left stairs to the first room.
    // For the room, choose its left connection point (on its left edge).
    Position firstRoomConn;
    firstRoomConn.row = rooms[0].start.row + 1;
    firstRoomConn.col = rooms[0].start.col;  // left edge
    printf("Connecting left stairs to room 0:\n");
    printf("Left stairs conn: (%d, %d) -> Room0 left conn: (%d, %d)\n",
           leftStairsConn.row, leftStairsConn.col,
           firstRoomConn.row, firstRoomConn.col);
    drawCorridor(leftStairsConn, firstRoomConn, map);

    // 2. Connect rooms together.
    // For each room, define:
    //   - Right connection point = (start.row+1, end.col)
    //   - Left connection point of the next room = (start.row+1, start.col)
    for (int i = 0; i + 1 < index; i++) {
        Position roomRight, nextRoomLeft;
        roomRight.row = rooms[i].start.row + 1;
        roomRight.col = rooms[i].end.col;  // right edge of room i
        nextRoomLeft.row = rooms[i+1].start.row + 1;
        nextRoomLeft.col = rooms[i+1].start.col;  // left edge of room i+1

        printf("Connecting room %d to room %d:\n", i, i+1);
        printf("Room%d right conn: (%d, %d) -> Room%d left conn: (%d, %d)\n",
               i, roomRight.row, roomRight.col,
               i+1, nextRoomLeft.row, nextRoomLeft.col);
        drawCorridor(roomRight, nextRoomLeft, map);
    }

    // 3. Connect the last room to the right stairs.
    Position lastRoomConn;
    lastRoomConn.row = rooms[index-1].start.row + 1;
    lastRoomConn.col = rooms[index-1].end.col;  // right edge of the last room
    printf("Connecting room %d to right stairs:\n", index-1);
    printf("Room%d right conn: (%d, %d) -> Right stairs conn: (%d, %d)\n",
           index-1, lastRoomConn.row, lastRoomConn.col,
           rightStairsConn.row, rightStairsConn.col);
    drawCorridor(lastRoomConn, rightStairsConn, map);

    free(rooms);
}

void generateStairs(char map[][COL]) {
    // Up staircases are represented with '<'
    // Down staircases are represented with '>'
    // Staircases are placed in a 4x4 block.
    // A coin flip determines where these blocks are placed.
    int coinFlip = rand() % 2;
    if (coinFlip == 1) {
        // Place a 4x4 block of up staircases in the top-left corner.
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                map[i][j] = '<';
            }
        }
        // Place a 4x4 block of down staircases in the bottom-right corner.
        for (int i = ROW - 4; i < ROW; i++) {
            for (int j = COL - 4; j < COL; j++) {
                map[i][j] = '>';
            }
        }
    }
    else {
        for (int i = ROW - 4; i < ROW; i++) {
            for (int j = 0; j < 4; j++) {
                map[i][j] = '<';
            }
        }
        // Place a 4x4 block of down staircases in the bottom-right corner.
        for (int i = 0; i < 4; i++) {
            for (int j = COL - 4; j < COL; j++) {
                map[i][j] = '>';
            }
        }
    }
}

void generateRock() {
    //have a hardness field to mimic veins of minerals
    //maybe have it look at surrounding cells to determine rock type
    //and either continue a vein or stop and do something else
    //need to determine what input would be 
    //rock is represented with spaces
}

/*
    REQUIREMENTS:
        - The dungeon should be fully connected, meaning that from any position on the floor, your adventurer
            should be able to walk to any other position on the floor without passing through rock.
    TODO:
        - set parameters
            - Dungeon measures 80 units in the x (horizontal) direction and 21 units in the y (vertical) direction. A
                standard terminal is 80 × 24, and limiting the dungeon to 21 rows leaves three rows for text, things
                like gameplay messages and player status, which come later.
        - create boundaries
        - create rooms
        - create stairs
    */
int main() {
    // Initialize the map with a default character (e.g., a space).
    char map[ROW][COL];
    //int numRooms = 6;
    //RoomArea *rooms = findRooms(map, &numRooms);

    // Generate rooms on the map.
    generateBoundaries(map);
    generateStairs(map);
    generateRooms(map);
    generateCorridors(map);

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            putchar(map[i][j]);
        }
        putchar('\n');
    }

    return 0;
}