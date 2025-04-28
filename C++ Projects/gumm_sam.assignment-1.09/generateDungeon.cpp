#include <algorithm> //for std::swap, std::max, std::find
#include <ncurses.h>
#include <cstdlib> //for c-version of rand()
#include "generateDungeon.hpp"
#include "heap.h"
#include <memory>
#include <cmath> //for std::abs
#include <climits> //for INF
#include "monster_description.hpp"
#include "monster_factory.hpp"
#include <iostream>
#include "parser.hpp"
#include <set> //std::set
#include "objects.hpp"
#include "object_factory.hpp"
#include "object_description.hpp"


const char* equip_slot_name(EquipSlot s);
ObjectType slot_to_object_type(EquipSlot s);
class Dungeon {
    public:
        void add_object(const Object& o);
        bool remove_object(const Object* ptr);
        static constexpr int H = DUNGEON_HEIGHT;
        static constexpr int W = DUNGEON_WIDTH;
        Parser parser;
        Player player;
        std::vector<MonsterDescription> monster_descs;
        std::vector<Monster> monsters;
        std::set<std::string> used_uniques;
        std::vector<ObjectDescription> object_descs;
        std::vector<Object> objects;
        std::set<std::string> used_artifacts;     // once picked up



        Dungeon() {
            monster_descs = parser.readMonsterDescriptions("monster_list.txt");
            object_descs = parser.readObjectDescriptions("object_data.txt");
        }
        
        // Returns true if room 'a' and room 'b' overlap (with a 1-cell gap).
        bool rooms_overlap(const room_t& a, const room_t& b) const {
            if ((a.x - 1) > (b.x + b.width) || (b.x - 1) > (a.x + a.width))
                return false;
            if ((a.y - 1) > (b.y + b.height) || (b.y - 1) > (a.y + a.height))
                return false;
            return true;
        }

        // carve a straight line: if horizontal==true, fix row=fixed, vary x=start→end;
        // otherwise fix col=fixed_coord, vary y=start→end.
        void carve_line(bool horizontal, int fixed_coord, int start, int end) {
            if (start > end) {
                std::swap(start, end);
            }
            for (int v = start; v <= end; ++v) {
                int y = horizontal ? fixed_coord : v;
                int x = horizontal ? v : fixed_coord;

                if (map[y][x] == ' ') {
                    map[y][x] = '#';
                    hardness[y][x] = 0;
                }
            }
        }

        // Carves an L‑shaped corridor between (x1,y1) and (x2,y2).
        // use with myDungeon.carve_corridor to carve a L-shaped 
        void carve_corridor(int x1, int y1, int x2, int y2) {
            bool horiz = (rand() & 1) == 0;
            if (horiz) {
                carve_line(true,  y1, x1, x2); // horizontal segment at y = y1
                carve_line(false, x2, y1, y2); // vertical segment at x = x2
            } else {
                carve_line(false, x1, y1, y2); // vertical segment at x = x1
                carve_line(true,  y2, x1, x2); // horizontal segment at y = y2
            }
        }

        /* cell_compare: Comparison function for cell_t pointers used in dijkstra.
        */
        static int32_t cell_compare(const void *key, const void *with) {
            const cell_t *c1 = static_cast<const cell_t*>(key);
            const cell_t *c2 = static_cast<const cell_t*>(with);
            // return -1, 0, or +1 depending on c1->distance vs c2->distance
            return (c1->distance < c2->distance) ? -1 : (c1->distance > c2->distance) ? +1 : 0;
        }

        //Use with d.generate_dungeon();
        void generate_dungeon() {
            int x, y;
            room_t rooms[MAX_ROOMS];
            int num_rooms = 0;
            int i, j;
            int attempts = 0;


            for(y = 0; y < H; y++) {
                for(x = 0; x < W; x++) {
                    if(y == 0 || y == H - 1 || x == 0 || x == W - 1) {
                        map[y][x] = ' ';
                        hardness[y][x] = 255;
                    }
                    else {
                        map[y][x] = ' ';
                        hardness[y][x] = (rand() % 254) + 1;
                    }
                }
            }

            while (attempts < MAX_ROOM_ATTEMPTS && num_rooms < MIN_ROOMS) {
                int room_width = MIN_ROOM_WIDTH + rand() % (MAX_ROOM_WIDTH - MIN_ROOM_WIDTH + 1);
                int room_height = MIN_ROOM_HEIGHT + rand() % (MAX_ROOM_HEIGHT - MIN_ROOM_HEIGHT + 1);
                int room_x = 1 + rand() % (W - room_width - 2);
                int room_y = 1 + rand() % (H - room_height - 2);
        
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
                            map[j][i] = '.';
                            hardness[j][i] = 0;
                        }
                    }
                    rooms[num_rooms++] = new_room;
                }
                attempts++;
            }
        
            if (num_rooms < MIN_ROOMS) {
                std::cout << "Error: Only " << num_rooms << " rooms could be placed after " <<  attempts << " attempts." << std::endl;
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
                carve_corridor(room1_center_x, room1_center_y,
                               room2_center_x, room2_center_y);
            }
        
            // Place stairs.
            int up_stair_x, up_stair_y, down_stair_x, down_stair_y;
            {
                int room_index = rand() % num_rooms;
                up_stair_x = rooms[room_index].x + rand() % rooms[room_index].width;
                up_stair_y = rooms[room_index].y + rand() % rooms[room_index].height;
                map[up_stair_y][up_stair_x] = '<';
            }
            {
                int room_index;
                do {
                    room_index = rand() % num_rooms;
                    down_stair_x = rooms[room_index].x + rand() % rooms[room_index].width;
                    down_stair_y = rooms[room_index].y + rand() % rooms[room_index].height;
                } while (down_stair_x == up_stair_x && down_stair_y == up_stair_y);
                map[down_stair_y][down_stair_x] = '>';
            }
        }

        // Renders a distance map with digits 0–9 (space for INF) and '@' at the PC.
        void render_distance_map(const int dist[H][W]) const {
            for (int y = 0; y < H; ++y) {
                for (int x = 0; x < W; ++x) {
                    if (x == char_pos.x && y == char_pos.y) {
                        putchar('@');
                    } else if (dist[y][x] == INF) {
                        putchar(' ');
                    } else {
                        putchar('0' + (dist[y][x] % 10));
                    }
                }
                putchar('\n');
            }
        }

        bool monsters_remaining() const {
            for (const Monster &m : monsters) {
                if (m.alive) {
                    return true;
                }
            }
            return false;
        }
          

        void place_pc_starting_pos() {
            int x, y;
            do {
                x = rand() % W;
                y = rand() % H;
            } while (hardness[y][x] != 0);
            char_pos.x = x;
            char_pos.y = y;
        }

        /**
        * Runs Dijkstra's algorithm on this dungeon, filling `dist[y][x]`
        * with the minimum-cost path distances from the PC's position.
        *
        * @param dist   A 2D array [H][W] which will be populated.
        * @param tunnel If true, monsters can tunnel through rock with extra cost.
        */
        void dijkstra(int dist[H][W], bool tunnel) const {
            // 1) Initialize all distances to INF
            for (int y = 0; y < H; ++y) {
            for (int x = 0; x < W; ++x) {
                dist[y][x] = INF;
            }
            }
            // Distance to self is zero
            dist[char_pos.y][char_pos.x] = 0;

            // 2) Set up min‑heap
            heap_t h;
            heap_init(&h, cell_compare, nullptr);

            // Push the start cell
            cell_t* start = new cell_t{char_pos, 0};
            heap_insert(&h, start);

            static constexpr int dirs[8][2] = {
            {-1,-1},{ 0,-1},{ 1,-1},
            {-1, 0},        { 1, 0},
            {-1, 1},{ 0, 1},{ 1, 1}
            };

            // 3) Main loop
            while (h.size > 0) {
            cell_t* curr = static_cast<cell_t*>(heap_remove_min(&h));
            int x        = curr->position.x;
            int y        = curr->position.y;
            int d0       = curr->distance;
            delete curr;

            for (auto& dir : dirs) {
                int nx = x + dir[0];
                int ny = y + dir[1];

                // skip out of bounds
                if (nx < 0 || nx >= W || ny < 0 || ny >= H) {
                continue;
                }

                int w;
                if (!tunnel) {
                // non-tunneling: only through empty space
                if (hardness[ny][nx] != 0) {
                    continue;
                }
                w = 1;
                } else {
                // tunneling: rock costs extra, immutable rock (255) is impassable
                if (hardness[ny][nx] == 255) {
                    continue;
                }
                w = (hardness[ny][nx] == 0)
                        ? 1
                        : 1 + (hardness[ny][nx] / 85);
                }

                int nd = d0 + w;
                if (nd < dist[ny][nx]) {
                dist[ny][nx] = nd;
                cell_t* nb   = new cell_t{{nx, ny}, nd};
                heap_insert(&h, nb);
                }
            }
            }

            // 4) Clean up
            heap_delete(&h);
        }

        // Returns true if (x,y) is occupied by the PC or any alive monster.
        bool is_cell_occupied(int x, int y) const {
            // Check the PC first
            if (char_pos.x == x && char_pos.y == y) {
              return true;
            }
        
            // Check each monster
            for(const Monster &m : monsters) {
                if(m.alive && m.position.x == x && m.position.y == y) {
                    return true;
                }
            }
            return false;
        }

        /**
         * Picks a random cell (x,y) where hardness==0 and nothing is occupying it.
         * @param[out] x  column index
         * @param[out] y  row index
         */
        void get_random_open_cell(int &x, int &y) const {
            do {
                x = rand() % W;
                y = rand() % H;
            } while (hardness[y][x] != 0 || is_cell_occupied(x, y));
        }
        
        void move_monster(Monster &m) {
            if (!m.alive) return;

            /* --------------------------------------------------------- *
            * 1) choose a step (ERRATIC or greedy toward the player)
            * --------------------------------------------------------- */
            int dx = 0, dy = 0;
            if (m.has_ability("ERRATIC") && (rand() & 1)) {
                dx = (rand() % 3) - 1;           // −1,0,+1
                dy = (rand() % 3) - 1;
            } else {
                dx = (player.position.x > m.position.x) - (player.position.x <  m.position.x);
                dy = (player.position.y > m.position.y) - (player.position.y <  m.position.y);
            }
            const int new_x = m.position.x + dx;
            const int new_y = m.position.y + dy;

            /* --------------------------------------------------------- *
            * 2) bounds & rock handling (tunnelling if ability set)
            * --------------------------------------------------------- */
            if (new_x < 0 || new_x >= W || new_y < 0 || new_y >= H) return;

            if (hardness[new_y][new_x] != 0) {
                if (!m.has_ability("TUNNEL") || hardness[new_y][new_x] == 255)
                    return;                                 // can’t enter
                /* TUNNEL: dig and move */
                hardness[new_y][new_x] = std::max(0, hardness[new_y][new_x] - 85);
                if (hardness[new_y][new_x] > 0) return;     // not yet passable
                map[new_y][new_x] = '#';                    // leave corridor
            }

            /* --------------------------------------------------------- *
            * 3) collision with the player  → the player dies
            * --------------------------------------------------------- */
            // monster attacks PC, then end its turn
            if (new_x == player.position.x && new_y == player.position.y) {
                int dmg = m.attack();
                player.HP -= dmg;
                mvprintw(0,0,"The %s hits you for %d HP!", m.NAME.c_str(), dmg);
                if (player.HP <= 0) {
                    player.alive = false;
                    mvprintw(1,0,"You have been slain!");
                }
                return;  // end monster’s turn — no movement
            }


            /* --------------------------------------------------------- *
            * 4) collision with another monster
            * --------------------------------------------------------- */
            for (Monster &other : monsters) {
                if (&other != &m && other.alive &&
                    other.position.x == new_x && other.position.y == new_y)
                {
                    other.alive = false;
                    if (other.is_unique) used_uniques.insert(other.NAME);
                    break;                                // only one in a cell
                }
            }

            /* --------------------------------------------------------- *
            * 5) finally move this monster
            * --------------------------------------------------------- */
            m.position = {new_x, new_y};
        }


        void process_pc_command(int command) {
            int new_x = player.position.x;
            int new_y = player.position.y;
        
            // Clear any previous status line
            move(0, 0);
            clrtoeol();
        
            switch (command) {
            case 'f':
                // Toggle fog‑of‑war
                player.fullVisibility = !player.fullVisibility;
                if (player.fullVisibility) {
                    mvprintw(0, 0, "Fog of war disabled (full map visible).");
                } else {
                    mvprintw(0, 0, "Fog of war enabled.");
                }
                return;
        
            case 'g':
                process_teleport_mode();
                return;
        
            // Numpad/vi‑style movement
            case '7': case 'y': new_x--; new_y--; break;
            case '8': case 'k':          new_y--; break;
            case '9': case 'u': new_x++; new_y--; break;
            case '4': case 'h': new_x--;           break;
            case '6': case 'l': new_x++;           break;
            case '1': case 'b': new_x--; new_y++; break;
            case '2': case 'j':          new_y++; break;
            case '3': case 'n': new_x++; new_y++; break;
        
            case '.':
                if (map[player.position.y][player.position.x] == '>') {
                    mvprintw(0, 0, "Going down stairs...");
                    refresh();
                    generate_new_level();
                } else {
                    mvprintw(0, 0, "There is no downward staircase here.");
                }
                return;
        
            case ',':
                if (map[player.position.y][player.position.x] == '<') {
                    mvprintw(0, 0, "Going up stairs...");
                    refresh();
                    generate_new_level();
                } else {
                    mvprintw(0, 0, "There is no upward staircase here.");
                }
                return;
        
            case '5': case ' ':
                // Wait a turn
                return;
        
            case 'q':
                // Quit
                player.alive = false;
                return;
                
                case 'i': player.list_inventory(); return;
                case 'e': player.list_equipment(); return;
                case 'w': { player.list_inventory();
                            int idx = getch()-'0';
                            if(idx>=0&&idx<10) player.equip_item(idx,EquipSlot::WEAPON);
                            return;}
                case 't': { player.list_equipment();
                            char s = getch();                   // e.g. 'w' for weapon slot
                            if(s=='w') player.unequip_item(EquipSlot::WEAPON,
                                                           player.first_free_carry_index());
                            return;}
                case 'd': { player.list_inventory();
                            int idx = getch()-'0';
                            if(idx>=0&&idx<10) player.drop_from_inventory(idx,*this);
                            return;}
                case 'x': { player.list_inventory();
                            int idx = getch()-'0';
                            if(idx>=0&&idx<10) player.expunge_from_inventory(idx);
                            return;}
                
            default:
                mvprintw(0, 0, "Unrecognized command.");
                return;
            }
        
            // Bounds check
            if (new_x < 0 || new_x >= W || new_y < 0 || new_y >= H) {
                mvprintw(0, 0, "Cannot move outside the dungeon boundaries.");
                return;
            }
        
            // Wall check
            if (hardness[new_y][new_x] != 0) {
                mvprintw(0, 0, "A wall blocks your way!");
                return;
            }
        
            for (Monster &m : monsters) {
                if (!m.alive ||
                    m.position.x != new_x ||
                    m.position.y != new_y) {
                  continue;
                }
                // PC attacks once, then end the turn
                int dmg = player.attack();
                m.HP -= dmg;
                mvprintw(0,0,"You hit the %s for %d HP!", m.NAME.c_str(), dmg);
                if (m.HP <= 0) {
                  m.alive = false;
                  mvprintw(1,0,"You kill the %s!", m.NAME.c_str());
                }
                return;  // end PC’s turn
              }
              
        
            // Move the PC
            player.position.x = new_x;
            player.position.y = new_y;
            char_pos.x = new_x;
            char_pos.y = new_y;
            for (auto it = objects.begin(); it != objects.end(); ++it) {
                if (it->pos.x == player.position.x && it->pos.y == player.position.y) {
                    // add to inventory (vector<Object> inventory;)
                    player.inventory.push_back(*it);
                    if (it->is_artifact) used_artifacts.insert(it->name);
                    objects.erase(it);
                    mvprintw(0,0,"You pick up %s.", player.inventory.back().name.c_str());
                    break;
                }
            }
            
        }

        void process_teleport_mode() {
            int target_x = player.position.x;
            int target_y = player.position.y;
            bool teleporting = true;
        
            mvprintw(0, 0,
              "Teleport mode: use movement keys to target, 'g' to confirm, 'r' for random.");
            while (teleporting) {
                // Draw the dungeon & monsters
                render_dungeon_ncurses();
                // Overlay the targeting cursor (map starts at row 1)
                mvaddch(target_y + 1, target_x, '*');
                refresh();
        
                int key = getch();
                switch (key) {
                case '7': case 'y':
                    if (target_x > 0 && target_y > 0) {
                        target_x--; target_y--;
                    }
                    break;
                case '8': case 'k':
                    if (target_y > 0) {
                        target_y--;
                    }
                    break;
                case '9': case 'u':
                    if (target_y > 0 && target_x + 1 < W) {
                        target_x++; target_y--;
                    }
                    break;
                case '4': case 'h':
                    if (target_x > 0) {
                        target_x--;
                    }
                    break;
                case '6': case 'l':
                    if (target_x + 1 < W) {
                        target_x++;
                    }
                    break;
                case '1': case 'b':
                    if (target_x > 0 && target_y + 1 < H) {
                        target_x--; target_y++;
                    }
                    break;
                case '2': case 'j':
                    if (target_y + 1 < H) {
                        target_y++;
                    }
                    break;
                case '3': case 'n':
                    if (target_x + 1 < W && target_y + 1 < H) {
                        target_x++; target_y++;
                    }
                    break;
                case 'g':
                    // Confirm teleport
                    if (hardness[target_y][target_x] == 255) {
                        mvprintw(0, 0,
                          "Cannot teleport into immutable rock. Choose another target.");
                        refresh();
                    } else {
                        player.position.x = target_x;
                        player.position.y = target_y;
                        char_pos.x = target_x;
                        char_pos.y = target_y;
                        teleporting = false;
                    }
                    break;
                case 'r': {
                    // Random teleport
                    int rx, ry;
                    do {
                        rx = rand() % W;
                        ry = rand() % H;
                    } while (hardness[ry][rx] == 255);
                    player.position.x = rx;
                    player.position.y = ry;
                    char_pos.x = rx;
                    char_pos.y = ry;
                    teleporting = false;
                    break;
                }
                default:
                    // ignore other keys
                    break;
                }
            }
        
            // After teleport, update the PC's memory of the map
            update_memory();
        }

        void round_robin_game_loop() {
            // Build a turn‐order list: PC first, then each monster
            int total_characters = 1 + static_cast<int>(monsters.size());
            std::vector<Character*> turn_order;
            turn_order.reserve(total_characters);
        
            turn_order.push_back(&player);            // the PC
            for (auto &m : monsters) {               // each Monster in vector<Monster>*
                turn_order.push_back(&m);
            }
        
            int index = 0;
            // Loop until PC dies or no monsters remain
            while (player.alive && monsters_remaining()) {
                Character *current = turn_order[index];
        
                // Skip dead characters
                if (!current->alive) {
                    index = (index + 1) % total_characters;
                    continue;
                }
        
                if (current == &player) {
                    // PC’s turn: read a key and process it
                    int cmd = getch();
                    process_pc_command(cmd);
                } else {
                    // Monster’s turn: move it
                    move_monster(*static_cast<Monster*>(current));
                }
        
                // Re‐draw the dungeon & actors
                render_dungeon_ncurses();
        
                index = (index + 1) % total_characters;
            }
        
            // End‐of‐game message
            if (!player.alive) {
                mvprintw(0, 0, "You lose!");
            } else {
                mvprintw(0, 0, "You win!");
            }
            refresh();
            getch();  // wait for final key
        }
        
        void generate_new_level() {
            monsters.clear();
        
            // 2) Generate a brand‐new dungeon layout
            generate_dungeon();
        
            // 3) Place the PC in the new level
            place_pc_starting_pos();
            player.position = char_pos;
            
            // 3.5) Reset FOW
            for (int y = 0; y < H; ++y) {
                for (int x = 0; x < W; ++x) {
                    player.memory[y][x] = '^'; 
                }
            }
                
            // 4) Populate the new level's monsters:
            populate_monsters(DEFAULT_NMON);
            // 4.5) Populate new levels objects
            populate_objects(10);

            // 5) Draw the new level
            render_dungeon_ncurses();
        
            // 6) Notify the user
            mvprintw(0, 0, "A new floor appears!");
            refresh();
        }
        
        // Frees the heap‑allocated monsters vector.
        // After this call, monsters == nullptr.
        void cleanup() {
            monsters.clear();
        }

        // Copies visible tiles (within a radius of 2) from the dungeon map
        // into the PC's memory buffer (char memory[H][W] inside Player).
        void update_memory() {
            for (int y = 0; y < H; ++y) {
                for (int x = 0; x < W; ++x) {
                    int dx = x - player.position.x;
                    int dy = y - player.position.y;
                    // distance ≤ 2 ⇒ within 5×5 light radius
                    if (std::max(std::abs(dx), std::abs(dy)) <= 2) {
                        player.memory[y][x] = map[y][x];
                    }
                }
            }
        }

        void render_dungeon_ncurses() {
            update_memory();                       // refresh PC memory first

            auto terrain_at = [&](int x,int y)->char {
                if (player.fullVisibility) return map[y][x];

                int dx = std::abs(x - player.position.x);
                int dy = std::abs(y - player.position.y);
                return (std::max(dx,dy) <= 2) ? map[y][x]           // within torch‑light
                                            : player.memory[y][x]; // fog
            };

            auto visible_monster_at = [&](int x,int y)->char {
                for (const Monster &m : monsters) {
                    if (!m.alive || m.position.x != x || m.position.y != y) continue;

                    if (player.fullVisibility) return m.SYMB;

                    int dx = std::abs(x - player.position.x);
                    int dy = std::abs(y - player.position.y);
                    if (std::max(dx,dy) <= 2) return m.SYMB;         // inside light
                }
                return '\0';                                         // none visible
            };

            auto visible_object_at = [&](int cx,int cy)->const Object* {
                for (const Object &o : objects)
                    if (o.pos.x==cx && o.pos.y==cy) {
                        if (player.fullVisibility) return &o;
                        int dx = std::abs(cx - player.position.x);
                        int dy = std::abs(cy - player.position.y);
                        if (std::max(dx,dy) <= 2) return &o;      // inside torch‑light
                    }
                return nullptr;                                   // no visible object
            };

            for (int y = 0; y < H; ++y) {
                for (int x = 0; x < W; ++x) {
        
                    char ch = terrain_at(x,y);
                    int pair_id = 1; // default: your map-terrain pair

                    // 2) Object overlay
                    if (auto *o = visible_object_at(x,y)) {
                        // map curses color constant to our init_pair: (0-7)+1 => 1-8
                        pair_id = o->color + 1;  
                        attron(COLOR_PAIR(pair_id));
                        mvaddch(y+1, x, o->glyph());
                        attroff(COLOR_PAIR(pair_id));
                        continue;
                    }

                    // 3) Monster overlay
                    if (char sym = visible_monster_at(x,y)) {
                        // use first color in m.COLORS or fallback
                        const Monster &m = *std::find_if(monsters.begin(),
                                                        monsters.end(),
                                                        [&](auto &mm){
                                                        return mm.position.x==x &&
                                                                mm.position.y==y && mm.alive;
                                                        });
                        int fg = m.COLORS.empty() ? COLOR_RED : m.COLORS[0];
                        pair_id = fg + 1;
                        attron(COLOR_PAIR(pair_id) | A_BOLD);
                        mvaddch(y+1, x, sym);
                        attroff(COLOR_PAIR(pair_id) | A_BOLD);
                        continue;
                    }

        
                    /* 4) PC always last */
                    if (x == player.position.x && y == player.position.y)
                        ch = '@';
        
                    mvaddch(y + 1, x, ch);   // row 0 is status, map starts at row 1
                }
            }
            refresh();
        }

        void populate_monsters(int count) {
            monsters.clear();
            int placed = 0;
            while (placed < count) {
                const auto &d = monster_descs[rand() % monster_descs.size()];
        
                if (d.is_unique && used_uniques.count(d.name)) continue;
                if ((rand() % 100) >= d.rarity) continue;
        
                Monster m = make_monster(d);
                get_random_open_cell(m.position.x, m.position.y);
                monsters.push_back(m);
                placed++;
            }
        }

        void populate_objects(int min_count = 10) {
            objects.clear();
            int placed = 0;
            while (placed < min_count) {
                const auto &d = object_descs[rand()%object_descs.size()];
                if (d.is_artifact && used_artifacts.count(d.name)) continue;
                if ((rand()%100) >= d.rarity) continue;

                Object o = make_object(d);
                get_random_open_cell(o.pos.x,o.pos.y);
                objects.push_back(o);
                placed++;
            }
        }


    private:
        char map[H][W];
        int hardness[H][W];
        position_t char_pos;
};

bool Player::has_free_carry_slot() const {
    return std::any_of(carry_.begin(), carry_.end(), [](auto p){return p==nullptr;});
  }
  int  Player::first_free_carry_index() const {
    for (int i=0;i<10;i++) if(!carry_[i]) return i;
    return -1;
  }
  bool Player::add_to_inventory(Object *o, Dungeon &d) {
    int idx = first_free_carry_index();
    if (idx<0) return false;
    carry_[idx]=o;
    d.remove_object(o);
    return true;
  }
  void Player::drop_from_inventory(int idx, Dungeon &d) {
    if(idx<0||idx>=10||!carry_[idx]) return;
    carry_[idx]->pos = position;
    d.add_object(*carry_[idx]);
    carry_[idx]=nullptr;
  }
  bool Player::equip_item(int cidx, EquipSlot s) {
    if(cidx<0||cidx>=10) return false;
    if(!carry_[cidx] || carry_[cidx]->type()!=slot_to_object_type(s)) return false;
    std::swap(equip_[(int)s], carry_[cidx]);
    return true;
  }
  bool Player::unequip_item(EquipSlot s, int cidx) {
    if(cidx<0||cidx>=10||carry_[cidx]||!equip_[(int)s]) return false;
    std::swap(equip_[(int)s], carry_[cidx]);
    return true;
  }
  void Player::expunge_from_inventory(int idx){
    if(idx<0||idx>=10||!carry_[idx]) return;
    delete carry_[idx];
    carry_[idx]=nullptr;
  }
  void Player::list_inventory()const{
    clear(); mvprintw(0,0,"Inventory:");
    for(int i=0;i<10;i++)
      mvprintw(i+2,0,"%d) %s",i,carry_[i]?carry_[i]->name.c_str():"(empty)");
    refresh(); getch();
  }
  void Player::list_equipment()const{
    clear(); mvprintw(0,0,"Equipment:");
    for(int s=0;s<(int)EquipSlot::SLOTS;s++)
      mvprintw(s+2,0,"%s: %s",equip_slot_name((EquipSlot)s),
               equip_[s]?equip_[s]->name.c_str():"(none)");
    refresh(); getch();
  }
  void Player::inspect_inventory(int idx)const{
    if(idx<0||idx>=10||!carry_[idx])return;
    clear(); mvprintw(0,0,"%s",carry_[idx]->name.c_str()); refresh(); getch();
  }
  // ──────────────────────────────────────────────────────────────
const char *equip_slot_name(EquipSlot s){
    static const char* names[]={
      "WEAPON","OFFHAND","RANGED","ARMOR","HELMET","CLOAK",
      "GLOVES","BOOTS","AMULET","LIGHT","RING_A","RING_B"};
    return names[(int)s];
  }
  ObjectType slot_to_object_type(EquipSlot s){
    switch(s){
      case EquipSlot::WEAPON:   return ObjectType::WEAPON;
      case EquipSlot::OFFHAND:  return ObjectType::OFFHAND;
      case EquipSlot::RANGED:   return ObjectType::RANGED;
      case EquipSlot::ARMOR:    return ObjectType::ARMOR;
      case EquipSlot::HELMET:   return ObjectType::HELMET;
      case EquipSlot::CLOAK:    return ObjectType::CLOAK;
      case EquipSlot::GLOVES:   return ObjectType::GLOVES;
      case EquipSlot::BOOTS:    return ObjectType::BOOTS;
      case EquipSlot::AMULET:   return ObjectType::AMULET;
      case EquipSlot::LIGHT:    return ObjectType::LIGHT;
      default:                  return ObjectType::RING; // both ring slots
    }
  }
  // ──────────────────────────────────────────────────────────────
    void Dungeon::add_object(const Object& o) {
        objects.push_back(o);
    }

    bool Dungeon::remove_object(const Object* ptr) {
    auto it = std::find_if(objects.begin(), objects.end(),
        [&](const Object &ob){ return &ob == ptr; });
    if (it == objects.end()) {
        return false;
    }
    objects.erase(it);
    return true;
    }



int main() {
    // 1) Initialize ncurses
    initscr();            // start curses mode
    cbreak();             // disable line buffering
    noecho();             // don't echo() while we getch()
    keypad(stdscr, TRUE); // enable arrow and function keys
    start_color();
    init_pair(COLOR_RED,     COLOR_RED,     COLOR_BLACK);
    init_pair(COLOR_GREEN,   COLOR_GREEN,   COLOR_BLACK);
    init_pair(COLOR_YELLOW,  COLOR_YELLOW,  COLOR_BLACK);
    init_pair(COLOR_BLUE,    COLOR_BLUE,    COLOR_BLACK);
    init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_CYAN,    COLOR_CYAN,    COLOR_BLACK);
    init_pair(COLOR_WHITE,   COLOR_WHITE,   COLOR_BLACK);

    // 2) Seed the random generator
    srand(static_cast<unsigned>(time(nullptr)));

    // 3) Create dungeon and first level
    Dungeon dungeon;
    dungeon.generate_new_level();

    // 4) Enter round‑robin play (PC + monsters)
    dungeon.round_robin_game_loop();

    // 5) Tear everything down
    dungeon.cleanup();
    endwin();             // restore terminal

    return 0;
}
