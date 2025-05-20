#include <iostream>
#include <vector>
#include <random>

#include "dungeon_creation/nagazod_dungeon.hpp"
#include "characters/nagazod_player.hpp"
#include "combat/nagazod_encounter.hpp"
#include "characters/nagazod_monster.hpp"
#include "loot/nagazod_object_factory.hpp"

// ANSI‐escape clear screen + home
static void clear_screen() {
    std::cout << "\033[2J\033[H";
}

int main() {
    Dungeon dungeon;
    Player  player;

    // --- 1) Setup level 1 ---
    dungeon.create_level_1();
    player.set_current_room(dungeon.firstRoom());

    bool running = true;
    int  level   = 1;

    while (running) {
        clear_screen();

        // --- 2) Show stats & inventory ---
        player.display_stats();
        player.display_inventory();
        std::cout << "\n";

        // --- 3) Describe current room ---
        Room* room = player.currentRoom();
        std::cout << "Room #" << room->id()
                  << " [" << room->roomtype_string() << "]\n\n";
        dungeon.display_room(room);

        // Grab a reference to the shared RNG
        std::mt19937& rng = player.rng();

        // --- 4) First‐visit event ---
        if (!room->has_been_visited()) {
            switch (room->roomtype()) {
                case RoomType::G_LOOT_ROOM: {
                        std::vector<Object_ID> table = {
                        Object_ID::IRON_SWORD,
                        Object_ID::IRON_HELMET,
                        // add more high‐tier items here
                        };
                        std::uniform_int_distribution<size_t> chooser(0, table.size()-1);
                        Object_ID found = table[ chooser(rng) ];
            
                        auto item = make_object(found);
                        std::cout << "\nYou uncover a treasure chest and find: "
                                << item->displayStatBlock() << "\n";
                        player.add_to_inventory(std::move(item));
                } break;
                case RoomType::R_LOOT_ROOM: {
                    // choose from small loot table
                    std::vector<Object_ID> table =
                    (room->roomtype() == RoomType::G_LOOT_ROOM)
                        ? std::vector<Object_ID>{ Object_ID::IRON_SWORD,
                                                Object_ID::IRON_HELMET }
                        : std::vector<Object_ID>{ Object_ID::CLOTH_BOOTS,
                                                Object_ID::CLOTH_LEGGINGS };

                    std::uniform_int_distribution<size_t> pick(0, table.size()-1);
                    Object_ID found = table[pick(rng)];

                    auto item = make_object(found);
                    std::cout << "\nYou found: " 
                            << item->displayStatBlock() << "\n";
                    player.add_to_inventory(std::move(item));
                } break;

                case RoomType::MONSTER_ROOM: {// pick from several grunt‐type monsters
                    std::vector<std::unique_ptr<Monster>> grunts;
                    grunts.push_back(std::make_unique<Slime>(room));
                    grunts.push_back(std::make_unique<Skeleton>(room));
                    // … add more basic monster types
        
                    std::uniform_int_distribution<size_t> chooser(0, grunts.size()-1);
                    auto  mob = std::move(grunts[ chooser(rng) ]);
                    Encounter fight(player, *mob);
                    if (!fight.engage()) {
                        std::cout << "\n*** YOU DIED. GAME OVER. ***\n";
                        running = false;
                    }
                } break;
                case RoomType::BOSS_ROOM: {
                    auto boss = std::make_unique<Lich_King>(room);
                    Encounter showdown(player, *boss);
                    if (!showdown.engage()) {
                      std::cout << "\n*** THE BOSS HAS VANQUISHED YOU. ***\n";
                      running = false;
                    }
                } break;

                case RoomType::END_ROOM: {
                    std::cout << "\nStairs lead down. Press '>' to descend: ";
                    char c; std::cin >> c;
                    if (c == '>') {
                    level++;
                    switch (level) {
                        case 2: dungeon.create_level_2(); break;
                        case 3: dungeon.create_level_3(); break;
                        default:
                        std::cout << "\nAll floors cleared — you win!\n";
                        running = false;
                        break;
                    }
                    if (running)
                        player.set_current_room(dungeon.firstRoom());
                    continue;  // skip movement prompt
                    }
                } break;

                default:
                    // nothing for START_ROOM, CORRIDOR, EMPTY
                    break;
            }

            // mark visited so we don’t repeat
            room->has_been_visited(true);

            // if we died or won, break out
            if (!running) break;
        }

        // --- 5) Movement prompt ---
        std::cout << "\nCommands: w=North  s=South  a=West  d=East  q=Quit\n> ";
        char cmd; std::cin >> cmd;
        switch (cmd) {
          case 'w': player.move(DIRECTION::NORTH); break;
          case 's': player.move(DIRECTION::SOUTH); break;
          case 'a': player.move(DIRECTION::WEST);  break;
          case 'd': player.move(DIRECTION::EAST);  break;
          case 'q':  running = false;              break;
          default:
            std::cout << "Unknown command\n";
            break;
        }
    }

    std::cout << "\nThank you for playing!\n";
    return 0;
}