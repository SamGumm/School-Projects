#include "dungeon.h"
#include <cstdlib>      // for rand, srand, atoi
#include <cstdio>       // for printf, perror, fprintf
#include <ctime>        // for time()
#include <climits>      // for INT_MAX (used for INF)
#include <unistd.h>     // for usleep
#include "heap.h"       // professor’s heap implementation (used in dijkstra)
#include <ncurses.h>    // for ncurses interface
#include <algorithm>    // for std::max

/* Global variables for the simulation. */
dungeon_t dungeon;                   // the dungeon
character_t *pc;                     // the player character (PC)
character_t **monsters;              // array of pointers to monsters
int num_monsters = DEFAULT_NMON;     // number of monsters

// Forward declarations for utility functions.
void generate_new_level(void);
void render_dungeon_ncurses(const dungeon_t *d, const character_t *pc,
                            const character_t **monsters, int num_monsters);
void process_teleport_mode(void);
void update_memory(character_t *pc, const dungeon_t *d);


// Returns true if room 'a' and room 'b' overlap (with a 1-cell gap).
bool rooms_overlap(room_t a, room_t b) {
    if ((a.x - 1) > (b.x + b.width) || (b.x - 1) > (a.x + a.width))
        return false;
    if ((a.y - 1) > (b.y + b.height) || (b.y - 1) > (a.y + a.height))
        return false;
    return true;
}

/* carve_corridor: Carves an L-shaped corridor between (x1,y1) and (x2,y2).
*/
void carve_corridor(char dungeon[DUNGEON_HEIGHT][DUNGEON_WIDTH],
                    int hardness[DUNGEON_HEIGHT][DUNGEON_WIDTH],
                    int x1, int y1, int x2, int y2) {
    int x, y;
    bool horizontal_first = rand() % 2;
    
    if (horizontal_first) {
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

/* cell_compare: Comparison function for cell_t pointers used in dijkstra.
*/
int cell_compare(const void *a, const void *b) {
    const cell_t *cell1 = (const cell_t *)a;
    const cell_t *cell2 = (const cell_t *)b;
    return cell1->distance - cell2->distance;
}

/* generate_dungeon: Generates a random dungeon layout.
*/
void generate_dungeon(dungeon_t *d) {
    int x, y;
    room_t rooms[MAX_ROOMS];
    int num_rooms = 0;
    int i, j;

    for (y = 0; y < DUNGEON_HEIGHT; y++) {
        for (x = 0; x < DUNGEON_WIDTH; x++) {
            if (y == 0 || y == DUNGEON_HEIGHT - 1 || x == 0 || x == DUNGEON_WIDTH - 1) {
                d->map[y][x] = ' ';
                d->hardness[y][x] = 255;
            } else {
                d->map[y][x] = ' ';
                d->hardness[y][x] = (rand() % 254) + 1;
            }
        }
    }

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
            for (j = new_room.y; j < new_room.y + new_room.height; j++) {
                for (i = new_room.x; i < new_room.x + new_room.width; i++) {
                    d->map[j][i] = '.';
                    d->hardness[j][i] = 0;
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

    for (int r = 1; r < num_rooms; r++) {
        int room1_center_x = rooms[r].x + rooms[r].width / 2;
        int room1_center_y = rooms[r].y + rooms[r].height / 2;
        int closest_room_index = 0;
        int min_dist_sq = INT_MAX;
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
        carve_corridor(d->map, d->hardness, room1_center_x, room1_center_y,
                       room2_center_x, room2_center_y);
    }

    // Place stairs.
    int up_stair_x, up_stair_y, down_stair_x, down_stair_y;
    {
        int room_index = rand() % num_rooms;
        up_stair_x = rooms[room_index].x + rand() % rooms[room_index].width;
        up_stair_y = rooms[room_index].y + rand() % rooms[room_index].height;
        d->map[up_stair_y][up_stair_x] = '<';
    }
    {
        int room_index;
        do {
            room_index = rand() % num_rooms;
            down_stair_x = rooms[room_index].x + rand() % rooms[room_index].width;
            down_stair_y = rooms[room_index].y + rand() % rooms[room_index].height;
        } while (down_stair_x == up_stair_x && down_stair_y == up_stair_y);
        d->map[down_stair_y][down_stair_x] = '>';
    }
}

/* render_distance_map: */
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

/* monsters_remaining: Returns true if any monster is alive.
 */
bool monsters_remaining(void) {
    for (int i = 0; i < num_monsters; i++) {
        if (monsters[i]->alive)
            return true;
    }
    return false;
}

/* place_pc: Places the PC randomly on a cell with hardness 0.
*/
void place_pc(dungeon_t *d) {
    int x, y;
    do {
        x = rand() % DUNGEON_WIDTH;
        y = rand() % DUNGEON_HEIGHT;
    } while (d->hardness[y][x] != 0);
    d->char_pos.x = x;
    d->char_pos.y = y;
}

/* move_pc: Moves the PC randomly among adjacent cells. */
void move_pc(character_t *pc) {
    int dx = (rand() % 3) - 1;
    int dy = (rand() % 3) - 1;
    int new_x = dungeon.char_pos.x + dx;
    int new_y = dungeon.char_pos.y + dy;
    if (new_x < 0 || new_x >= DUNGEON_WIDTH || new_y < 0 || new_y >= DUNGEON_HEIGHT ||
        dungeon.hardness[new_y][new_x] == 255) {
        return;
    }
    if (dungeon.map[dungeon.char_pos.y][dungeon.char_pos.x] == '.') {
        if (dungeon.map[new_y][new_x] != '.')
            return;
    }
    for (int i = 0; i < num_monsters; i++) {
        if (monsters[i]->alive &&
            monsters[i]->pos.x == new_x && monsters[i]->pos.y == new_y) {
            monsters[i]->alive = false;
        }
    }
    dungeon.char_pos.x = new_x;
    dungeon.char_pos.y = new_y;
    pc->pos.x = new_x;
    pc->pos.y = new_y;
}

/* dijkstra: Computes distance maps using Dijkstra's algorithm.
*/
void dijkstra(dungeon_t *d, int tunnel, int dist[DUNGEON_HEIGHT][DUNGEON_WIDTH]) {
    int i, j;
    for (i = 0; i < DUNGEON_HEIGHT; i++) {
        for (j = 0; j < DUNGEON_WIDTH; j++) {
            dist[i][j] = INF;
        }
    }
    dist[d->char_pos.y][d->char_pos.x] = 0;

    heap_t h;
    heap_init(&h, cell_compare, NULL);

    cell_t *start = new cell_t;
    start->position.x = d->char_pos.x;
    start->position.y = d->char_pos.y;
    start->distance = 0;
    heap_insert(&h, start);

    int directions[8][2] = {
        {-1, -1}, {0, -1}, {1, -1},
        {-1,  0},          {1,  0},
        {-1,  1}, {0,  1}, {1,  1}
    };

    while (h.size > 0) {
        cell_t *curr = static_cast<cell_t*>(heap_remove_min(&h));
        int x = curr->position.x;
        int y = curr->position.y;
        int curr_dist = curr->distance;
        delete curr;

        for (int dir = 0; dir < 8; dir++) {
            int nx = x + directions[dir][0];
            int ny = y + directions[dir][1];

            if (nx < 0 || nx >= DUNGEON_WIDTH || ny < 0 || ny >= DUNGEON_HEIGHT)
                continue;

            int weight;
            if (!tunnel) {
                if (d->hardness[ny][nx] != 0)
                    continue;
                weight = 1;
            } else {
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
                cell_t *neighbor = new cell_t;
                neighbor->position.x = nx;
                neighbor->position.y = ny;
                neighbor->distance = new_dist;
                heap_insert(&h, neighbor);
            }
        }
    }
    heap_delete(&h);
}

/* is_cell_occupied: Checks if the given cell is occupied by the PC or any monster.
*/
bool is_cell_occupied(dungeon_t *d, character_t **monsters, int n_monsters, int x, int y) {
    if (d->char_pos.x == x && d->char_pos.y == y)
        return true;
    for (int i = 0; i < n_monsters; i++) {
        if (monsters[i] && monsters[i]->alive &&
            monsters[i]->pos.x == x && monsters[i]->pos.y == y)
            return true;
    }
    return false;
}

/* get_random_open_cell: Finds a random open cell (hardness == 0) that isn't occupied.
*/
void get_random_open_cell(dungeon_t *d, character_t **monsters, int n_monsters, int *x, int *y) {
    do {
        *x = rand() % DUNGEON_WIDTH;
        *y = rand() % DUNGEON_HEIGHT;
    } while (d->hardness[*y][*x] != 0 || is_cell_occupied(d, monsters, n_monsters, *x, *y));
}

/* generate_monsters: Generates an array of monster pointers.
*/
character_t ** generate_monsters(dungeon_t *d, int n_monsters) {
    character_t **mons = new character_t*[n_monsters];
    if (!mons) {
        perror("new for monsters array failed");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n_monsters; i++) {
        mons[i] = new character_t;
        if (!mons[i]) {
            perror("new for monster failed");
            exit(EXIT_FAILURE);
        }
        mons[i]->alive = true;
        mons[i]->speed = 5 + rand() % 16;
        int rand_val = rand() % 16;
        mons[i]->c.intelligence = (rand_val & 1);
        mons[i]->c.telepathy    = ((rand_val >> 1) & 1);
        mons[i]->c.tunneling    = ((rand_val >> 2) & 1);
        mons[i]->c.erratic      = ((rand_val >> 3) & 1);
        if (rand_val < 10)
            mons[i]->symbol = '0' + rand_val;
        else
            mons[i]->symbol = 'a' + (rand_val - 10);
        int x, y;
        get_random_open_cell(d, mons, n_monsters, &x, &y);
        mons[i]->pos.x = x;
        mons[i]->pos.y = y;
        // In the event-queue version, schedule the monster move here.
    }
    return mons;
}

/* move_monster: Moves a monster one step toward the PC (or randomly if erratic).
*/
void move_monster(character_t *monster) {
    if (!monster->alive)
        return;
    int dx = 0, dy = 0;
    if (monster->c.erratic && (rand() % 2 == 0)) {
        dx = (rand() % 3) - 1;
        dy = (rand() % 3) - 1;
    } else {
        if (pc->pos.x > monster->pos.x) dx = 1;
        else if (pc->pos.x < monster->pos.x) dx = -1;
        if (pc->pos.y > monster->pos.y) dy = 1;
        else if (pc->pos.y < monster->pos.y) dy = -1;
    }
    int new_x = monster->pos.x + dx;
    int new_y = monster->pos.y + dy;
    if (new_x < 0 || new_x >= DUNGEON_WIDTH || new_y < 0 || new_y >= DUNGEON_HEIGHT ||
        dungeon.hardness[new_y][new_x] == 255)
        return;
    if (new_x == pc->pos.x && new_y == pc->pos.y) {
        pc->alive = false;
        return;
    }
    for (int i = 0; i < num_monsters; i++) {
        if (monsters[i]->alive &&
            monsters[i]->pos.x == new_x && monsters[i]->pos.y == new_y) {
            monsters[i]->alive = false;
        }
    }
    monster->pos.x = new_x;
    monster->pos.y = new_y;
}

/* process_pc_command: Processes keyboard input for the PC.
*/
void process_pc_command(int command) {
    int new_x = pc->pos.x, new_y = pc->pos.y;
    move(0, 0);
    clrtoeol();
    switch (command) {
        case 'f': {
            // Toggle fog-of-war
            pc->fullVisibility = !pc->fullVisibility;
            if (pc->fullVisibility)
                mvprintw(0, 0, "Fog of war disabled (full map visible).");
            else
                mvprintw(0, 0, "Fog of war enabled.");
            break;
        }
        case 'g': {
            // Enter teleport mode.
            process_teleport_mode();
            break;
        }
        case '7': case 'y':
            new_x = pc->pos.x - 1; new_y = pc->pos.y - 1;
            break;
        case '8': case 'k':
            new_y = pc->pos.y - 1;
            break;
        case '9': case 'u':
            new_x = pc->pos.x + 1; new_y = pc->pos.y - 1;
            break;
        case '4': case 'h':
            new_x = pc->pos.x - 1;
            break;
        case '6': case 'l':
            new_x = pc->pos.x + 1;
            break;
        case '1': case 'b':
            new_x = pc->pos.x - 1; new_y = pc->pos.y + 1;
            break;
        case '2': case 'j':
            new_y = pc->pos.y + 1;
            break;
        case '3': case 'n':
            new_x = pc->pos.x + 1; new_y = pc->pos.y + 1;
            break;
        case '.':
            if (dungeon.map[pc->pos.y][pc->pos.x] == '>') {
                mvprintw(0, 0, "Going down stairs...");
                refresh();
                generate_new_level();
            } else {
                mvprintw(0, 0, "There is no downward staircase here.");
            }
            return;
        case ',':
            if (dungeon.map[pc->pos.y][pc->pos.x] == '<') {
                mvprintw(0, 0, "Going up stairs...");
                refresh();
                generate_new_level();
            } else {
                mvprintw(0, 0, "There is no upward staircase here.");
            }
            return;
        case '5': case ' ':
            return;
        case 'q':
            pc->alive = false;
            return;
        default:
            mvprintw(0, 0, "Unrecognized command.");
            return;
    }
    if (new_x < 0 || new_x >= DUNGEON_WIDTH || new_y < 0 || new_y >= DUNGEON_HEIGHT) {
        mvprintw(0, 0, "Cannot move outside the dungeon boundaries.");
        return;
    }
    if (dungeon.hardness[new_y][new_x] != 0) {
        mvprintw(0, 0, "A wall blocks your way!");
        return;
    }
    for (int i = 0; i < num_monsters; i++) {
        if (monsters[i]->alive &&
            monsters[i]->pos.x == new_x && monsters[i]->pos.y == new_y) {
            monsters[i]->alive = false;
            mvprintw(0, 0, "You killed monster %c!", monsters[i]->symbol);
            break;
        }
    }
    pc->pos.x = new_x;
    pc->pos.y = new_y;
    dungeon.char_pos.x = new_x;
    dungeon.char_pos.y = new_y;
}

/* process_teleport_mode:
   When the PC presses 'g', enter teleport mode.
   The movement keys (same as for moving the PC) now move a targeting pointer (displayed as '*').
   While in teleport mode, pressing:
     - g will teleport the PC to the target location (if the cell is not immutable rock).
     - r will teleport the PC to a random valid location.
*/
void process_teleport_mode(void) {
    int target_x = pc->pos.x;
    int target_y = pc->pos.y;
    bool teleporting = true;
    mvprintw(0, 0, "Teleport mode: use movement keys to target, 'g' to confirm, 'r' for random.");
    while (teleporting) {
        // Render dungeon normally
        render_dungeon_ncurses(&dungeon, pc, (const character_t **)monsters, num_monsters);
        // Overlay the targeting pointer:
        mvaddch(target_y + 1, target_x, '*');
        refresh();
        int key = getch();
        switch (key) {
            case '7': case 'y':
                if (target_x > 0 && target_y > 0) { target_x--; target_y--; }
                break;
            case '8': case 'k':
                if (target_y > 0) target_y--;
                break;
            case '9': case 'u':
                if (target_y > 0 && target_x < DUNGEON_WIDTH - 1) { target_x++; target_y--; }
                break;
            case '4': case 'h':
                if (target_x > 0) target_x--;
                break;
            case '6': case 'l':
                if (target_x < DUNGEON_WIDTH - 1) target_x++;
                break;
            case '1': case 'b':
                if (target_x > 0 && target_y < DUNGEON_HEIGHT - 1) { target_x--; target_y++; }
                break;
            case '2': case 'j':
                if (target_y < DUNGEON_HEIGHT - 1) target_y++;
                break;
            case '3': case 'n':
                if (target_x < DUNGEON_WIDTH - 1 && target_y < DUNGEON_HEIGHT - 1) { target_x++; target_y++; }
                break;
            case 'g': {
                // Attempt to teleport to the targeted location.
                if (dungeon.hardness[target_y][target_x] == 255) {
                    mvprintw(0, 0, "Cannot teleport into immutable rock. Choose another target.");
                    refresh();
                } else {
                    pc->pos.x = target_x;
                    pc->pos.y = target_y;
                    dungeon.char_pos.x = target_x;
                    dungeon.char_pos.y = target_y;
                    teleporting = false;
                }
                break;
            }
            case 'r': {
                // Teleport randomly (allowed into rock except immutable rock)
                int rx, ry;
                do {
                    rx = rand() % DUNGEON_WIDTH;
                    ry = rand() % DUNGEON_HEIGHT;
                } while(dungeon.hardness[ry][rx] == 255);
                pc->pos.x = rx;
                pc->pos.y = ry;
                dungeon.char_pos.x = rx;
                dungeon.char_pos.y = ry;
                teleporting = false;
                break;
            }
            default:
                // Ignore other keys.
                break;
        }
    }
    update_memory(pc, &dungeon);
}

/* round_robin_game_loop: Implements a round-robin loop over the PC and monsters.
*/
void round_robin_game_loop(void) {
    int total_characters = 1 + num_monsters;
    character_t **turn_order = new character_t*[total_characters];
    if (!turn_order) {
        perror("new for turn_order failed");
        exit(EXIT_FAILURE);
    }
    turn_order[0] = pc;
    for (int i = 0; i < num_monsters; i++) {
        turn_order[i + 1] = monsters[i];
    }
    
    int index = 0;
    while (pc->alive && monsters_remaining()) {
        character_t *current = turn_order[index];
        if (!current->alive) {
            index = (index + 1) % total_characters;
            continue;
        }
        if (current == pc) {
            int command = getch();
            process_pc_command(command);
        } else {
            move_monster(current);
        }
        render_dungeon_ncurses(&dungeon, pc, (const character_t **)monsters, num_monsters);
        index = (index + 1) % total_characters;
    }
    delete[] turn_order;
    
    if (!pc->alive)
        mvprintw(0, 0, "You lose!");
    else
        mvprintw(0, 0, "You win!");
    refresh();
    getch();
}

/* generate_new_level: Simulates moving to a new floor.
*/
void generate_new_level(void) {
    if (monsters) {
        for (int i = 0; i < num_monsters; i++) {
            delete monsters[i];
        }
        delete[] monsters;
    }
    
    generate_dungeon(&dungeon);
    place_pc(&dungeon);
    pc->pos.x = dungeon.char_pos.x;
    pc->pos.y = dungeon.char_pos.y;
    
    monsters = generate_monsters(&dungeon, num_monsters);
    
    render_dungeon_ncurses(&dungeon, pc, (const character_t **)monsters, num_monsters);
    
    mvprintw(0, 0, "A new floor appears!");
    refresh();
}

/* cleanup: Frees memory for the PC, monsters, etc.
*/
void cleanup(void) {
    delete pc;
    for (int i = 0; i < num_monsters; i++) {
        delete monsters[i];
    }
    delete[] monsters;
}

/* update_memory: Updates the PC’s remembered dungeon terrain based on its light radius.
*/
void update_memory(character_t *pc, const dungeon_t *d) {
    for (int y = 0; y < DUNGEON_HEIGHT; y++) {
        for (int x = 0; x < DUNGEON_WIDTH; x++) {
            int dx = x - pc->pos.x;
            int dy = y - pc->pos.y;
            if (std::max(std::abs(dx), std::abs(dy)) <= 2) { // light radius of 3 cells
                pc->memory[y][x] = d->map[y][x];
            }
        }
    }
}

/* render_dungeon_ncurses: Renders the dungeon using fog-of-war.
   When fullVisibility is off, only the 5×5 region (centered on the PC) is shown;
   outside this, the PC’s remembered terrain is used (or a space if never seen).
*/
void render_dungeon_ncurses(const dungeon_t *d, const character_t *pc,
                            const character_t **monsters, int num_monsters) {
    // First update the PC's memory.
    update_memory((character_t *)pc, d);
    for (int y = 0; y < DUNGEON_HEIGHT; y++) {
        for (int x = 0; x < DUNGEON_WIDTH; x++) {
            char ch;
            if (pc->fullVisibility) {
                ch = d->map[y][x];
            } else {
                int dx = x - pc->pos.x;
                int dy = y - pc->pos.y;
                if (std::max(std::abs(dx), std::abs(dy)) <= 2)
                    ch = d->map[y][x];
                else
                    ch = pc->memory[y][x];
            }
            // Always overlay the PC.
            if (x == pc->pos.x && y == pc->pos.y)
                ch = '@';
            // Display monsters only if in the illuminated area (or if fullVisibility is on).
            if (!pc->fullVisibility) {
                for (int i = 0; i < num_monsters; i++) {
                    if (monsters[i]->alive) {
                        int mdx = monsters[i]->pos.x - pc->pos.x;
                        int mdy = monsters[i]->pos.y - pc->pos.y;
                        if (std::max(std::abs(mdx), std::abs(mdy)) <= 2 &&
                            monsters[i]->pos.x == x && monsters[i]->pos.y == y) {
                            ch = monsters[i]->symbol;
                            break;
                        }
                    }
                }
            } else {
                for (int i = 0; i < num_monsters; i++) {
                    if (monsters[i]->alive &&
                        monsters[i]->pos.x == x && monsters[i]->pos.y == y) {
                        ch = monsters[i]->symbol;
                        break;
                    }
                }
            }
            mvaddch(y + 1, x, ch);
        }
    }
    refresh();
}

/* MAIN */
int main(int argc, char *argv[]) {
    initscr();            // initialize screen
    cbreak();             // disable line buffering
    noecho();             // do not echo keys
    keypad(stdscr, TRUE); // enable function and arrow keys

    if (has_colors()) { //stub for eventual use
        start_color();
    }
    if (argc > 1) {
        int temp = atoi(argv[1]);
        if (temp > 0)
            num_monsters = temp;
    }
    
    srand(time(NULL));
    
    generate_dungeon(&dungeon);
    
    // Create the PC.
    pc = new character_t;
    if (!pc) {
        perror("new for PC failed");
        exit(EXIT_FAILURE);
    }
    pc->alive = true;
    pc->speed = PC_SPEED;
    pc->symbol = '@';
    pc->fullVisibility = false;  // start with fog-of-war enabled
    // (Initialize the PC's memory to spaces)
    for (int y = 0; y < DUNGEON_HEIGHT; y++) {
        for (int x = 0; x < DUNGEON_WIDTH; x++) {
            pc->memory[y][x] = ' ';
        }
    }
    
    place_pc(&dungeon);
    pc->pos.x = dungeon.char_pos.x;
    pc->pos.y = dungeon.char_pos.y;
    
    int non_tunnel_dist[DUNGEON_HEIGHT][DUNGEON_WIDTH];
    int tunnel_dist[DUNGEON_HEIGHT][DUNGEON_WIDTH];
    dijkstra(&dungeon, 0, non_tunnel_dist);
    dijkstra(&dungeon, 1, tunnel_dist);
    
    monsters = generate_monsters(&dungeon, num_monsters);
    
    render_dungeon_ncurses(&dungeon, pc, (const character_t **)monsters, num_monsters);
    
    round_robin_game_loop();
    
    endwin();
    cleanup();
    return 0;
}

