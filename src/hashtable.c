#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "hashtable.h"

// Define global variables
// Hash table array
node *hash_table[BUCKETS];

// Word counter for unique words
int unique_word_count = 0;

// Word counter for total words
int total_words = 0;

// Helper function to add a word to the hash table
bool insert_word(const char *word)
{
    total_words++;
    // Hash the word to find the bucket
    unsigned int hash_index = hash(word);

    // Check if the word already exists in the hash table
    node *crawler = hash_table[hash_index];
    while (crawler != NULL)
    {
        if (strcmp(crawler->word, word) == 0)
        {
            crawler->count++;
            return true; // Word found and count incremented
        }
        crawler = crawler->next;
    }

    // If the word is not found, create a new node
    node *new_node = malloc(sizeof(node));
    if (new_node == NULL)
    {
        return false;
    }

    size_t word_len = strlen(word);
    new_node->word = malloc(word_len + 1);
    if (new_node->word == NULL)
    {
        free(new_node);
        return false;
    }
    memcpy(new_node->word, word, word_len + 1);
    new_node->count = 1; // Initialize count to 1 for a new word

    // Insert the new node at the beginning of the list
    new_node->next = hash_table[hash_index];
    hash_table[hash_index] = new_node;

    unique_word_count++;
    return true;
}

// Helper function to process a raw word token for hash table insertion
bool process_raw_word(const char *raw_word) {
    char word[LENGTH + 1];
    int index = 0;
    for (int i = 0; raw_word[i] != '\0'; i++) {
        char c = raw_word[i];
        // A "clean" word is alphanumeric, with apostrophes/hyphens allowed mid-word.
        if (isalnum(c) || ((c == '\'' || c == '-') && index > 0)) {
            if (index < LENGTH) {
                word[index++] = tolower(c);
            }
        } else {
            // A non-word character acts as a delimiter inside the raw token.
            if (index > 0) {
                word[index] = '\0';
                if (!insert_word(word)) {
                    return false; // Abort on insertion failure
                }
                index = 0; // Reset for the next clean word
            }
        }
    }
    // If the raw token ends with a clean word.
    if (index > 0) {
        word[index] = '\0';
        if (!insert_word(word)) {
            return false;
        }
    }
    return true;
}

// Loads words from file into hash table, returning true if successful, else false
bool load(const char *textfile) {
    FILE *stream = fopen(textfile, "r");
    if (stream == NULL) {
        return false;
    }

    char raw_word[LENGTH + 1];
    int index = 0;

    for (int c = fgetc(stream); c != EOF; c = fgetc(stream)) {
        if (!isspace(c)) {
            if (index < LENGTH) {
                raw_word[index++] = c;
            }
        } else if (index > 0) {
            raw_word[index] = '\0';
            if (!process_raw_word(raw_word)) {
                fclose(stream);
                unload();
                return false;
            }
            index = 0;
        }
    }

    if (index > 0) {
        raw_word[index] = '\0';
        if (!process_raw_word(raw_word)) {
            fclose(stream);
            unload();
            return false;
        }
    }

    fclose(stream);
    return true;
}

// Unloads hash table from memory, returning true if successful, else false
bool unload(void) {
    // Iterate through each bucket in the hash table
    for (int i = 0; i < BUCKETS; i++)
    {
        node *crawler = hash_table[i];

        // Free each node in the linked list
        while (crawler != NULL) {
            node *temp = crawler->next;
            free(crawler->word);
            free(crawler);
            crawler = temp;
        }
        
        // Set the table index to NULL to prevent double-freeing
        hash_table[i] = NULL;
    }
    unique_word_count = 0;
    total_words = 0;
    return true;
}

// Hashes word to a number (djb2 algorithm)
unsigned int hash(const char *word) {
    unsigned long hash = 5381;
    int c;
    while ((c = *word++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % BUCKETS;
}

// Checks if a word exists in the hash table
int check(const char *word)
{
    // Create a lowercase version of the word to check
    size_t len = strlen(word);
    if (len > LENGTH)
    {
        // The word is longer than any possible word in the dictionary
        return 0;
    }
    char lower_word[LENGTH + 1];

    for (size_t i = 0; i < len; i++)
    {
        lower_word[i] = tolower(word[i]);
    }
    lower_word[len] = '\0';

    // Hash the lowercase word to find the bucket
    unsigned int hash_index = hash(lower_word);

    // Traverse the linked list at the bucket
    node *crawler = hash_table[hash_index];
    while (crawler != NULL)
    {
        if (strcmp(crawler->word, lower_word) == 0)
        {
            return crawler->count; // Word found, return its count
        }
        crawler = crawler->next;
    }

    return 0; // Word not found
}
