#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <arpa/inet.h>    // for htonl/htons/ntohl/ntohs
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <limits.h>       // for PATH_MAX

/* --- Constants --- */
#define ROW 21
#define COL 80
#define MAX_ROOMS 6       /* For generation we limit to 6 rooms */
#define FILE_MARKER "RLG327-S2025"

/* --- Structures --- */

/* A simple position with row and column.
   (When saving, x is the column and y is the row.) */
typedef struct {
    int row;
    int col;
} Position;

/* A room: upper–left corner (x,y) and size (width, height) */
typedef struct {
    uint8_t x;      // column (x position)
    uint8_t y;      // row (y position)
    uint8_t width;  // room width
    uint8_t height; // room height
} room_t;

/* --- Global Variables --- */

/* The dungeon “map” used for display */
char dungeon[ROW][COL];
/* A parallel hardness matrix: 0 for open, 255 for immutable rock */
uint8_t hardness[ROW][COL];

/* Room information (dynamically allocated) */
room_t *rooms = NULL;
int num_rooms = 0;

/* Staircase positions (we assume one up and one down staircase) */
Position up_stair, down_stair;

/* Player character (PC) position */
Position pc;

/* --- Utility Functions --- */

/* Swap two positions */
void swap(Position *a, Position *b) {
    Position temp = *a;
    *a = *b;
    *b = temp;
}

/* Shuffle an array of positions (Fisher–Yates shuffle) */
void shufflePositions(Position positions[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(&positions[i], &positions[j]);
    }
}

/* --- Dungeon Generation Functions --- */

/* generateBoundaries:
   Fills the entire dungeon with rock.
   Sets the border cells to immutable rock (hardness 255) and interior cells to random hardness (1–254).
   Also initializes the dungeon character display to a blank rock (space). */
void generateBoundaries(void) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (i == 0 || j == 0 || i == ROW - 1 || j == COL - 1) {
                hardness[i][j] = 255;  /* immutable rock */
            } else {
                hardness[i][j] = (uint8_t)(rand() % 254 + 1);
            }
            dungeon[i][j] = ' ';  /* rock represented as a space */
        }
    }
}

/* generateStairs:
   Places staircases in a 4×4 block.
   A coin flip determines which corners are used.
   Also records a single staircase coordinate for saving (choosing a fixed cell within the block)
   and sets the hardness to 0 for these cells. */
void generateStairs(void) {
    int coinFlip = rand() % 2;
    if (coinFlip == 1) {
        /* Up stairs in top–left block; down stairs in bottom–right block */
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                dungeon[i][j] = '<';
                hardness[i][j] = 0;
            }
        }
        for (int i = ROW - 4; i < ROW; i++) {
            for (int j = COL - 4; j < COL; j++) {
                dungeon[i][j] = '>';
                hardness[i][j] = 0;
            }
        }
        /* Record staircase positions (choose a representative cell in each block) */
        up_stair.row = 1;   up_stair.col = 1;
        down_stair.row = ROW - 2;   down_stair.col = COL - 2;
    } else {
        /* Up stairs in bottom–left block; down stairs in top–right block */
        for (int i = ROW - 4; i < ROW; i++) {
            for (int j = 0; j < 4; j++) {
                dungeon[i][j] = '<';
                hardness[i][j] = 0;
            }
        }
        for (int i = 0; i < 4; i++) {
            for (int j = COL - 4; j < COL; j++) {
                dungeon[i][j] = '>';
                hardness[i][j] = 0;
            }
        }
        up_stair.row = ROW - 2;   up_stair.col = 1;
        down_stair.row = 1;       down_stair.col = COL - 2;
    }
}

/* generateRooms:
   Randomly places 6 rooms of fixed size (3 rows tall × 4 columns wide).
   Rooms must have a one–cell border between one another.
*/
void generateRooms(void) {
    num_rooms = 0;
    rooms = malloc(MAX_ROOMS * sizeof(room_t));
    if (!rooms) {
        fprintf(stderr, "Memory allocation for rooms failed.\n");
        exit(1);
    }

    int numPositions = (ROW - 3 + 1) * (COL - 4 + 1);
    Position *positions = malloc(numPositions * sizeof(Position));
    if (!positions) {
        fprintf(stderr, "Memory allocation for room positions failed.\n");
        exit(1);
    }
    int index = 0;
    for (int r = 0; r <= ROW - 3; r++) {
        for (int c = 0; c <= COL - 4; c++) {
            positions[index].row = r;
            positions[index].col = c;
            index++;
        }
    }
    shufflePositions(positions, numPositions);

    for (int k = 0; k < numPositions && num_rooms < MAX_ROOMS; k++) {
        int r = positions[k].row;
        int c = positions[k].col;
        int canPlace = 1;
        /* Check area for room plus one–cell border:
           room spans rows [r, r+2] and cols [c, c+3] */
        for (int i = r - 1; i <= r + 3; i++) {
            for (int j = c - 1; j <= c + 4; j++) {
                if (i < 0 || i >= ROW || j < 0 || j >= COL)
                    continue;
                if (dungeon[i][j] == '.' || dungeon[i][j] == '<' || dungeon[i][j] == '>')
                {
                    canPlace = 0;
                    break;
                }
            }
            if (!canPlace)
                break;
        }
        if (canPlace) {
            /* Carve out the room */
            for (int i = r; i < r + 3; i++) {
                for (int j = c; j < c + 4; j++) {
                    dungeon[i][j] = '.';
                    hardness[i][j] = 0;
                }
            }
            /* Save room data
            note: x is column and y is row */
            rooms[num_rooms].x = (uint8_t)c;
            rooms[num_rooms].y = (uint8_t)r;
            rooms[num_rooms].width = 4;
            rooms[num_rooms].height = 3;
            num_rooms++;
        }
    }
    free(positions);
}

/* drawCorridor:
   Draws an L–shaped corridor between start and end positions.
   First moves vertically then horizontally, setting dungeon cells to '#' and hardness to 0.
*/
void drawCorridor(Position start, Position end) {
    int row = start.row, col = start.col;
    dungeon[row][col] = '#';
    hardness[row][col] = 0;
    
    while (row != end.row) {
        row += (row < end.row) ? 1 : -1;
        dungeon[row][col] = '#';
        hardness[row][col] = 0;
    }
    while (col != end.col) {
        col += (col < end.col) ? 1 : -1;
        dungeon[row][col] = '#';
        hardness[row][col] = 0;
    }
}

/* generateCorridors:
   Connects the rooms and also connects the staircase blocks to the rooms.
   Uses the global rooms array.
   For room-to-room corridors:
     - Right connection point for room i: (rooms[i].x + width - 1, rooms[i].y + height/2)
     - Left connection point for room i+1: (rooms[i+1].x, rooms[i+1].y + height/2)
   Also connects:
     - The up staircase (up_stair) to the first room’s left–edge connection.
     - The last room’s right–edge connection to the down staircase (down_stair).
*/
void generateCorridors(void) {
    if (num_rooms == 0)
        return;

    /* Connect up staircase to first room */
    Position firstRoomConn;
    firstRoomConn.row = rooms[0].y + (rooms[0].height / 2);
    firstRoomConn.col = rooms[0].x;  /* left edge of room 0 */
    drawCorridor(up_stair, firstRoomConn);

    /* Connect consecutive rooms */
    for (int i = 0; i < num_rooms - 1; i++) {
        Position roomRight, nextRoomLeft;
        roomRight.row = rooms[i].y + (rooms[i].height / 2);
        roomRight.col = rooms[i].x + rooms[i].width - 1;
        nextRoomLeft.row = rooms[i+1].y + (rooms[i+1].height / 2);
        nextRoomLeft.col = rooms[i+1].x;
        drawCorridor(roomRight, nextRoomLeft);
    }
    /* Connect last room to down staircase */
    Position lastRoomConn;
    lastRoomConn.row = rooms[num_rooms - 1].y + (rooms[num_rooms - 1].height / 2);
    lastRoomConn.col = rooms[num_rooms - 1].x + rooms[num_rooms - 1].width - 1;
    drawCorridor(lastRoomConn, down_stair);
}

/* rebuildDungeon:
   After loading a dungeon from file (which supplies the hardness matrix, room data,
   staircase positions, and PC position), rebuild the character display.
     - Default: if hardness > 0, cell is rock (' ')
     - If hardness == 0, mark as corridor floor ('#')
     - Then overlay rooms ('.') according to room data.
     - Then place staircases ('<' and '>') and the PC ('@').
*/
void rebuildDungeon(void) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (hardness[i][j] == 0)
                dungeon[i][j] = '#';
            else
                dungeon[i][j] = ' ';
        }
    }
    /* Overlay rooms */
    for (int r = 0; r < num_rooms; r++) {
        for (int i = rooms[r].y; i < rooms[r].y + rooms[r].height; i++) {
            for (int j = rooms[r].x; j < rooms[r].x + rooms[r].width; j++) {
                dungeon[i][j] = '.';
            }
        }
    }
    /* Place staircases */
    dungeon[up_stair.row][up_stair.col] = '<';
    dungeon[down_stair.row][down_stair.col] = '>';
    /* Place PC */
    dungeon[pc.row][pc.col] = '@';
}

/* generate_dungeon:
   Calls the various generation routines to build a new dungeon.
   Updates the global dungeon, hardness, rooms, staircase and PC information.
*/
void generate_dungeon(void) {
    generateBoundaries();
    generateStairs();
    generateRooms();
    generateCorridors();
    /* Set PC at upper–left corner of room 0 if available; otherwise, default to (1,1) */
    if (num_rooms > 0) {
        pc.row = rooms[0].y;
        pc.col = rooms[0].x;
    } else {
        pc.row = 1;
        pc.col = 1;
    }
    dungeon[pc.row][pc.col] = '@';
}

/* display_dungeon:
   Prints the dungeon (the character map) to stdout.
*/
void display_dungeon(void) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            putchar(dungeon[i][j]);
        }
        putchar('\n');
    }
}

/* --- Save and Load Functions --- */

/* save_dungeon:
   Saves the current dungeon data to disk at $HOME/.rlg327/dungeon.
   Writes the header, version, file size, PC position, hardness matrix,
   room data, upward staircase data, and downward staircase data.
*/
int save_dungeon(void) {
    const char *home = getenv("HOME");
    if (!home) {
        fprintf(stderr, "Error: HOME not set.\n");
        return -1;
    }
    char dir_path[PATH_MAX];
    snprintf(dir_path, sizeof(dir_path), "%s/.rlg327", home);
    if (mkdir(dir_path, 0755) == -1 && errno != EEXIST) {
        perror("mkdir");
        return -1;
    }
    char file_path[PATH_MAX];
    int ret = snprintf(file_path, sizeof(file_path), "%s/dungeon", dir_path);
    if (ret < 0 || ret >= sizeof(file_path)) {
        fprintf(stderr, "Error: file path was truncated.\n");
        return -1;
    }

    
    FILE *fp = fopen(file_path, "wb");
    if (!fp) {
        perror("fopen");
        return -1;
    }
    
    /* Write file marker (12 bytes) */
    if (fwrite(FILE_MARKER, 1, 12, fp) != 12) {
        perror("fwrite marker");
        fclose(fp);
        return -1;
    }
    
    /* Write file version (4 bytes) */
    uint32_t version = htonl(0);
    if (fwrite(&version, sizeof(version), 1, fp) != 1) {
        perror("fwrite version");
        fclose(fp);
        return -1;
    }
    
    /* Compute file size:
       12 (marker) + 4 (version) + 4 (file size field) + 2 (PC position)
       + 1680 (hardness matrix) + 2 (num_rooms) + (num_rooms*4)
       + 2 (num_upstairs) + (upstairs count * 2) + 2 (num_downstairs) + (downstairs count * 2)
       Assuming one up and one down staircase.
    */
    uint32_t file_size = 12 + 4 + 4 + 2 + (ROW * COL) + 2 + (num_rooms * 4) + 2 + (1 * 2) + 2 + (1 * 2);
    uint32_t file_size_net = htonl(file_size);
    if (fwrite(&file_size_net, sizeof(file_size_net), 1, fp) != 1) {
        perror("fwrite file_size");
        fclose(fp);
        return -1;
    }
    
    /* Write PC position (2 bytes: x then y) */
    uint8_t pc_pos[2];
    pc_pos[0] = (uint8_t)pc.col;
    pc_pos[1] = (uint8_t)pc.row;
    if (fwrite(pc_pos, 1, 2, fp) != 2) {
        perror("fwrite PC position");
        fclose(fp);
        return -1;
    }
    
    /* Write the hardness matrix (1680 bytes) in row–major order */
    for (int i = 0; i < ROW; i++) {
        if (fwrite(hardness[i], 1, COL, fp) != COL) {
            perror("fwrite hardness row");
            fclose(fp);
            return -1;
        }
    }
    
    /* Write number of rooms (2 bytes) */
    uint16_t num_rooms_net = htons((uint16_t)num_rooms);
    if (fwrite(&num_rooms_net, sizeof(num_rooms_net), 1, fp) != 1) {
        perror("fwrite num_rooms");
        fclose(fp);
        return -1;
    }
    /* Write room data: for each room, 4 bytes: x, y, width, height */
    for (int i = 0; i < num_rooms; i++) {
        uint8_t room_data[4];
        room_data[0] = rooms[i].x;
        room_data[1] = rooms[i].y;
        room_data[2] = rooms[i].width;
        room_data[3] = rooms[i].height;
        if (fwrite(room_data, 1, 4, fp) != 4) {
            perror("fwrite room data");
            fclose(fp);
            return -1;
        }
    }
    
    /* Write number of upward staircases (2 bytes)*/
    uint16_t num_up = htons(1);
    if (fwrite(&num_up, sizeof(num_up), 1, fp) != 1) {
        perror("fwrite num upward stairs");
        fclose(fp);
        return -1;
    }
    /* Write the upward staircase’s position (2 bytes: x then y) */
    uint8_t up_data[2];
    up_data[0] = (uint8_t)up_stair.col;
    up_data[1] = (uint8_t)up_stair.row;
    if (fwrite(up_data, 1, 2, fp) != 2) {
        perror("fwrite upward stair data");
        fclose(fp);
        return -1;
    }
    
    /* Write number of downward staircases (2 bytes, network order); assume 1 */
    uint16_t num_down = htons(1);
    if (fwrite(&num_down, sizeof(num_down), 1, fp) != 1) {
        perror("fwrite num downward stairs");
        fclose(fp);
        return -1;
    }
    /* Write downward staircase’s position (2 bytes: x then y) */
    uint8_t down_data[2];
    down_data[0] = (uint8_t)down_stair.col;
    down_data[1] = (uint8_t)down_stair.row;
    if (fwrite(down_data, 1, 2, fp) != 2) {
        perror("fwrite downward stair data");
        fclose(fp);
        return -1;
    }
    
    fclose(fp);
    return 0;
}

/* load_dungeon:
   Loads the dungeon from disk at $HOME/.rlg327/dungeon.
   Reads the file header, version, file size, PC position, hardness matrix,
   room data, and staircase data. After reading, it calls rebuildDungeon() to reconstruct
   the character map.
*/
int load_dungeon(void) {
    const char *home = getenv("HOME");
    if (!home) {
        fprintf(stderr, "Error: HOME not set.\n");
        return -1;
    }
    char file_path[PATH_MAX];
    snprintf(file_path, sizeof(file_path), "%s/.rlg327/dungeon", home);
    
    FILE *fp = fopen(file_path, "rb");
    if (!fp) {
        perror("fopen (load_dungeon)");
        return -1;
    }
    
    /* Read and verify file marker (12 bytes) */
    char marker[13] = {0};
    if (fread(marker, 1, 12, fp) != 12) {
        perror("fread marker");
        fclose(fp);
        return -1;
    }
    if (strncmp(marker, FILE_MARKER, 12) != 0) {
        fprintf(stderr, "Error: incorrect file marker.\n");
        fclose(fp);
        return -1;
    }
    
    /* Read version (4 bytes) */
    uint32_t version;
    if (fread(&version, sizeof(version), 1, fp) != 1) {
        perror("fread version");
        fclose(fp);
        return -1;
    }
    version = ntohl(version);
    if (version != 0) {
        fprintf(stderr, "Error: unsupported file version %u.\n", version);
        fclose(fp);
        return -1;
    }
    
    /* Read file size (4 bytes) */
    uint32_t file_size;
    if (fread(&file_size, sizeof(file_size), 1, fp) != 1) {
        perror("fread file size");
        fclose(fp);
        return -1;
    }
    file_size = ntohl(file_size);
    /* (Optional: verify file_size matches expected value.) */
    
    /* Read PC position (2 bytes) */
    uint8_t pc_pos[2];
    if (fread(pc_pos, 1, 2, fp) != 2) {
        perror("fread PC position");
        fclose(fp);
        return -1;
    }
    pc.col = pc_pos[0];
    pc.row = pc_pos[1];
    
    /* Read hardness matrix (1680 bytes) */
    for (int i = 0; i < ROW; i++) {
        if (fread(hardness[i], 1, COL, fp) != COL) {
            perror("fread hardness row");
            fclose(fp);
            return -1;
        }
    }
    
    /* Read number of rooms (2 bytes) */
    uint16_t num_r;
    if (fread(&num_r, sizeof(num_r), 1, fp) != 1) {
        perror("fread number of rooms");
        fclose(fp);
        return -1;
    }
    num_r = ntohs(num_r);
    num_rooms = num_r;
    if (num_rooms > 0) {
        rooms = malloc(num_rooms * sizeof(room_t));
        if (!rooms) {
            perror("malloc rooms");
            fclose(fp);
            return -1;
        }
    }
    /* Read room data (4 bytes per room) */
    for (int i = 0; i < num_rooms; i++) {
        uint8_t room_data[4];
        if (fread(room_data, 1, 4, fp) != 4) {
            perror("fread room data");
            fclose(fp);
            return -1;
        }
        rooms[i].x = room_data[0];
        rooms[i].y = room_data[1];
        rooms[i].width = room_data[2];
        rooms[i].height = room_data[3];
    }
    
    /* Read number of upward stairs (2 bytes) */
    uint16_t num_up;
    if (fread(&num_up, sizeof(num_up), 1, fp) != 1) {
        perror("fread num upward stairs");
        fclose(fp);
        return -1;
    }
    num_up = ntohs(num_up);
    if (num_up != 1) {
        fprintf(stderr, "Error: expected 1 upward staircase, got %u.\n", num_up);
        fclose(fp);
        return -1;
    }
    /* Read upward staircase position (2 bytes) */
    uint8_t up_data[2];
    if (fread(up_data, 1, 2, fp) != 2) {
        perror("fread upward stair data");
        fclose(fp);
        return -1;
    }
    up_stair.col = up_data[0];
    up_stair.row = up_data[1];
    
    /* Read number of downward stairs (2 bytes) */
    uint16_t num_down;
    if (fread(&num_down, sizeof(num_down), 1, fp) != 1) {
        perror("fread num downward stairs");
        fclose(fp);
        return -1;
    }
    num_down = ntohs(num_down);
    if (num_down != 1) {
        fprintf(stderr, "Error: expected 1 downward staircase, got %u.\n", num_down);
        fclose(fp);
        return -1;
    }
    /* Read downward staircase position (2 bytes) */
    uint8_t down_data[2];
    if (fread(down_data, 1, 2, fp) != 2) {
        perror("fread downward stair data");
        fclose(fp);
        return -1;
    }
    down_stair.col = down_data[0];
    down_stair.row = down_data[1];
    
    fclose(fp);
    
    /* Rebuild the dungeon character map from the loaded data */
    rebuildDungeon();
    
    return 0;
}

/* --- Main --- */
int main(int argc, char *argv[]) {
    int load_flag = 0, save_flag = 0;
    
    /* Process command–line switches */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--load") == 0)
            load_flag = 1;
        if (strcmp(argv[i], "--save") == 0)
            save_flag = 1;
    }
    
    srand((unsigned int)time(NULL));
    
    if (load_flag) {
        if (load_dungeon() != 0) {
            fprintf(stderr, "Error loading dungeon; generating a new one instead.\n");
            generate_dungeon();
        }
    } else {
        generate_dungeon();
    }
    
    display_dungeon();
    
    if (save_flag) {
        if (save_dungeon() != 0) {
            fprintf(stderr, "Error saving dungeon.\n");
        }
    }
    
    /* Free allocated room data */
    free(rooms);
    
    return 0;
}