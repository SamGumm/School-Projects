#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

#define ROW 21
#define COL 80

/*
0___________________________________________________________________________________
|                                                                           |     |
|                                                                           |     |
|    ______                        _________                                |_____|
|   |      |                      |         |                                     |
|   |      |----------------------|         |                                     |
|   |______|                      |_________|                                     |
|                                      |                                          |
|                                      |                                          |
|                                      |____                                      |
|                                          |       ______                         |
|                                          |_______|    |                         |
|                                                  |____|                         |
|                                                                                 |
|___                                                                              |
|___|_____________________________________________________________________________|
• */
void generateBoundaries(char map[][COL]) {
    //The outermost cells of the dungeon are immutable, thus they must remain rock and cannot be part of any room or corridor
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            if(i == 0 || j == 0 || i == ROW - 1 || j == COL - 1) {
                map[i][j] = '8'; //become a border
            }

            else {
                map[i][j] = ' '; //become a rock
            }
        }
    }
}

void generateRooms(char map[][COL]) {
    //Rooms cannot contact one another. There must be at least 1 cell of non-room between any two different rooms.
    //Room cells should be drawn with periods
    //Require at least 6 rooms per dungeon
    //Each room measures at least 4 units in the x direction and at least 3 units in the y direction.
}

void generateCorridors(char map[][]) {
    //corridors are represented by hashes
    //Corridors should not extend into rooms, e.g., no hashes should be rendered inside rooms
}

void generateStairs(char map[][]) {
    //up staircases are represented with less-than signs
    //down staircases with greater-than signs
    /*There should be at least one up and one down staircase. Staircases work like floor (for now) and should be placed in a 
            location where floor would otherwise be. A character on the floor somewhere in the dungeon should be able to walk 
            to the stairs.*/
    
}

void generateRock() {
    //have a hardness field to mimic veins of minerals
    //maybe have it look at surrounding cells to determine rock type
    //and either continue a vein or stop and do something else
    //need to determine what input would be 
    //rock is represented with spaces
}

// void createRoom(char map[][]) {

// }



int main() {
    /*
    REQUIREMENTS:
        - The dungeon should be fully connected, meaning that from any position on the floor, your adventurer
            should be able to walk to any other position on the floor without passing through rock.
    TODO:
        - set parameters
            - Dungeon measures 80 units in the x (horizontal) direction and 21 units in the y (vertical) direction. A
                standard terminal is 80 × 24, and limiting the dungeon to 21 rows leaves three rows for text, things
                like gameplay messages and player status, which come later.
        - create boundaries
        - create rooms
        - create stairs
    */


    char map[ROW][COL]; //create 2d array to hold everything
    generateBoundaries(map); //turn edges of array into rock
    // generateRooms(); //place rooms into the array
    // generateStairs(); // create stair sections 
    // generateTunnels(); // create tunnels between rooms
    // generateRock();// fill the rest of the array with rock
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            putchar(map[i][j]);
        }
        putchar('\n');
    }
    return 0;
}