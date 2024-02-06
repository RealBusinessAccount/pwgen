// mkpass - A simple command line password generator.
// Written by Anachronaut
// 1/25/2024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int processArgs(int args, char *argv[], int *length, unsigned int *settings){
    // Process the arguments:
    // -a = include alphabetical characters
    // -n = include numerical characters
    // -p = include punctuation characters
    // -e = extended characters
    // -l accept a user defined length
    char *chr = NULL;
    for (int i = 1; i < args; i++) {
        chr = argv[i];
        if(chr[0] == '-') {
            for (int q = 1; q < strlen(chr); q++) {
                if(chr[q] == 'a') {
                    *settings = *settings | 1;
                } else if (chr[q] == 'n') {
                    *settings = *settings | 2;
                } else if (chr[q] == 'p') {
                    *settings = *settings | 4;
                } else if (chr[q] == 'e') {
                    *settings = *settings | 8;
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
                } else if(chr[q] == 'h' || chr[q] == 'H') {
                    printf("mkpass - simple command line password generator\n\nUsage: mkpass -<options> <length>\n\nOptions:\n  -a Include capital and lowercase letters.\n  -n Include numerals.\n  -p Include punctuation characters.\n  -e Include extended characters. (additional symbols)\n\nWith no options specified, it generates 16 characters using all character sets.\n");
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
    const char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";     // 52 characters
    const char numerals[] = "0123456789";                                               // 10 characters
    const char punctuation[] = ",.!?:;";                                                // 6 characters
    const char extended[] = "@#$%^&*";                                                  // 7 characters
                                                                                        // 75 total characters
    // Take processed arguments and prepare the character set.
    if (*settings == 0) {
        *settings = 0xF;
    }
    if (*settings & 0x1) {
        // Add the alphabet characters to characterSet.
        strcat(characterSet, alphabet);
    }
    if (*settings & 0x2) {
        // Add the numerals.
        strcat(characterSet, numerals);
    }
    if (*settings & 0x4) {
        // Add the punctuation.
        strcat(characterSet, punctuation);
    }
    if (*settings & 0x8) {
        // Add the extended characters.
        strcat(characterSet, extended);
    }
    // printf("Total characters available are: %ld\n", strlen(characterSet) - 1);
    // printf("Possible characters are: %s\n", characterSet);
    return 0;
}

int generateString(int length, char characterSet[]) {
    // Read an unsigned int from /dev/urandom.
    FILE *f;
    unsigned int seed;
    f = fopen("/dev/urandom", "r");
    if (!f) {
        fprintf(stderr, "Error, failed to open /dev/urandom.\n");
        return 1;
    }
    if (fread(&seed, sizeof(seed), 1, f) != 1) {
        fclose(f);
        fprintf(stderr, "Error, failed to read /dev/urandom.\n");
        return 1;
    }
    fclose(f);
    srand(seed);

    for (int i = 0; i < length; i++) {
        printf("%c", characterSet[rand() % strlen(characterSet)]);
    }
    return 0;
}

int main(int args, char *argv[]) {
    int length = 16;
    char characterSet[76];  // 76, one more than the maximum number of characters that can be in the array to account for the null terminator.
    unsigned int settings = 0;

    if ( processArgs(args, argv, &length, &settings) ) {
        fprintf(stderr, "Quitting.\n");
        return 1;
    }
    prepareCharacterSet(&settings, characterSet);
    if ( generateString(length, characterSet)) {
        fprintf(stderr, "Quitting.\n");
        return 1;
    }
    return 0;
}

