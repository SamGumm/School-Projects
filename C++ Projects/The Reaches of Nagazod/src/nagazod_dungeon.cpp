#include "dungeon_creation/nagazod_dungeon.hpp"

void Dungeon::delete_room(Room* victim) {
    if(!victim) return;

    /* Unlink the neighbors*/
    victim->unlinkNorth();
    victim->unlinkSouth();
    victim->unlinkEast();
    victim->unlinkWest();

    /* Erase owning unique_ptr */
    /*Scans through every unique_ptr<Room> in ownedRooms, and returns the iterator that
        points to the element whose RAW pointer equals*/
    auto room = std::find_if(ownedRooms.begin(), ownedRooms.end(), [victim](const std::unique_ptr<Room>& up){ return up.get() == victim; });
    /*Erase the found room from the list*/
    if(room != ownedRooms.end()) ownedRooms.erase(room);

    /* Erase from Observer list */
    roomList.erase(
        std::remove(roomList.begin(), roomList.end(), victim),
        roomList.end());
}

/**
 * Potential thoughts
 * - print the starting room
 *      - then check each pointer (preference to north)
 *          - if player has visited, print that room to the corresponding direction
 *              - 
 */

void Dungeon::display_dungeon() {}

void Dungeon::display_room(Room* r) {
    if (!r) return;
    // Draw an ASCII mini-map of this room
    r->print_room();
    // Then print its long description
    std::cout << "\n" << r->description() << "\n\n";
}


Room* Dungeon::current_room() {
    return traverse(this->firstRoom(), BSF_PURPOSE::SEARCH_PLAYER, 0, true);
}

/**
 * Potential Thoughts
 * - BFS to provide a traversal option for whatever reason
 * - Traverse using the has_been_visited_ bool to follow the player path
 *      - potential use for a roving monster
 * - DFS to build a queue for printing visited parts of the dungeon
 * - Make own search algorithm off of DFS?
 *     - follow furthest path first(preference= n > e > w), mark paths-to-be-visited in other queue
 */

/**
 * Traverses the Dungeon to check the path the player has taken.
 * @param start The starting room to begin the search from, usually the start of the dungeon.
 * @param purpose SEARCH_PLAYER, SEARCH_END, SEARCH_MONSTER, SEARCH_ROOM
 * @param id Number of room to be searched for, generated at object instantiation in order of creation
 * @param requirePlayerVisited (true) causes traverser to only visit rooms the player has, (false) goes through entire graph
 * @return A pointer to desired Room.
 */
Room* Dungeon::traverse(Room *start, BSF_PURPOSE purpose, int id, bool requirePlayerVisited) { 
    //good ol workhorse, doing what needs to be done
    //there is nothing this thing cannot find... like the terminator
    if(!start) return nullptr;

    visited_.clear();
    visited_.reserve(ownedRooms.size());

    std::queue<Room*> q;

    visited_.insert(start->id());
    q.push(start);

    auto enqueue_if = [&](Room* n) {
        if(!n) return; //null
        if(requirePlayerVisited && !n->has_been_visited()) return; //if wanting to only follow player
        if(!visited_.insert(n->id()).second) return; //already visited
        q.push(n);
    };

    while(!q.empty()) {
        Room* cur = q.front();
        q.pop();

        /*per room work here*/
        switch(purpose) {
            case BSF_PURPOSE::SEARCH_END: if(cur->roomtype() == RoomType::END_ROOM) return cur; break;
            case BSF_PURPOSE::SEARCH_MONSTER: return nullptr; break; //TODO
            case BSF_PURPOSE::SEARCH_PLAYER: if(cur->contains_player()) return cur; break;
            case BSF_PURPOSE::SEARCH_ROOM: if(cur->id() == id) return cur; break;
            case BSF_PURPOSE::PRINTING: display_room(cur); break;
        }

        enqueue_if(cur->north());
        enqueue_if(cur->south());
        enqueue_if(cur->east());
        enqueue_if(cur->west());
    }

    /* If we get here, nothing matched */
    return nullptr;
}

void Dungeon::create_test_level_1() {
    Room* start = make_room(RoomType::START_ROOM);
    start->has_been_visited(true);
    Room* corridor = make_room(RoomType::CORRIDOR);
    corridor->has_been_visited(true);

    //boss has not been visited
    Room* boss = make_room(RoomType::BOSS_ROOM);
    boss->has_been_visited(true);
    Room* loot_g = make_room(RoomType::G_LOOT_ROOM);
    loot_g->has_been_visited(true);
    Room* loot_r = make_room(RoomType::R_LOOT_ROOM);
    loot_r->has_been_visited(true);
    Room* monster = make_room(RoomType::MONSTER_ROOM);
    Room* empty = make_room(RoomType::EMPTY);
    Room* end = make_room(RoomType::END_ROOM);

    //linking
    start->linkEast(corridor);
    corridor->linkEast(boss);
    corridor->linkSouth(loot_g);
    loot_g->linkEast(loot_r);
    boss->linkSouth(monster);
    monster->linkEast(empty);
    empty->linkSouth(end);
}

void Dungeon::create_test_level_1_1() {
    //making rooms
    //rooms start out as not having been visited
    //must set to true
    //next step is to make player
    Room* zero = make_room(RoomType::START_ROOM);
    zero->has_been_visited(true);
    Room* one = make_room(RoomType::CORRIDOR);
    one->has_been_visited(true);
    Room* two = make_room(RoomType::G_LOOT_ROOM);
    two->has_been_visited(true);
    Room* three = make_room(RoomType::BOSS_ROOM);
    three->has_been_visited(true);
    Room* four = make_room(RoomType::END_ROOM);
    four->has_been_visited(true);


    //linking
    zero->linkNorth(one);
    one->linkNorth(two);
    two->linkNorth(three);
    two->linkEast(four);

}

void Dungeon::create_test_level_1_2() {
    Room* zero = make_room(RoomType::START_ROOM);
    Room* one = make_room(RoomType::CORRIDOR);
    Room* two = make_room(RoomType::G_LOOT_ROOM);
    Room* three = make_room(RoomType::CORRIDOR);
    Room* four = make_room(RoomType::CORRIDOR);
    Room* five = make_room(RoomType::CORRIDOR);
    Room* six = make_room(RoomType::EMPTY);
    Room* seven = make_room(RoomType::MONSTER_ROOM);
    Room* eight = make_room(RoomType::R_LOOT_ROOM);
    Room* nine = make_room(RoomType::CORRIDOR);
    Room* ten = make_room(RoomType::CORRIDOR);
    Room* eleven = make_room(RoomType::CORRIDOR);
    Room* twelve = make_room(RoomType::EMPTY);
    Room* thirteen = make_room(RoomType::EMPTY);
    Room* fourteen = make_room(RoomType::CORRIDOR);
    Room* fifteen = make_room(RoomType::R_LOOT_ROOM);
    Room* sixteen = make_room(RoomType::CORRIDOR);
    Room* seventeen = make_room(RoomType::EMPTY);
    Room* eighteen = make_room(RoomType::CORRIDOR);
    Room* nineteen = make_room(RoomType::EMPTY);
    Room* twenty = make_room(RoomType::CORRIDOR);
    Room* twentyone = make_room(RoomType::CORRIDOR);
    Room* twentytwo = make_room(RoomType::EMPTY);
    Room* twentythree = make_room(RoomType::BOSS_ROOM);
    Room* twentyfour = make_room(RoomType::CORRIDOR);
    Room* twentyfive = make_room(RoomType::END_ROOM);
    
    //linking
    zero->linkNorth(one);
    one->linkNorth(two);
    two->linkNorth(three);
    two->linkEast(four);
    two->linkWest(five);
    three->linkNorth(six);
    four->linkEast(seven);
    five->linkWest(eight);
    six->linkWest(nine);
    seven->linkNorth(ten);
    eight->linkNorth(eleven);
    nine->linkEast(twelve);
    ten->linkNorth(thirteen);
    eleven->linkNorth(twelve);
    thirteen->linkNorth(fourteen);
    fourteen->linkNorth(fifteen);
    fifteen->linkWest(sixteen);
    sixteen->linkEast(seventeen);
    seventeen->linkNorth(eighteen);
    eighteen->linkNorth(nineteen);
    nineteen->linkEast(twenty);
    nineteen->linkWest(twentyone);
    twenty->linkEast(twentytwo);
    twentyone->linkWest(twentythree);
    twentythree->linkSouth(twentyfour);
    twentyfour->linkSouth(twentyfive);
}

void Dungeon::create_level_1() {
    Room* zero = make_room(RoomType::START_ROOM);
    Room* one = make_room(RoomType::CORRIDOR);
    Room* two = make_room(RoomType::G_LOOT_ROOM);
    Room* three = make_room(RoomType::CORRIDOR);
    Room* four = make_room(RoomType::CORRIDOR);
    Room* five = make_room(RoomType::CORRIDOR);
    Room* six = make_room(RoomType::EMPTY);
    Room* seven = make_room(RoomType::MONSTER_ROOM);
    Room* eight = make_room(RoomType::R_LOOT_ROOM);
    Room* nine = make_room(RoomType::CORRIDOR);
    Room* ten = make_room(RoomType::CORRIDOR);
    Room* eleven = make_room(RoomType::CORRIDOR);
    Room* twelve = make_room(RoomType::EMPTY);
    Room* thirteen = make_room(RoomType::EMPTY);
    Room* fourteen = make_room(RoomType::CORRIDOR);
    Room* fifteen = make_room(RoomType::R_LOOT_ROOM);
    Room* sixteen = make_room(RoomType::CORRIDOR);
    Room* seventeen = make_room(RoomType::EMPTY);
    Room* eighteen = make_room(RoomType::CORRIDOR);
    Room* nineteen = make_room(RoomType::EMPTY);
    Room* twenty = make_room(RoomType::CORRIDOR);
    Room* twentyone = make_room(RoomType::CORRIDOR);
    Room* twentytwo = make_room(RoomType::EMPTY);
    Room* twentythree = make_room(RoomType::BOSS_ROOM);
    Room* twentyfour = make_room(RoomType::CORRIDOR);
    Room* twentyfive = make_room(RoomType::END_ROOM);
    
    //linking
    zero->linkNorth(one);
    one->linkNorth(two);
    two->linkNorth(three);
    two->linkEast(four);
    two->linkWest(five);
    three->linkNorth(six);
    four->linkEast(seven);
    five->linkWest(eight);
    six->linkWest(nine);
    seven->linkNorth(ten);
    eight->linkNorth(eleven);
    nine->linkEast(twelve);
    ten->linkNorth(thirteen);
    eleven->linkNorth(twelve);
    thirteen->linkNorth(fourteen);
    fourteen->linkNorth(fifteen);
    fifteen->linkWest(sixteen);
    sixteen->linkEast(seventeen);
    seventeen->linkNorth(eighteen);
    eighteen->linkNorth(nineteen);
    nineteen->linkEast(twenty);
    nineteen->linkWest(twentyone);
    twenty->linkEast(twentytwo);
    twentyone->linkWest(twentythree);
    twentythree->linkSouth(twentyfour);
    twentyfour->linkSouth(twentyfive);
}

void Dungeon::create_level_2() {
    // A start with four branches: N→Loot, E→Monster, S→Boss, W→Empty→End
    Room* zero    = make_room(RoomType::START_ROOM);     // (0,0)
    Room* north1  = make_room(RoomType::CORRIDOR);       // (–1,0)
    Room* north2  = make_room(RoomType::G_LOOT_ROOM);    // (–2,0)

    Room* east1   = make_room(RoomType::CORRIDOR);       // (0,1)
    Room* east2   = make_room(RoomType::MONSTER_ROOM);   // (0,2)

    Room* south1  = make_room(RoomType::CORRIDOR);       // (1,0)
    Room* south2  = make_room(RoomType::BOSS_ROOM);      // (2,0)
    Room* south3  = make_room(RoomType::END_ROOM);       // (3,0)

    Room* west1   = make_room(RoomType::CORRIDOR);       // (0,–1)
    Room* west2   = make_room(RoomType::EMPTY);          // (0,–2)
    Room* west3   = make_room(RoomType::R_LOOT_ROOM);    // (0,–3)

    // Link North branch
    zero   ->linkNorth(north1);
    north1 ->linkNorth(north2);

    // Link East branch
    zero   ->linkEast(east1);
    east1  ->linkEast(east2);

    // Link South branch
    zero   ->linkSouth(south1);
    south1 ->linkSouth(south2);
    south2 ->linkSouth(south3);

    // Link West branch
    zero   ->linkWest(west1);
    west1  ->linkWest(west2);
    west2  ->linkWest(west3);
}

void Dungeon::create_level_3() {
    // A 3×3 grid:
    // [ G ]—[ S ]—[ C ]
    //   |      |      |
    // [ M ]—[ . ]—[ R ]
    //   |      |      |
    // [ . ]—[ B ]—[ E ]
    //
    // G = G_LOOT_ROOM, S = START_ROOM, C = CORRIDOR
    // M = MONSTER_ROOM, . = CORRIDOR or EMPTY, R = R_LOOT_ROOM
    // B = BOSS_ROOM, E = END_ROOM

    Room* r0 = make_room(RoomType::G_LOOT_ROOM);    // (0,0)
    Room* r1 = make_room(RoomType::START_ROOM);     // (0,1)
    Room* r2 = make_room(RoomType::CORRIDOR);       // (0,2)

    Room* r3 = make_room(RoomType::MONSTER_ROOM);   // (1,0)
    Room* r4 = make_room(RoomType::CORRIDOR);       // (1,1)
    Room* r5 = make_room(RoomType::R_LOOT_ROOM);    // (1,2)

    Room* r6 = make_room(RoomType::CORRIDOR);       // (2,0)
    Room* r7 = make_room(RoomType::BOSS_ROOM);      // (2,1)
    Room* r8 = make_room(RoomType::END_ROOM);       // (2,2)

    // Horizontal links
    r0->linkEast(r1);  r1->linkEast(r2);
    r3->linkEast(r4);  r4->linkEast(r5);
    r6->linkEast(r7);  r7->linkEast(r8);

    // Vertical links
    r0->linkSouth(r3); r1->linkSouth(r4); r2->linkSouth(r5);
    r3->linkSouth(r6); r4->linkSouth(r7); r5->linkSouth(r8);
}

Dungeon::Dungeon() {}

