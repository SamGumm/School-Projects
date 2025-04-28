#include "dungeon_creation/nagazod_dungeon.hpp"




int main() {
    Dungeon dungeon;

    // dungeon1.create_test_level_1();
    // dungeon1.display_room(dungeon1.traverse(dungeon1.firstRoom(), BSF_PURPOSE::SEARCH_END, 0));

    
    // dungeon.create_test_level_1_1();
    // dungeon.display_room(dungeon.traverse(dungeon.firstRoom(), BSF_PURPOSE::SEARCH_PLAYER, 0, true));
    // dungeon.display_room(dungeon.traverse(dungeon.firstRoom(), BSF_PURPOSE::SEARCH_END, 0, false));
    // Room* room2 = dungeon2.traverse(dungeon2.firstRoom(), BSF_PURPOSE::SEARCH_ROOM, 20);
    // room2->set_description();
    // std::cout << room2->id() << std::endl;

    dungeon.create_test_level_1_2();
    dungeon.display_room(dungeon.traverse(dungeon.firstRoom(), BSF_PURPOSE::SEARCH_PLAYER, 0, true));
    dungeon.display_room(dungeon.traverse(dungeon.firstRoom(), BSF_PURPOSE::SEARCH_END, 0, false));
    dungeon.display_room(dungeon.traverse(dungeon.firstRoom(), BSF_PURPOSE::SEARCH_ROOM, 23, false));

    dungeon.traverse(dungeon.firstRoom(), BSF_PURPOSE::PRINTING, 0, false);

    // // Room* room3 = dungeon3.traverse(dungeon3.firstRoom(), BSF_PURPOSE::TESTING, 0);
    // // std::cout << room3->id() << std::endl;

    // Room* room4 = dungeon3.traverse(dungeon3.firstRoom(), BSF_PURPOSE::SEARCH_ROOM, 50);
    // std::cout << room4->id() << std::endl;

    // Room* room5 = dungeon3.traverse(dungeon3.firstRoom(), BSF_PURPOSE::SEARCH_END, 0);
    // std::cout << room5->id() << std::endl;

}   