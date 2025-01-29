#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {
    time_t seconds;
    seconds = time(NULL);
    srand(seconds);
    int choice = rand() % (2 + 1 - 1) + 1;
    printf("Generated Number: %d", choice);
}





    

    /*
    TODO:
        - fill each link in the line with a random letter
        - use the formula to turn 1d position into 2d position
        - fill out the grid
            -> need to make internal logic for setting letters
                - first explore the path of the word to determine if it will fit
                - if it does, then turn every letter into the necessary one stored in argv or arr
                - if it doesnt go somewhere else
        - maybe have a coin flip to determine if the starting point is top or bottom
            -> then a dice roll happens for every square to determine where to store word
            -> when storing a word, the index is also stored so other words do not replace it 
                -> might be able to do something where the exploring logic also checks for used squares, and whether the squares have the correct letter to use
                    -> so that it can make overlapping words
        */
                                //PLACE WORDS//
    

    //generate random number bounded to size of the array minus index 0
    
    
    // int pos3 = rand() % (122 + 1 - 97) + 97;
    // printf("random number generated: POS1: %d   POS2; %d\n", position, pos3);
    for(int i = 0; i < argc; i++) {
        int choice = rand() % (2 + 1 - 1) + 1;
        if(choice == 1) {
            //start from top
            int posTop = 0;
            int wordPos = 0;
            while(true) {
                int moveSpaces = rand() % (gridLength);
                posTop =- moveSpaces;
                while(gridLine[posTop]) {
                    if(gridLine[posTop] == argv[i][wordPos]) {
                        printf("THIS WORKS!\n");
                        break;
                    }
                }
                // posTop++;
            }
        }

        else {
            //start from bottom
            int posBottom = argc;
            while(true) {
                int moveSpaces = rand() % (gridLength);
                // posBottom--;
            }
            
        }
        
    }
   






                                //PRINT ARRAY//
    //print the stored strings
    








    

    //createGrid();
    //placeWords();
    //fillGrid();
    //printGrid();






