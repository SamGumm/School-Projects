#include "nagazod_room.hpp"
#include <iostream>

void Room::linkNorth(Room* r) {
    north_ = r; //this rooms north exit points to r now
    if(r) { //null check
        r->south_ = this; //r's south exit points to current room
    }
    set_description();
    if(r) r->set_description();
    if (type_ == RoomType::CORRIDOR) set_dimensions(3, 10);
    if (r && r->roomtype() == RoomType::CORRIDOR) r->set_dimensions(3, 10);
}

void Room::linkSouth(Room* r) {
    south_ = r;
    if(r) {
        r->north_ = this;
    }
    set_description();
    if(r) r->set_description();
    if (type_ == RoomType::CORRIDOR) set_dimensions(3, 10);
    if (r && r->roomtype() == RoomType::CORRIDOR) r->set_dimensions(3, 10);

}

void Room::linkEast(Room* r) {
    east_ = r;
    if(r) {
        r->west_ = this;
    }
    set_description();
    if(r) r->set_description();

    if (type_ == RoomType::CORRIDOR) set_dimensions(10, 3);
    if (r && r->roomtype() == RoomType::CORRIDOR) r->set_dimensions(10, 3);
}


void Room::linkWest(Room* r) {
    west_ = r;
    if(r) {
        r->east_ = this;
    }
    set_description();
    if(r) r->set_description();
    if (type_ == RoomType::CORRIDOR) set_dimensions(10, 3);
    if (r && r->roomtype() == RoomType::CORRIDOR) r->set_dimensions(10, 3);
}


void Room::unlinkNorth() {
    if (north_) {
        north_->south_ = nullptr;
        north_ = nullptr;
    }
}

void Room::unlinkSouth() {
    if (south_) {
        south_->north_ = nullptr;
        south_ = nullptr;
    }
}

void Room::unlinkEast() {
    if (east_) {
        east_->west_ = nullptr;
        east_ = nullptr;
    }
}

void Room::unlinkWest() {
    if (west_) {
        west_->east_ = nullptr;
        west_ = nullptr;
    }
}

Room::Room(RoomType t) : type_(t), id_{nextId_++} {
    //initialize the rooms here
};

void Room::set_description() {
    switch(type_) {
        case (RoomType::CORRIDOR): desc_ = "A long corridor stretches out before you"; break;
        case (RoomType::BOSS_ROOM): desc_ = "An imposing chamber awaits you"; break;
        case (RoomType::START_ROOM): desc_ = "Here starts your journey upon this floor"; break;
        case (RoomType::G_LOOT_ROOM): desc_ = "The glint of loot catches your eye"; break;
        case (RoomType::MONSTER_ROOM): desc_ = "Something shifts in the dark"; break;
        case (RoomType::END_ROOM): desc_ = "Stairs lead down to a new floor"; break;
        case (RoomType::R_LOOT_ROOM): desc_ = "You think you see something glinting"; break;
        case (RoomType::EMPTY): desc_ = "An empty room lies before you"; break;
        default: desc_ = "A crumbling, impassible room sits in your path"; break;
    };
    
    if(north()) { desc_ += ". A door lies to the north"; }
    if(south()) { desc_ += ". A door lies to the south"; }
    if(east()) { desc_ += ". A door lies to the east"; }
    if(west()) { desc_ += ". A door lies to the west"; }
    
    desc_ += ".";             
}

Room* Room::neighbor(DIRECTION d) const {
    switch (d) {
        case DIRECTION::NORTH: return north_;
        case DIRECTION::SOUTH: return south_;
        case DIRECTION::EAST:  return east_;
        case DIRECTION::WEST:  return west_;
    }
    return nullptr;  // keep the compiler quiet
}

void Room::print_room() const {
    int w = dimensions_.width;    // number of columns
    int h = dimensions_.height;   // number of rows

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            // border cells get '#', interior gets ' '
            if (y == 0 || y == h - 1 || x == 0 || x == w - 1)
                std::cout << '#';
            else
                std::cout << ' ';
        }
        std::cout << '\n';       // end of this row
    }
}

std::string Room::roomtype_string() const {
    switch (type_) {
        case RoomType::START_ROOM:   return "Start Room";
        case RoomType::CORRIDOR:     return "Corridor";
        case RoomType::G_LOOT_ROOM:  return "Treasure Vault";
        case RoomType::R_LOOT_ROOM:  return "Storage Chamber";
        case RoomType::MONSTER_ROOM: return "Monster Lair";
        case RoomType::BOSS_ROOM:    return "Boss Chamber";
        case RoomType::END_ROOM:     return "Stairwell";
        case RoomType::EMPTY:        return "Empty Room";
        default:                     return "Unknown";
    }
}