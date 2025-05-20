#pragma once

#include "nagazod_room.hpp"
#include <vector>
#include <memory> //for unique_ptr
#include <algorithm> //for erase
#include <iostream>
#include <unordered_set>

/**
 * Defines what function the BSF traverser will accomplish
 */
enum struct BSF_PURPOSE {
    SEARCH_PLAYER, //look for player's current room
    SEARCH_END, //look for the end of the dungeon
    SEARCH_MONSTER, //look for roaming monster
    SEARCH_ROOM, //look for specific room
    PRINTING //go through every room and print details
};

/**
 * Levels of the Dungeon
 */
enum struct FLOOR {
    LEVEL_1, LEVEL_2, LEVEL_3
};

class Dungeon {
    public:
        //Methods
        void delete_room(Room *victim); //delete single room
        void display_dungeon();
        Room* traverse(Room *start, BSF_PURPOSE purpose, int id, bool requirePlayerVisited);
        Room* firstRoom() { return ownedRooms.empty() ? nullptr : ownedRooms.front().get(); } //think about having read-only version
        Room* current_room();
        void display_room(Room* r);
        void move_to_next_floor();
        void set_floor(FLOOR f) { current_floor_ = f; }

        FLOOR current_floor() { return current_floor_; }


        //Pre-determined Dungeon Creation
        void create_test_level_1();
        void create_test_level_1_1();
        void create_test_level_1_2();
        void create_level_1();
        void create_level_2();
        void create_level_3();

        //Random Dungeon Creation (TODO.... eventually)

        //Constructor
        Dungeon();

        //Destructor
        ~Dungeon() = default;


    private:
        //Data
        std::vector<std::unique_ptr<Room>> ownedRooms; //own every room through unique_ptr
        std::vector<Room*> roomList; //raw pointers for doing graph operations (if necessary) (this is an Observer)
        std::unordered_set<int> visited_;
        FLOOR current_floor_ {FLOOR::LEVEL_1};


                
        //Helper methods
        Room* make_room(RoomType t = RoomType::EMPTY) {
            ownedRooms.push_back(std::make_unique<Room>(t)); //allocate and own
            Room *r = ownedRooms.back().get(); //retrieve raw pointer for observer list
            roomList.push_back(r); //add to Observer list
            // 3) Set default dimensions based *only* on the room’s type
            switch (t) {
                case RoomType::START_ROOM:
                r->set_dimensions(8, 6);    // e.g. wide entry hall
                break;
        
                case RoomType::CORRIDOR:
                r->set_dimensions(3, 10);   // treat as vertical by default
                break;
        
                case RoomType::G_LOOT_ROOM:
                case RoomType::R_LOOT_ROOM:
                r->set_dimensions(6, 6);    // square treasure rooms
                break;
        
                case RoomType::MONSTER_ROOM:
                r->set_dimensions(7, 7);
                break;
        
                case RoomType::BOSS_ROOM:
                r->set_dimensions(12, 12);
                break;
        
                case RoomType::EMPTY:
                default:
                r->set_dimensions(5, 5);
                break;
            }
            return r;
        }
};

/**
 * Potential additions:
 *  - map builder
 *  - random map generator
 */

/** Notes
 *  - Owning vs non-owning
 *      - Decides who is responsible for deleting an object
 *          and expresses that in the type system
 *      - going with 'sole owner' approach with unique_ptrs
 * 
 *  - in-class initializer
 *      - ensures objects are consistent and safe even if specialized constructor is never called
 */
