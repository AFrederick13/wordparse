#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "hashtable.h"

int main(int argc, char *argv[]) {

    printf("+------------------+\n");
    printf("|  wordparse v1.0  |\n");
    printf("+------------------+\n");
    printf("\n");

    if (argc != 2) {
        printf("Usage: ./wordparse ./path/to/textfile\n");
        return 1;
    }

    char *textfile = argv[1];

    // Open the text file using load function
    bool file_loaded = load(textfile);

    if (!file_loaded) {
        printf("Failed to load file: %s\n", textfile);
        return 1;
    }

    printf("File loaded successfully: %s\n", textfile);
    printf("\n");
    printf("Total words in file: %d\n", total_words);
    printf("\n");
    printf("Total unique words loaded: %d\n", unique_word_count);
    printf("---------------------------------------------------------------------\n");
    printf("Enter a word to search for (type:  '/quit' or '/exit'  to end the program):\n");

    char input_word[LENGTH + 1];
    while (1) {
        printf("> ");
        if (scanf("%45s", input_word) != 1) {
            // Clear stdin if there was an issue
            while (getchar() != '\n' && getchar() != EOF);
            continue;
        }

        if (strcmp(input_word, "/quit") == 0 || strcmp(input_word, "/exit") == 0) {
            break;
        }

        int count = check(input_word);
        if (count > 0) {
            printf("'%s' appeared %d time(s) in the file.\n", input_word, count);
        } else {
            printf("'%s' is NOT in the file.\n", input_word);
        }
    }

    // Call unload function to free memory
    unload();

    printf("--------------------------------------------\n");
    printf("File unloaded.\n");

    return 0;
}

