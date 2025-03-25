#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "heap.h"

#define DUNGEON_WIDTH 80
#define DUNGEON_HEIGHT 21
#define INF INT_MAX

typedef struct dungeon {
    int hardness[DUNGEON_HEIGHT][DUNGEON_WIDTH];
    char map[DUNGEON_HEIGHT][DUNGEON_WIDTH];
    struct {
        int x, y;
    } pc;
} dungeon_t;

typedef struct cell {
    int x, y;
    int distance;
} cell_t;

void generate_dungeon(dungeon_t *d);
void place_pc(dungeon_t *d);
void render_dungeon(dungeon_t *d);
void render_distance_map(dungeon_t *d, int dist[DUNGEON_HEIGHT][DUNGEON_WIDTH]);
void dijkstra(dungeon_t *d, int tunnel, int dist[DUNGEON_HEIGHT][DUNGEON_WIDTH]);

/* Comparison function for cell_t pointers.
   This is used by the heap to order cells by distance.
*/
int cell_compare(const void *a, const void *b) {
    const cell_t *cell1 = a;
    const cell_t *cell2 = b;
    return cell1->distance - cell2->distance;
}

/* Datum delete function for cells.
*/
void cell_datum_delete(void *v) {
    /* to be completed */
}

/* Generates a simple dungeon.
   Boundary cells are walls (hardness 255) and interior cells are floors (hardness 0).
   An extra wall region is added for testing tunneling.
*/
void generate_dungeon(dungeon_t *d) {
    int x, y;
    for (y = 0; y < DUNGEON_HEIGHT; y++) {
        for (x = 0; x < DUNGEON_WIDTH; x++) {
            if (y == 0 || y == DUNGEON_HEIGHT - 1 || x == 0 || x == DUNGEON_WIDTH - 1) {
                d->map[y][x] = ' ';   /* wall */
                d->hardness[y][x] = 255;
            } else {
                d->map[y][x] = '.';
                d->hardness[y][x] = 0;
            }
        }
    }
    /* Add a region with increased hardness to test tunneling */
    for (y = 5; y < 10; y++) {
        for (x = 20; x < 30; x++) {
            d->map[y][x] = ' ';
            d->hardness[y][x] = 200;
        }
    }
}

/* Randomly places the PC on a floor cell (hardness 0) */
void place_pc(dungeon_t *d) {
    int x, y;
    do {
        x = rand() % DUNGEON_WIDTH;
        y = rand() % DUNGEON_HEIGHT;
    } while (d->hardness[y][x] != 0);
    d->pc.x = x;
    d->pc.y = y;
}

/* Renders the dungeon with the PC marked as '@' */
void render_dungeon(dungeon_t *d) {
    int x, y;
    for (y = 0; y < DUNGEON_HEIGHT; y++) {
        for (x = 0; x < DUNGEON_WIDTH; x++) {
            if (x == d->pc.x && y == d->pc.y)
                putchar('@');
            else
                putchar(d->map[y][x]);
        }
        putchar('\n');
    }
}

/* Renders a distance map.
   For each cell, if reachable (distance != INF), it prints the last digit (mod 10)
   of the computed distance; unreachable cells are printed as a blank space.
   The PC is still shown as '@'.
*/
void render_distance_map(dungeon_t *d, int dist[DUNGEON_HEIGHT][DUNGEON_WIDTH]) {
    int x, y;
    for (y = 0; y < DUNGEON_HEIGHT; y++) {
        for (x = 0; x < DUNGEON_WIDTH; x++) {
            if (x == d->pc.x && y == d->pc.y)
                putchar('@');
            else if (dist[y][x] == INF)
                putchar(' ');
            else
                putchar('0' + (dist[y][x] % 10));
        }
        putchar('\n');
    }
}

/* Dijkstra's algorithm to compute distance maps.
   When tunnel is 0, only floor cells (hardness 0) are traversable (cost = 1).
   When tunnel is nonzero, cells with hardness 1–254 are allowed with a cost of 1 + (hardness/85),
   and cells with hardness 255 are impassable.
   The resulting distances are stored in the provided 2D array dist.
*/
void dijkstra(dungeon_t *d, int tunnel, int dist[DUNGEON_HEIGHT][DUNGEON_WIDTH]) {
    int i, j;
    /* Initialize distances to "infinity" */
    for (i = 0; i < DUNGEON_HEIGHT; i++) {
        for (j = 0; j < DUNGEON_WIDTH; j++) {
            dist[i][j] = INF;
        }
    }
    dist[d->pc.y][d->pc.x] = 0;

    heap_t h;
    heap_init(&h, cell_compare, cell_datum_delete);

    /* Insert the PC's cell as the starting point */
    cell_t *start = malloc(sizeof(cell_t));
    start->x = d->pc.x;
    start->y = d->pc.y;
    start->distance = 0;
    heap_insert(&h, start);

    /* 8-way connectivity */
    int directions[8][2] = {
        {-1, -1}, {0, -1}, {1, -1},
        {-1,  0},          {1,  0},
        {-1,  1}, {0,  1}, {1,  1}
    };

    while (h.size > 0) {  /* Continue while there are nodes in the heap */
        cell_t *curr = heap_remove_min(&h);
        int x = curr->x;
        int y = curr->y;
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
                neighbor->x = nx;
                neighbor->y = ny;
                neighbor->distance = new_dist;
                heap_insert(&h, neighbor);
            }
        }
    }
    heap_delete(&h);
}

int main(void) {
    srand(time(NULL));
    dungeon_t dungeon;
    generate_dungeon(&dungeon);
    place_pc(&dungeon);

    int non_tunnel_dist[DUNGEON_HEIGHT][DUNGEON_WIDTH];
    int tunnel_dist[DUNGEON_HEIGHT][DUNGEON_WIDTH];

    /* Compute distance maps for non-tunneling and tunneling monsters */
    dijkstra(&dungeon, 0, non_tunnel_dist);
    dijkstra(&dungeon, 1, tunnel_dist);

    printf("Dungeon with PC:\n");
    render_dungeon(&dungeon);
    printf("\nNon-tunneling distance map:\n");
    render_distance_map(&dungeon, non_tunnel_dist);
    printf("\nTunneling distance map:\n");
    render_distance_map(&dungeon, tunnel_dist);

    return 0;
}

