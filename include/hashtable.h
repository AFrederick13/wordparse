#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdbool.h>
#include <stdlib.h>

// Number of buckets in hash table
#define BUCKETS 1000

// Maximum length for a word
#define LENGTH 45

// Define the structure for an individual node in the hash table/linked list
typedef struct node {
    char *word;            // Pointer to dynamically allocated word
    int count;             // Frequency of the word
    struct node *next;     // Pointer to the next node in the linked list
} node;

// Global variables (declarations only)
extern node *hash_table[BUCKETS];
extern int unique_word_count;
extern int total_words;

// Function prototypes
bool load(const char *filename);
unsigned int hash(const char *word);
int check(const char *word);
bool unload(void);
bool insert_word(const char *word);
bool process_raw_word(const char *raw_word);

#endif
