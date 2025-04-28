/*
|   - Rooms will have N E S W options upon creation
|       - Pointers to other rooms
|   - Corridors will be empty linkages of rooms
|       - A CORRIDOR must exist between two room
|   - A room can exist as one of 7 states
|       - Guaranteed Loot Room
|       - Random Change Loot Room
|       - Next Level Room
|       - Monster Room
|       - Start Room
|       - Corridor
|       - Boss Room
|       - Empty (default value)
|   - Will exist as a quad-linked list
*/
#ifndef NAGAZOD_ROOM_HPP
#define NAGAZOD_ROOM_HPP
#include <string>
#include <queue> //for bfs traversal
#include <vector>

/**
 * Determines what function a Room will fulfill.
 */
enum struct RoomType {
    CORRIDOR, //Used to connect rooms, empty. Has a standard dimension. 2 types, tall and long (left-right, up-down)
    G_LOOT_ROOM, //Predetermined loot             
    R_LOOT_ROOM, //Rolls for loot on a loot table 
    END_ROOM, //Ending point of a dungeon  
    MONSTER_ROOM, //Initiates an encounter        
    START_ROOM, //Starting point for a dungeon    
    BOSS_ROOM, //Contains the boss                
    EMPTY //Default value for construction        
};

/**
 * Dimensions of how to draw the room on the map.
 */
struct Room_Dimension {
    int width, height;
};

/**
 * A subunit of the Dungeon.
 */
class Room {    
    public:
        //Getters
        Room* north() const { return north_; }
        Room* south() const { return south_; }
        Room* east() const { return east_; }
        Room* west() const { return west_; }
        bool contains_player() { return contains_player_; }
        bool has_been_visited() { return has_been_visited_; }
        RoomType roomtype() { return type_; };
        std::string description() { return desc_; }
        //Setters

        /**
         * North-South/East-West linkers functionally accomplish the same thing, just from a different axis/direction
         * So I could just collapse them into each other, but i dont want to, it makes it a little easier 
         * to keep things straight in my head.
         */

        void linkNorth(Room* r);
        void unlinkNorth();

        void linkSouth(Room* r);
        void unlinkSouth();

        void linkEast(Room* r);
        void unlinkEast();

        void linkWest(Room* r);
        void unlinkWest();
        
        void set_description();

        void has_been_visited(bool state) { has_been_visited_ = state; }
        void contains_player(bool state) { contains_player_ = state; }

        int id() const noexcept { return id_; };

        //Constructors
        explicit Room(RoomType t); //convenience constructor, linkages/description will be set after creation

    private: 
        Room *north_ {nullptr}; //C++ doesnt like when i have north (variable) and north() (function) T_T
        Room *south_ {nullptr};
        Room *east_ {nullptr};
        Room *west_ {nullptr};
        bool contains_player_ {false};
        bool has_been_visited_ {false};
        std::string desc_ {NULL};
        Room_Dimension dimensions_ {0,0};
        RoomType type_ {RoomType::EMPTY}; //in-class initializer
        int id_;
        static inline int nextId_{0};

    protected:

};

#endif