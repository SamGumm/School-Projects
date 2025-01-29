#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

/*NOTES
    - use a 2d array
    - remember how to create a string array
    - pull up ascii chart
*/


/*
TODO:
    - second argument is the size of the array
    - every subsequent arg is a word to be used
    - You are to write a word search generator in C. Your word search generator will take an unbounded
number of parameters on the command line. The first parameter will be a positive integer of magnitude 20
or smaller, giving the number of rows and columns in your desired word search (all generated word search
puzzles will be square). All remaining parameters are words to be hidden in your puzzle. We will not
require you to error check your parameters; your program may assume that all parameters are well formed
(the integer is positive and smaller than 21 and all of the words are composed of letters and are short enough
to fit in the puzzle).
Your words should be inserted randomly into valid locations in your puzzle (if you always start at the
upper left corner, you’re going to generate some pretty boring word search puzzles!). Words may be hidden
in four directions: diagonal up, rightward, diagonal down, and downward. If, after attempting to insert a
word in all possible positions and locations, no valid location is found, your generator should terminate with
a message about which word cannot be fit
*/


int main(int argc, char *argv[]) {   
    //INITIALIZING VARIABLES
    time_t seconds;
    seconds = time(NULL);
    srand(seconds);
    int i = 0; //unimportant
    int memAlloc = 0; //unimportant
    int magSize = atoi(argv[1]);
    int wordAmount = 0;
    while(argv[i]) {
        printf("%d: \n Contents: %s\n String Length: %zu\n Pointer Size: %lu\n\n", i, argv[i], strlen(argv[i]), sizeof(argv[i]));
        memAlloc += sizeof(argv[i]) + strlen(argv[i]);
        i++;
    } //unimportant
    printf("magSize: %d\n", magSize); //unimportant
    printf("magSize memory allocation: %lu\n", sizeof(magSize)); //unimportant
    printf("Total Mem Allocation: %d\n", memAlloc); //unimportant
    printf("Hello, The Program has Started...\n"); //unimportant
    printf("Number of arguments: %d\n", argc); //unimportant

    //CHECKING COMMAND ARGUMENTS FOR ERRORS THEN TAKING VALID WORDS
    //Number of arguments provided on CL, minus argv[0]
    int count = argc - 1;
    if (count < 1) {
        printf("No arguments provided.\n");
        return 1;
    }
    //create and allocate space for array of pointers to contain each string
    //based on the size of the array of pointers
    char **arr = malloc(count * sizeof(*arr));
    if(!arr) {
        perror("malloc failed for arr");
        return 1;
    }
    //copy provided words into newly created array
    //starting at argv[1]
    for (int i = 0; i < count; i++) {
        size_t length = strlen(argv[i + 1]);
        arr[i] = malloc((length + 1) * sizeof(char)); // +1 for '\0'
        if (!arr[i]) {
            perror("malloc failed for arr[i]");
            return 1;
        }
        if(magSize >= strlen(argv[i + 1])) {
            printf("magSize: %d  wordSize: %ld\n", magSize, strlen(argv[i])); //unimportant
            strcpy(arr[i], argv[i + 1]);
            wordAmount++;
        }
        else {
            printf("%s was too large\n", argv[i + 1]);
            arr[i] = NULL;
            continue;
        }
    }
    
    //CREATE AND FILL ARRAY//
    //ASCII 97-122 (Dec) -> characters
    printf("Made it to CREATE AND FILL ARRAY\n"); //unimportant
    int r = magSize;
    int c = magSize;
    int gridLength = magSize * magSize;
    char grid[r][c];
    char gridLine[gridLength - 1];
    printf("Rows: [%d]  Columns: [%d]  Line Length: [%d]  Valid Words: [%d]", r, c, gridLength, wordAmount); //unimportant
    for(int i = 0; i < gridLength; i++) {
        int pos2 = rand() % (122 + 1 - 97) + 97;
        gridLine[i] = pos2;
    }



    //PLACING WORDS IN ARRAY
    for(int i = 0; i < wordAmount; i++) {
        int choice = rand() % (2 + 1 - 1) + 1;
        if(choice == 1) {
            printf("TOP");
            int posTop = 0;
            int wordPos = 0;
            int moveSpaces = rand() % (gridLength);
            printf("Spaces to move: %d", moveSpaces);
            // while(true) {
            //     int moveSpaces = rand() % (gridLength);
            //     posTop =- moveSpaces;
            //     while(gridLine[posTop]) {
            //         if(gridLine[posTop] == argv[i][wordPos]) {
            //             printf("THIS WORKS!\n");
            //             break;
            //         }
            //         posTop++;
            //     }
            //     // posTop++;
            // }
        }
        else {
            printf("BOTTOM");
        }
    }










    


    printf("\nSTORED STRINGS:\n");
    printf("\npointer char array:\n");
    for (int i = 0; i < count; i++) {
        printf("arr[%d] = %s\n", i, arr[i]);
    }
    printf("\n2D grid:\n");
    for (int i = 0; i < count; i++) {
        printf("grid[%d] = %s\n", i, grid[i]);
    }
    printf("\n1D grid:\n");
    for (int i = 0; i < count; i++) {
        printf("gridLine[%d] = %d\n", i , gridLine[i]);
    }


    printf("\nFilled 1D Grid:\n");
    int a = 0;
    int newline = 0;
    char x;
    while(a < gridLength) {
        if(newline == magSize) {
            printf("\n");
            newline = 0;
        }
        x = (char) gridLine[a];
        printf("%c ", x);
        a++;
        newline++;
    }
    printf("\n");

    //free allocated memory
    for (int i = 0; i < count; i++) {
        free(arr[i]);
    }
    free(arr);
    return 0;
}
