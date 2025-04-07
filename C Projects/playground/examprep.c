#include <stdlib.h>
#include <stdio.h>
//a function that takes a pointer of type void * 
//and an integer n representing the number of bytes, 
//and prints the hexadecimal values of those n bytes
void printBytes(void *ptr, size_t n) {
    unsigned char *p = (unsigned char *)ptr;
    for(size_t i = 0; i < n; i++) {
        printf("%02x ", p[i]);
    }
    printf("\n");
};


// The name of enumeration is "flag" and the constant
// are the values of the flag. By default, the values
// of the constants are as follows:
// constant1 = 0, constant2 = 1, constant3 = 2 and 
// so on.
/*

type flag       values                                   */
enum character {
//default is sequential assignment
//  0         1       2            3      4 
    Superman, Batman, Wonderwoman, Flash, Aquaman
};

enum example {
//can assign the values integer numbers
    ex1 = 1, ex2 = 2, ex10 = 10
};

enum example2 {
//enumeration starts at 4 now
//  4        5    6
    ex4 = 4, ex5, ex6
};






int main() {
    enum character mystery = 2;
    switch(mystery) {
        case 0:
            printf("Mystery Character is Superman!\n");
        case 1:
            printf("Mystery Character is Batman!");
        case 2:
            printf("Mystery Character is Wonderwoman!\n");
        case 3:
            printf("Mystery Character is Flash!\n");
        case 4:
            printf("Mystery Character is Aquaman!\n");
    }
    int a = 20;
    float b = 20.1;
    typedef struct node {
        int size;
        void *datum;
    } node_t;
    node_t c = {10, NULL};
    size_t n;
    n = 40;
    void *ptr = &a;
    printBytes(ptr, sizeof(a));
    printBytes(ptr, sizeof(a));
    ptr = &b;
    printBytes(ptr, sizeof(b));
    ptr = &c;
    printBytes(ptr, sizeof(c));
//START OF EXAM CHECKING
enum {
    marty, biff, doc, george, jennifer, libyans
};

char *car[] = {
    "81_DeLorean_DMC-12", "46_Ford_Super_De_Luxe", "48_Packard_Custom_Eight_Victoria", "84_BMW_733i", "84_AMC_Eagle_4WD_Wagon", "Volkswagon_Type_2_Microbus"
};

char character[][15] = {
    "Marty_McFly", "Biff_Tannen", "Emmet_Brown", "George_McFly", "Jennifer_Parker", "The_Libyans"
};

char *s = "There's_that_word_again.__\"Heavy\".";
printf("%s\n", s);








}