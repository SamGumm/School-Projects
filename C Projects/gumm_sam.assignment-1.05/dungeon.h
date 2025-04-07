#ifndef DUNGEON_H
#define DUNGEON_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h> //used to initialize rand seed
#include <limits.h> //used for INT_MAX to initialize INF for dijkstra
#include <stdbool.h> //used for booleans
#include <unistd.h> //used for 'usleep'
#include "heap.h" //profs implementation for heap

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

// Type definitions
typedef enum cell_type {
    ROCK,
    CORRIDOR,
    ROOM,
    IMMUTABLE
} cell_type_t;

typedef struct position {
    int x;
    int y;
} position_t;

typedef struct cell {
    cell_type_t type;
    int hardness;
    int distance;
    position_t position;
} cell_t;

typedef struct characteristics {
    unsigned int intelligence : 1;
    unsigned int telepathy : 1;
    unsigned int tunneling : 1;
    unsigned int erratic : 1;
} characteristics_t;

typedef struct character {
    position_t pos;
    int speed;
    char symbol;
    characteristics_t c;
    bool alive;
} character_t;

typedef struct event {
    long time;
    character_t *ch;
} event_t;

typedef struct dungeon {
    int hardness[DUNGEON_HEIGHT][DUNGEON_WIDTH];
    char map[DUNGEON_HEIGHT][DUNGEON_WIDTH];
    struct {
        int x, y;
    } char_pos;
} dungeon_t;

typedef struct Room {
    int x;
    int y;
    int width;
    int height;
} room_t;

// Function prototypes

// Dungeon layout and rendering functions
bool rooms_overlap(room_t a, room_t b);
void carve_corridor(char dungeon[DUNGEON_HEIGHT][DUNGEON_WIDTH],
                    int hardness[DUNGEON_HEIGHT][DUNGEON_WIDTH],
                    int x1, int y1, int x2, int y2);
int cell_compare(const void *a, const void *b);
void generate_dungeon(dungeon_t *d);
void render_dungeon(dungeon_t *d);
void render_distance_map(dungeon_t *d, int dist[DUNGEON_HEIGHT][DUNGEON_WIDTH]);
bool monsters_remaining(void);
void place_pc(dungeon_t *d);
void move_pc(character_t *pc);
void cell_datum_delete(void *v);
void dijkstra(dungeon_t *d, int tunnel, int dist[DUNGEON_HEIGHT][DUNGEON_WIDTH]);

// Monster and event functions
bool is_cell_occupied(dungeon_t *d, character_t **monsters, int n_monsters, int x, int y);
int event_compare(const void *a, const void *b);
void get_random_open_cell(dungeon_t *d, character_t **monsters, int n_monsters, int *x, int *y);
character_t **generate_monsters(dungeon_t *d, int n_monsters);
character_t ** generate_monsters(dungeon_t *d, int n_monsters);
void move_monster(character_t *monster);

// Discrete event simulation functions
void init_events(void);
void game_loop(void);
event_t *create_event(character_t *ch, long time);
void schedule_monster_move(character_t *monster);
void schedule_pc_move(character_t *pc);
void move_monster(character_t *monster);
void cleanup(void);

//to be classified *later*
void render_dungeon_ncurses(dungeon_t *d);
void generate_new_level(void);
void process_pc_command(int command);

#endif // DUNGEON_H
