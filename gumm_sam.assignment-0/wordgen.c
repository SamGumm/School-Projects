#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_ATTEMPTS 1000

// Directions:
// 0 = Right
// 1 = Down
// 2 = Diagonal Down
// 3 = Diagonal Up

// Check if a word can be placed in the grid starting at (row, col)
// in the given direction (dir). Returns true if it is possible, false otherwise.
bool canPlaceWord(char *grid, int N, const char *word, int row, int col, int dir) {
    int len = strlen(word);

    // Check boundary depending on direction
    switch(dir) {
        case 0: // Right
            // Must not exceed right boundary
            if (col + len > N) return false;
            break;
        case 1: // Down
            // Must not exceed bottom boundary
            if (row + len > N) return false;
            break;
        case 2: // Diagonal Down
            // Must not exceed right or bottom boundary
            if (col + len > N || row + len > N) return false;
            break;
        case 3: // Diagonal Up
            // row must not go above 0, col must not exceed right boundary
            // For "up-right" diagonal, the highest row index we can start from is len-1
            if (col + len > N || row - (len - 1) < 0) return false;
            break;
        default:
            return false;
    }

    // currently only checking for placeholders '.' or matching letters.)
    for(int i = 0; i < len; i++) {
        int r = row;
        int c = col;
        if (dir == 0) {        // Right
            c += i;
        } else if (dir == 1) { // Down
            r += i;
        } else if (dir == 2) { // Diagonal Down
            r += i;
            c += i;
        } else if (dir == 3) { // Diagonal Up
            r -= i;
            c += i;
        }
        char cell = grid[r * N + c];
        // If it's not '.' and not the same letter, can't place
        if (cell != '.' && cell != word[i]) {
            return false;
        }
    }

    return true;
}

// Place the word in the grid (assumes canPlaceWord returned true).
void placeWord(char *grid, int N, const char *word, int row, int col, int dir) {
    int len = strlen(word);

    for(int i = 0; i < len; i++) {
        int r = row;
        int c = col;
        if (dir == 0) {        // Right
            c += i;
        } else if (dir == 1) { // Down
            r += i;
        } else if (dir == 2) { // Diagonal Down
            r += i;
            c += i;
        } else if (dir == 3) { // Diagonal Up
            r -= i;
            c += i;
        }
        grid[r * N + c] = word[i];
    }
}

int main(int argc, char *argv[]) {
    // Seed the random number generator
    srand((unsigned int)time(NULL));

    // Check minimum args: program name + size + at least 1 word
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <gridSize> <word1> [word2] ...\n", argv[0]);
        return 1;
    }

    // Parse the puzzle size
    int N = atoi(argv[1]);
    if (N < 1 || N > 20) {
        fprintf(stderr, "Error: Grid size must be between 1 and 20.\n");
        return 1;
    }

    // Number of words to place
    int wordCount = argc - 2;
    char **words = &argv[2];

    // Allocate the grid and initialize with '.'
    char *grid = malloc(N * N * sizeof(char));
    if (!grid) {
        perror("malloc failed for grid");
        return 1;
    }
    for (int i = 0; i < N*N; i++) {
        grid[i] = '.';
    }

    // Try to place each word
    for (int w = 0; w < wordCount; w++) {
        const char *currentWord = words[w];
        bool placed = false;

        // Attempt up to MAX_ATTEMPTS random placements
        for (int attempt = 0; attempt < MAX_ATTEMPTS && !placed; attempt++) {
            int row = rand() % N;
            int col = rand() % N;
            int dir = rand() % 4; // one of the 4 directions

            if (canPlaceWord(grid, N, currentWord, row, col, dir)) {
                // Place it
                placeWord(grid, N, currentWord, row, col, dir);
                placed = true;
            }
        }

        // If we still haven't placed the word, it means it can't fit
        if (!placed) {
            fprintf(stderr, "Could not place the word '%s' in the puzzle.\n", currentWord);
            free(grid);
            return 1;
        }
    }

    // Fill any remaining '.' with random letters (ASCII 97-122 => 'a' to 'z' (IN DEC))
    for (int i = 0; i < N*N; i++) {
        if (grid[i] == '.') {
            grid[i] = (char)(rand() % 26 + 'a');
        }
    }

    // Print the resulting grid
    printf("\nGenerated Word Search (%dx%d):\n", N, N);
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            printf("%c ", grid[r * N + c]);
        }
        printf("\n");
    }

    // Cleanup
    free(grid);
    return 0;
}
