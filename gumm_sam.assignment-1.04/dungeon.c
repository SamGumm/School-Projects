#include "dungeon.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h> //used to initialize rand seed
#include <limits.h> //used for INT_MAX to initialize INF for dijkstra
#include <stdbool.h> //used for booleans
#include <unistd.h> //used for 'usleep'
#include "heap.h" //profs implementation for heap

/* Global variables for the simulation. */
dungeon_t dungeon;                   // the dungeon
character_t *pc;                     // The player character (PC)
character_t **monsters;              // Array of pointers to monsters
int num_monsters = DEFAULT_NMON;     // Number of monsters
heap_t event_queue;                  // Priority queue for events
long current_turn = 0;               // Global game turn

/* Boolean function to determine whether room coordinates overlap. */
bool rooms_overlap(room_t a, room_t b) {
    if ((a.x - 1) > (b.x + b.width) || (b.x - 1) > (a.x + a.width))
        return false;
    if ((a.y - 1) > (b.y + b.height) || (b.y - 1) > (a.y + a.height))
        return false;
    return true;
}
//can probably replace with pointers to the dungeon
//can also condense and make some helper methods
void carve_corridor(char dungeon[DUNGEON_HEIGHT][DUNGEON_WIDTH],
                    int hardness[DUNGEON_HEIGHT][DUNGEON_WIDTH],
                    int x1, int y1, int x2, int y2) {
    int x, y;
    bool horizontal_first = rand() % 2;
    
    if (horizontal_first) {
        /* Horizontal move */
        if (x1 <= x2) {
            for (x = x1; x <= x2; x++) {
                if (dungeon[y1][x] == ' ') {
                    dungeon[y1][x] = '#';
                    hardness[y1][x] = 0;
                }
            }
        } else {
            for (x = x1; x >= x2; x--) {
                if (dungeon[y1][x] == ' ') {
                    dungeon[y1][x] = '#';
                    hardness[y1][x] = 0;
                }
            }
        }
        /* Vertical move */
        if (y1 <= y2) {
            for (y = y1; y <= y2; y++) {
                if (dungeon[y][x2] == ' ') {
                    dungeon[y][x2] = '#';
                    hardness[y][x2] = 0;
                }
            }
        } else {
            for (y = y1; y >= y2; y--) {
                if (dungeon[y][x2] == ' ') {
                    dungeon[y][x2] = '#';
                    hardness[y][x2] = 0;
                }
            }
        }
    } else {
        /* Vertical move */
        if (y1 <= y2) {
            for (y = y1; y <= y2; y++) {
                if (dungeon[y][x1] == ' ') {
                    dungeon[y][x1] = '#';
                    hardness[y][x1] = 0;
                }
            }
        } else {
            for (y = y1; y >= y2; y--) {
                if (dungeon[y][x1] == ' ') {
                    dungeon[y][x1] = '#';
                    hardness[y][x1] = 0;
                }
            }
        }
        /* Horizontal move */
        if (x1 <= x2) {
            for (x = x1; x <= x2; x++) {
                if (dungeon[y2][x] == ' ') {
                    dungeon[y2][x] = '#';
                    hardness[y2][x] = 0;
                }
            }
        } else {
            for (x = x1; x >= x2; x--) {
                if (dungeon[y2][x] == ' ') {
                    dungeon[y2][x] = '#';
                    hardness[y2][x] = 0;
                }
            }
        }
    }
}
/* Comparison function for cell_t pointers.
    Used by the heap to order cells by distance. */
int cell_compare(const void *a, const void *b) {
    const cell_t *cell1 = a;
    const cell_t *cell2 = b;
    return cell1->distance - cell2->distance;
};
/* Generates a random layout for the dungeon each time the function runs.
    TODO: compartmentalize some of the functions to make it easier to interact with and alter
    Rock is represented by ' '
    Rooms are represented by '.'
*/
void generate_dungeon(dungeon_t *d) {
    int x, y;
    room_t rooms[MAX_ROOMS];
    int num_rooms = 0;
    int i, j;

    for (y = 0; y < DUNGEON_HEIGHT; y++) {
        for (x = 0; x < DUNGEON_WIDTH; x++) {
            if (y == 0 || y == DUNGEON_HEIGHT - 1 || x == 0 || x == DUNGEON_WIDTH - 1) {
                d->map[y][x] = ' ';   /* wall */
                d->hardness[y][x] = 255;
            } else {
                d->map[y][x] = ' ';
                //eventually add veins of minerals somehow
                d->hardness[y][x] = (rand() % 254) + 1; //random hardness
            }
        }
    }

    /* Attempt to place at least MIN_ROOMS rooms.
       Try up to MAX_ROOM_ATTEMPTS times.
       Each room is randomly sized and placed such that it fits inside the borders.
       The room must not contact (or overlap) any existing room (with a 1-cell gap).
    */
    int attempts = 0;
    while (attempts < MAX_ROOM_ATTEMPTS && num_rooms < MIN_ROOMS) {
        int room_width = MIN_ROOM_WIDTH + rand() % (MAX_ROOM_WIDTH - MIN_ROOM_WIDTH + 1);
        int room_height = MIN_ROOM_HEIGHT + rand() % (MAX_ROOM_HEIGHT - MIN_ROOM_HEIGHT + 1);
        int room_x = 1 + rand() % (DUNGEON_WIDTH - room_width - 2);
        int room_y = 1 + rand() % (DUNGEON_HEIGHT - room_height - 2);

        room_t new_room = { room_x, room_y, room_width, room_height };
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
                    d -> map[j][i] = '.';
                    d -> hardness[j][i] = 0;
                }
            }
            rooms[num_rooms++] = new_room;
        }
        attempts++;
    }

    if (num_rooms < MIN_ROOMS) {
        fprintf(stderr, "Error: Only %d rooms could be placed after %d attempts.\n",
                num_rooms, attempts);  
        return;  
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
        carve_corridor(d -> map, d -> hardness,room1_center_x, room1_center_y,
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
        d -> map[up_stair_y][up_stair_x] = '<';
    }
    {
        /* Choose a random room for the down staircase (ensure a different cell) */
        int room_index;
        do {
            room_index = rand() % num_rooms;
            down_stair_x = rooms[room_index].x + rand() % rooms[room_index].width;
            down_stair_y = rooms[room_index].y + rand() % rooms[room_index].height;
        } while (down_stair_x == up_stair_x && down_stair_y == up_stair_y);
            d -> map[down_stair_y][down_stair_x] = '>';
    }
    // /* Add a region with increased hardness for random obstacles */
    // /* Decide how many of these to place, replace numbers with variables */
    // for (y = 5; y < 10; y++) {
    //     for (x = 20; x < 30; x++) {
    //         d->map[y][x] = ' ';
    //         d->hardness[y][x] = 200;
    //     }
    // }
}
/*Prints the dungeon layout to the console. 
    Uses the map field for cell representation 
    and marks the PC with an '@' symbol.*/
void render_dungeon(dungeon_t *d) {
    int x, y;
    for (y = 0; y < DUNGEON_HEIGHT; y++) {
        for (x = 0; x < DUNGEON_WIDTH; x++) {
            bool printed = false;
            // If PC is here, print '@'
            if (x == d->char_pos.x && y == d->char_pos.y) {
                putchar('@');
                printed = true;
            } else {
                // Check if any monster is at (x,y) and alive.
                for (int i = 0; i < num_monsters; i++) {
                    if (monsters[i]->alive &&
                        monsters[i]->pos.x == x && monsters[i]->pos.y == y) {
                        putchar(monsters[i]->symbol);
                        printed = true;
                        break;
                    }
                }
            }
            // If nothing has been printed for this cell, print the dungeon map character.
            if (!printed)
                putchar(d->map[y][x]);
        }
        putchar('\n');
    }
}
/* Renders a distance map.
   For each cell, if reachable (distance != INF), it prints the last digit (mod 10)
   of the computed distance; unreachable cells are printed as a blank space.
   The PC is shown as '@'.
*/
void render_distance_map(dungeon_t *d, int dist[DUNGEON_HEIGHT][DUNGEON_WIDTH]) {
    int x, y;
    for (y = 0; y < DUNGEON_HEIGHT; y++) {
        for (x = 0; x < DUNGEON_WIDTH; x++) {
            if (x == d->char_pos.x && y == d->char_pos.y)
                putchar('@');
            else if (dist[y][x] == INF)
                putchar(' ');
            else
                putchar('0' + (dist[y][x] % 10));
        }
        putchar('\n');
    }
}
/*Returns true if any monster is alive.*/ 
bool monsters_remaining(void) {
    for (int i = 0; i < num_monsters; i++) {
        if (monsters[i]->alive)
            return true;
    }
    return false;
}
/* Places the pc randomly on a tile with hardness 0.*/
void place_pc(dungeon_t *d) {
    int x, y;
    do {
        x = rand() % DUNGEON_WIDTH;
        y = rand() % DUNGEON_HEIGHT;
    } while (d->hardness[y][x] != 0);
    d->char_pos.x = x;
    d->char_pos.y = y;
};
/* PC movement function.
    For now, the PC moves randomly among adjacent cells (including the possibility to remain still).
    If the PC moves into a cell occupied by a monster, that monster dies.*/
void move_pc(character_t *pc) {
    // Compute a random offset in the range -1 to 1 for both directions.
    int dx = (rand() % 3) - 1;
    int dy = (rand() % 3) - 1;
    int new_x = dungeon.char_pos.x + dx;
    int new_y = dungeon.char_pos.y + dy;
    // Check boundaries and ensure PC does not move into an immutable cell (hardness 255).
    if (new_x < 0 || new_x >= DUNGEON_WIDTH || new_y < 0 || new_y >= DUNGEON_HEIGHT ||
        dungeon.hardness[new_y][new_x] == 255) {
        // If invalid move, PC stays in place.
        return;
    }
    // If the PC is currently in a room (floor cell, '.'), only allow moves that remain on floor.
    if (dungeon.map[dungeon.char_pos.y][dungeon.char_pos.x] == '.') {
        if (dungeon.map[new_y][new_x] != '.') {
            return; // Reject move that would leave the room.
        }
    }
    // Collision check: if a monster occupies the target cell, kill that monster.
    for (int i = 0; i < num_monsters; i++) {
        if (monsters[i]->alive &&
            monsters[i]->pos.x == new_x && monsters[i]->pos.y == new_y) {
            // PC kills the monster.
            monsters[i]->alive = false;
            printf("PC kills monster %c at (%d,%d)\n", monsters[i]->symbol, new_x, new_y);
        }
    }
    // Move PC.
    dungeon.char_pos.x = new_x;
    dungeon.char_pos.y = new_y;
    pc->pos.x = new_x;
    pc->pos.y = new_y;
}
/* Frees the memory allocated to a cell.*/
void cell_datum_delete(void *v) {
    free(v); //free mem allocated to passed cell
};
/* Dijkstra's algorithm to compute distance maps.
   When tunnel is 0, only floor cells (hardness 0) are traversable (cost = 1).
   When tunnel is nonzero, cells with hardness 1–254 are allowed with a cost of 1 + (hardness/85),
   and cells with hardness 255 are impassable.
   The resulting distances are stored in the 2D array dist.
*/
void dijkstra(dungeon_t *d, int tunnel, int dist[DUNGEON_HEIGHT][DUNGEON_WIDTH]) {
    int i, j;
    /* Initialize distances to "infinity" */
    for (i = 0; i < DUNGEON_HEIGHT; i++) {
        for (j = 0; j < DUNGEON_WIDTH; j++) {
            dist[i][j] = INF;
        }
    }
    dist[d->char_pos.y][d->char_pos.x] = 0;

    heap_t h;
    heap_init(&h, cell_compare, cell_datum_delete);

    /* Insert the PC's cell as the starting point */
    cell_t *start = malloc(sizeof(cell_t));
    start -> position.x = d->char_pos.x;
    start -> position.y = d->char_pos.y;
    start -> distance = 0;
    heap_insert(&h, start);

    /* 8-way connectivity */
    int directions[8][2] = {
        {-1, -1}, {0, -1}, {1, -1},
        {-1,  0},          {1,  0},
        {-1,  1}, {0,  1}, {1,  1}
    };

    while (h.size > 0) {  /* Continue while there are nodes in the heap */
        cell_t *curr = heap_remove_min(&h);
        int x = curr-> position.x;
        int y = curr-> position.y;
        int curr_dist = curr->distance;
        free(curr);

        /* Examine all neighbors */
        for (int dir = 0; dir < 8; dir++) {
            int nx = x + directions[dir][0];
            int ny = y + directions[dir][1];

            if (nx < 0 || nx >= DUNGEON_WIDTH || ny < 0 || ny >= DUNGEON_HEIGHT)
                continue;

            int weight;
            if (!tunnel) {
                /* For non-tunnelers: only traverse if hardness is 0 */
                if (d->hardness[ny][nx] != 0)
                    continue;
                weight = 1;
            } else {
                /* For tunneling monsters: skip cells with hardness 255 */
                if (d->hardness[ny][nx] == 255)
                    continue;
                if (d->hardness[ny][nx] == 0)
                    weight = 1;
                else
                    weight = 1 + (d->hardness[ny][nx] / 85);
            }

            int new_dist = curr_dist + weight;
            if (new_dist < dist[ny][nx]) {
                dist[ny][nx] = new_dist;
                cell_t *neighbor = malloc(sizeof(cell_t));
                neighbor-> position.x = nx;
                neighbor-> position.y = ny;
                neighbor->distance = new_dist;
                heap_insert(&h, neighbor);
            }
        }
    }
    heap_delete(&h);
}
/*Checks if a cell is occupied by the PC or any monster.*/
bool is_cell_occupied(dungeon_t *d, character_t **monsters, int n_monsters, int x, int y) {
    // Check PC's position.
    if (d->char_pos.x == x && d->char_pos.y == y)
        return true;
    // Check each monster.
    for (int i = 0; i < n_monsters; i++) {
        if (monsters[i] && monsters[i]->alive &&
            monsters[i]->pos.x == x && monsters[i]->pos.y == y)
            return true;
    }
    return false;
}
int event_compare(const void *a, const void *b) {
    const event_t *event_a = a;
    const event_t *event_b = b;
    if (event_a->time < event_b->time)
        return -1;
    else if (event_a->time > event_b->time)
        return 1;
    return 0;
}
// Get a random open cell (hardness == 0) that isn’t already occupied.
void get_random_open_cell(dungeon_t *d, character_t **monsters, int n_monsters, int *x, int *y) {
    do {
        *x = rand() % DUNGEON_WIDTH;
        *y = rand() % DUNGEON_HEIGHT;
    } while (d->hardness[*y][*x] != 0 || is_cell_occupied(d, monsters, n_monsters, *x, *y));
}
character_t ** generate_monsters(dungeon_t *d, int n_monsters) {
    character_t **monsters = malloc(n_monsters * sizeof(character_t *));
    if (!monsters) {
        perror("malloc for monsters array failed");
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < n_monsters; i++) {
        monsters[i] = malloc(sizeof(character_t));
        if (!monsters[i]) {
            perror("malloc for monster failed");
            exit(EXIT_FAILURE);
        }
        
        monsters[i]->alive = true;
        monsters[i]->speed = 5 + rand() % 16;  // Speed between 5 and 20.
        
        int rand_val = rand() % 16;
        monsters[i]->c.intelligence = (rand_val & 1);
        monsters[i]->c.telepathy    = ((rand_val >> 1) & 1);
        monsters[i]->c.tunneling    = ((rand_val >> 2) & 1);
        monsters[i]->c.erratic      = ((rand_val >> 3) & 1);
        
        if (rand_val < 10) {
            monsters[i]->symbol = '0' + rand_val;
        } else {
            monsters[i]->symbol = 'a' + (rand_val - 10);
        }
        
        int x, y;
        get_random_open_cell(d, monsters, n_monsters, &x, &y);
        monsters[i]->pos.x = x;
        monsters[i]->pos.y = y;
        
        schedule_monster_move(monsters[i]);
    }
    
    return monsters;
}

/* Monster movement function.
    Each monster moves one step toward the PC,
    unless its erratic flag triggers a random move.*/
void move_monster(character_t *monster) {
    if (!monster->alive)
        return;
    int dx = 0, dy = 0;
    // If erratic behavior triggers (50% chance), move randomly.
    if (monster->c.erratic && (rand() % 2 == 0)) {
        dx = (rand() % 3) - 1;
        dy = (rand() % 3) - 1;
    } else {
        // Otherwise, move one step toward the PC.
        if (pc->pos.x > monster->pos.x) dx = 1;
        else if (pc->pos.x < monster->pos.x) dx = -1;
        if (pc->pos.y > monster->pos.y) dy = 1;
        else if (pc->pos.y < monster->pos.y) dy = -1;
    }
    int new_x = monster->pos.x + dx;
    int new_y = monster->pos.y + dy;
    // Check boundaries and ensure not moving into immutable cells.
    if (new_x < 0 || new_x >= DUNGEON_WIDTH || new_y < 0 || new_y >= DUNGEON_HEIGHT ||
        dungeon.hardness[new_y][new_x] == 255) {
        return; // Monster stays in place.
    }
    // Collision check: if moving into PC's cell, PC dies.
    if (new_x == pc->pos.x && new_y == pc->pos.y) {
        pc->alive = false;
        printf("Monster %c kills the PC at (%d,%d)!\n", monster->symbol, new_x, new_y);
        return;
    }
    // If moving into a cell occupied by another monster, kill that monster.
    for (int i = 0; i < num_monsters; i++) {
        if (monsters[i]->alive &&
            monsters[i]->pos.x == new_x && monsters[i]->pos.y == new_y) {
            // The moving monster kills the occupant.
            monsters[i]->alive = false;
            printf("Monster %c kills monster %c at (%d,%d)\n", monster->symbol, monsters[i]->symbol, new_x, new_y);
        }
    }
    // Update monster position.
    monster->pos.x = new_x;
    monster->pos.y = new_y;
}

/*Event creation and scheduling*/
event_t *create_event(character_t *ch, long time) {
    event_t *e = malloc(sizeof(event_t));
    if (!e) {
        perror("malloc for event failed");
        exit(EXIT_FAILURE);
    }
    e->ch = ch;
    e->time = time;
    return e;
}
// This function computes the event time for a monster based on its speed and current turn,
// creates an event, and inserts it into the global event_queue.
void schedule_monster_move(character_t *monster) {
    if (!monster->alive)
        return;
    event_t *e = create_event(monster, current_turn + (1000 / monster->speed));
    heap_insert(&event_queue, e);
}
// Schedule the PC's move event.
void schedule_pc_move(character_t *pc) {
    event_t *e = create_event(pc, current_turn + (1000 / pc->speed));
    heap_insert(&event_queue, e);
}

// This loop repeatedly processes the next event and reschedules it until the game is over.
void game_loop(void) {
    while (pc->alive && monsters_remaining()) {
        event_t *e = heap_remove_min(&event_queue);
        current_turn = e->time;
        if (e->ch == pc) {
            move_pc(pc);
            render_dungeon(&dungeon);
            usleep(300000); // Pause for 300ms to observe the move. 
            //(did you know it takes roughly 240ms for a human to gain minimal comprehension of what they are looking at? somewhere around there at least)
        } else {
            move_monster(e->ch);
        }
        // Reschedule event if character is still alive.
        if (e->ch->alive) {
            e->time = current_turn + (1000 / e->ch->speed);
            heap_insert(&event_queue, e);
        } else {
            free(e);
        }
    }
    if (!pc->alive)
        printf("You lose!\n");
    else
        printf("You win!\n");
}

// --- Cleanup ---
// Free memory for PC, monsters, and event queue.
void cleanup(void) {
    free(pc);
    for (int i = 0; i < num_monsters; i++) {
        free(monsters[i]);
    }
    free(monsters);
    heap_delete(&event_queue);
}

int main(int argc, char *argv[]) {
    // If an argument is provided, use it as the number of monsters.
    if (argc > 1) {
        int temp = atoi(argv[1]);
        if (temp > 0)
            num_monsters = temp;
    }
    
    system("clear");
    srand(time(NULL));
    
    // Initialize the event queue.
    heap_init(&event_queue, event_compare, free);
    
    // Generate the dungeon.
    generate_dungeon(&dungeon);
    
    // Create the PC.
    pc = malloc(sizeof(character_t));
    if (!pc) {
        perror("malloc for PC failed");
        exit(EXIT_FAILURE);
    }
    pc->alive = true;
    pc->speed = PC_SPEED;
    pc->symbol = '@';
    
    // Place the PC on a valid cell.
    place_pc(&dungeon);
    pc->pos.x = dungeon.char_pos.x;
    pc->pos.y = dungeon.char_pos.y;
    
    // Compute distance maps.
    int non_tunnel_dist[DUNGEON_HEIGHT][DUNGEON_WIDTH];
    int tunnel_dist[DUNGEON_HEIGHT][DUNGEON_WIDTH];
    dijkstra(&dungeon, 0, non_tunnel_dist);
    dijkstra(&dungeon, 1, tunnel_dist);
    
    // Generate monsters.
    monsters = generate_monsters(&dungeon, num_monsters);
    
    // Schedule initial events for PC and monsters.
    schedule_pc_move(pc);
    for (int i = 0; i < num_monsters; i++) {
        schedule_monster_move(monsters[i]);
    }
    
    // Render the initial dungeon.
    render_dungeon(&dungeon);
    
    // Enter the game loop.
    game_loop();
    
    // Cleanup allocated memory.
    cleanup();
    return 0;
}

