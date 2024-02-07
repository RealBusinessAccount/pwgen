// mkpass - A simple command line password generator.
// Written by Anachronaut
// Updated 2/6/2024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int processArgs(int args, char *argv[], int *length, unsigned int *settings, int *outputs){
    // Process the arguments:
    // -a = include lowercase alphabet
    // -A = include uppercase alphabet
    // -n = include numerical characters
    // -s = include symbol characters
    // -b = bracket characters
    // -l accept a user defined length
    // -o accept a user defined number of strings to generate
    char *chr = NULL;
    for (int i = 1; i < args; i++) {
        chr = argv[i];
        if(chr[0] == '-') {
            for (int q = 1; q < strlen(chr); q++) {
                if(chr[q] == 'a') {
                    *settings = *settings | 1;
                } else if (chr[q] == 'n') {
                    *settings = *settings | 2;
                } else if (chr[q] == 's') {
                    *settings = *settings | 4;
                } else if (chr[q] == 'b') {
                    *settings = *settings | 8;
                } else if (chr[q] == 'A') {
                    *settings = *settings | 16;
                } else if (chr[q] == '-') {
                    // Do nothing.
                } else if (chr[q] == 'l') {
                    if ((i + 1) >= args){
                        fprintf(stderr, "Error: No length specified, please provide a length after '-l'.\n");
                        return 1;
                    }
                    if (!(*length = atoi(argv[i + 1]))) {
                        fprintf(stderr, "Error: Invalid length, length must be a postive integer.\n");
                        printf("%d\n", *length);
                        return 1;
                    }
                } else if (chr[q] == 'o') {
                    if ((i + 1) >= args) {
                        fprintf(stderr, "Error: Number of output strings not specified. Please provide number of outputs after '-o'.\n");
                        return 1;
                    }
                    if (!(*outputs = atoi(argv[i + 1]))) {
                        fprintf(stderr, "Error: Number of output strings is invalid, number must be a positive integer.\n");
                        return 1;
                    }
                } else if(chr[q] == 'h' || chr[q] == 'H') {
                    printf("mkpass - simple command line password generator\n\nUsage: mkpass -<options> <length>\n\nOptions:\n  -a Include lowercase letters.\n  -A include uppercase letters.\n  -n Include numerals.\n  -p Include symbol characters.\n  -b Include bracket characters.\n\nWith no options specified, it generates 16 characters using all character sets.\n");
                    return 1;
                } else {
                    printf("Unknown option: '-%c'. Use -h for help.\n", chr[q]);
                    return 1;
                }
            }
        }
    }
    return 0;
}

int prepareCharacterSet(int *settings, char characterSet[]) {
    // Define acceptable character sets.
    const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";       // 26 characters
    const char ALPHABET[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";       // 26 characters
    const char numerals[] = "0123456789";                       // 10 characters
    const char symbols[] = ",.!?:;@#$%^&*";                     // 13 characters
    const char brackets[] = "[]{}<>()";                         // 8 characters
                                                                // 83 total characters
    // Take processed arguments and prepare the character set.
    if (*settings == 0) {
        *settings = 0x1F;
    }
    if (*settings & 0x1) {
        // Add the lowercase letters to characterSet.
        strcat(characterSet, alphabet);
    }
    if (*settings & 0x2) {
        // Add the numerals.
        strcat(characterSet, numerals);
    }
    if (*settings & 0x4) {
        // Add the symbols.
        strcat(characterSet, symbols);
    }
    if (*settings & 0x8) {
        // Add the bracket characters.
        strcat(characterSet, brackets);
    }
    if (*settings & 0x10) {
        // Add the uppercase letters to characterSet.
        strcat(characterSet, ALPHABET);
    }
    return 0;
}

int generateString(int length, char characterSet[], int *strings) {
    // Length is the length of each string.
    // *strings is the number of strings the user wants to generate.
    FILE *f;
    unsigned int seed;
    unsigned char random[1024];
    int remainingBuffer = 0;

    f = fopen("/dev/urandom", "r");
    if (!f) {
        fprintf(stderr, "Error: Failed to open /dev/urandom.\n");
        return 1;
    }
    // For each string the user wants to generate:
    for (int i = 0; i < *strings; i++) {
        // For each character in the length of that string:
        for (int o = 0; o < length; o++) {
            if (remainingBuffer == 0) {
                // Refill the buffer.
                size_t bytesRead = fread(random, 1, sizeof(random), f);
                if (bytesRead < sizeof(random)) {
                    fprintf(stderr, "Error: Failed to read /dev/urandom.\n");
                }
                remainingBuffer = sizeof(random) - 1;
            }
            printf("%c", characterSet[random[remainingBuffer] % strlen(characterSet)]);
            remainingBuffer--;
        }
        printf("\n");
    }
    fclose(f);
    return 0;
}

int main(int args, char *argv[]) {
    int length = 16;
    char characterSet[84] = {0};  // 84, one more than the maximum number of characters that can be in the array to account for the null terminator.
    unsigned int settings = 0;
    int outputs = 1;

    if ( processArgs(args, argv, &length, &settings, &outputs) ) {
        fprintf(stderr, "Quitting.\n");
        return 1;
    }
    prepareCharacterSet(&settings, characterSet);
    if ( generateString(length, characterSet, &outputs)) {
        fprintf(stderr, "Quitting.\n");
        return 1;
    }
    return 0;
}

