#include "dungeon_creation/nagazod_room.hpp"

void Room::linkNorth(Room* r) {
    north_ = r; //this rooms north exit points to r now
    if(r) { //null check
        r->south_ = this; //r's south exit points to current room
    }
    set_description();
    if(r) r->set_description();
}

void Room::linkSouth(Room* r) {
    south_ = r;
    if(r) {
        r->north_ = this;
    }
    set_description();
    if(r) r->set_description();
}

void Room::linkEast(Room* r) {
    east_ = r;
    if(r) {
        r->west_ = this;
    }
    set_description();
    if(r) r->set_description();
}


void Room::linkWest(Room* r) {
    west_ = r;
    if(r) {
        r->east_ = this;
    }
    set_description();
    if(r) r->set_description();
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