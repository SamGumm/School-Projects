#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strchrnulExam(const char *s, int c) {
    int count = 0;
    char *ptr = strlen(s);
    while(s) {
        if(s[count] == (char) c) {
            ptr = s[count];
        }
        count++;
    }
    return ptr;
}

char *strdupExam(const char *s) {
    char *newString = malloc(strlen(s) + 1);
    if(!newString) {
        fprintf(stderr, "not enough mem!");
        return;
    }
    int count = 0;
    while(s) {
        newString[count] = s[count];
        count++;
    }
    newString[++count] = '\0';
    return newString;
}

char *strcatExam(char *dst, const char *src) {
    char *returnString = malloc(strlen(dst) + strlen(src) + 1);
    if(!returnString) {
        fprintf(stderr, "failed\n");
        return;
    }
    int count = 0;
    int count2 = 0;
    while(dst) {
        returnString[count] = src[count2];
        count++;
        count2++;
    }
    returnString[++count] = '\0';
    dst = returnString;
    //return dst; 
}

int main() {

    //Question 1
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

    printf("%s:_%s\n", character[marty], car[marty]);

    strcpy(character[marty], "Marty");
    printf("%s\n", character[marty]);

    printf("%s\n", *((char **) car) + 33);

    printf("1: %s\n", character[biff]);
    strcpy(character[biff], "Biff");
    printf("2: %s\n", character[biff]);
    character[biff][4] = '_';
    printf("3: %s\n", character[biff]);



    return 0;
}