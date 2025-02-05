#include <stdlib.h>
#include <stdio.h>

void startIntro() {
    char *titleArr[] = {
    "  _______ _            _____                 _                        __   _   _                                _ ",
    " |__   __| |          |  __ \\               | |                      / _| | \\ | |                              | |",
    "    | |  | |__   ___  | |__) |___  __ _  ___| |__   ___  ___    ___ | |_  |  \\| | __ _  __ _  __ _ _______   __| |",
    "    | |  | '_ \\ / _ \\ |  _  // _ \\/ _` |/ __| '_ \\ / _ \\/ __|  / _ \\|  _| | . ` |/ _` |/ _` |/ _` |_  / _ \\ / _` |",
    "    | |  | | | |  __/ | | \\ \\  __/ (_| | (__| | | |  __/\\__ \\ | (_) | |   | |\\  | (_| | (_| | (_| |/ / (_) | (_| |",
    "    |_|  |_| |_|\\___| |_|  \\_\\___|\\__,_|\\___|_| |_|\\___||___/  \\___/|_|   |_| \\_|\\__,_|\\__, |\\__,_/___\\___/ \\__,_|",
    "         Sam Gumm                                                                       __/ |                     ",
    "                                                                                        "
    };

    int count = 0;
    for (int i = 0; titleArr[i]; i++) {
        
    }
    // while(titleArr[count]) {
    //     printf("%s", titleArr[count]);
    //     count++;
    // }
    printf("                                                          START [s]                                                   \n");
    printf("                                                          CONTINUE [c]                                                \n");
    printf("                                                          QUIT [q]                                                    \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

}

int main(int argc, char *argv[]) {
    /*
    NOTES:
        Can use system("clear") to clear the console and generate something new on the screen
        Can use system("wait") to hang a thread for a bit
    */

    system("clear");
    startIntro();

    return 0;
}


