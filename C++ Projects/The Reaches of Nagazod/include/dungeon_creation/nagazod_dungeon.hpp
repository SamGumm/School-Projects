#ifndef NAGAZOD_DUNGEON_HPP
#define NAGAZOD_DUNGEON_HPP

#include "nagazod_room.hpp"
#include <vector>
#include <memory> //for unique_ptr
#include <algorithm> //for erase
#include <iostream>
#include <unordered_set>

enum struct BSF_PURPOSE {
    SEARCH_PLAYER, //look for player's current room
    SEARCH_END, //look for the end of the dungeon
    SEARCH_MONSTER, //look for roaming monster
    SEARCH_ROOM, //look for specific room
    PRINTING //go through every room and print details
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

        //Pre-determined Dungeon Creation
        void create_test_level_1();
        void create_test_level_1_1();
        void create_test_level_1_2();
        void create_level_1();
        void create_level_2();
        void create_level_3();

        //Random Dungeon Creation

        //Constructor
        Dungeon();

        //Destructor
        ~Dungeon() = default;


    private:
        //Data
        std::vector<std::unique_ptr<Room>> ownedRooms; //own every room through unique_ptr
        std::vector<Room*> roomList; //raw pointers for doing graph operations (if necessary) (this is an Observer)
        std::unordered_set<int> visited_;
                
        //Helper methods
        Room* make_room(RoomType t = RoomType::EMPTY) {
            ownedRooms.push_back(std::make_unique<Room>(t)); //allocate and own
            Room *r = ownedRooms.back().get(); //retrieve raw pointer for observer list
            roomList.push_back(r); //add to Observer list
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
#endif