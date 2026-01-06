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

// Function to add a word to the hash table
bool insert_word(const char *word)
{
    // Increment the total word count
    total_words++;

    // Hash the word to find the bucket
    unsigned int hash_index = hash(word);

    // Check if the word already exists in the hash table
    node *crawler = hash_table[hash_index];

    // Traverse the linked list at this bucket
    while (crawler != NULL)
    {

        // Compare words (case-sensitive)
        if (strcmp(crawler->word, word) == 0)
        {

            // Increment the count of the current word
            crawler->count++;

            return true; // Word found and count incremented
        }

        // Move to the next node
        crawler = crawler->next;
    }

    size_t word_len = strlen(word);

    // If the word is not found, create a new node
    node *new_node = malloc(sizeof(node) + word_len + 1);

    // Check for malloc failure
    if (new_node == NULL)
    {
        return false;
    }

    // Allocate memory and copy the word into the new node
    new_node->word = malloc(word_len + 1);

    // Check for malloc failure
    if (new_node->word == NULL)
    {
        free(new_node);
        return false;
    }

    // Copy the word into the new node
    strcpy(new_node->word, word);
    new_node->count = 1; // Initialize count to 1 for a new word

    // Insert the new node at the beginning of the list
    new_node->next = hash_table[hash_index];
    hash_table[hash_index] = new_node;

    // Increment the unique word count
    unique_word_count++;
    return true;
}

// Function to process a raw word token for hash table insertion
bool process_raw_word(const char *raw_word) {

    // Extract clean words from the raw token
    char word[LENGTH + 1];

    // Index to track the current position in the array
    int index = 0;

    // Iterate through each character in the raw token
    for (int i = 0; raw_word[i] != '\0'; i++) {

        // Current character in the raw token
        char c = raw_word[i];

        // A "clean" word character: alphanumeric, apostrophe, or hyphen
        if (isalnum(c) || ((c == '\'' || c == '-') && index > 0)) {
            if (index < LENGTH) {
                word[index++] = tolower(c);
            }

        // Non-word character: process the current clean word if any
        } else {
            
            // If we have a clean word to insert
            if (index > 0) {

                // Null-terminate the clean word
                word[index] = '\0';

                // Insert the clean word into the hash table
                if (!insert_word(word)) {
                    return false; // Abort on insertion failure
                }

                // Reset for the next clean word
                index = 0;
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

    // Successful processing
    return true;
}

// Function to load words from a text file into the hash table
bool load(const char *textfile) {

    // Open the file for reading
    FILE *stream = fopen(textfile, "r");

    // Check for file open failure
    if (stream == NULL) {
        return false;
    }

    // Buffer to hold raw word tokens
    char raw_word[LENGTH + 1];

    // Index to track the current position in the array
    int index = 0;

    // Read characters from the file one by one
    for (int c = fgetc(stream); c != EOF; c = fgetc(stream)) {

        // If the character is not whitespace, add it to the raw word buffer
        if (!isspace(c)) {
            if (index < LENGTH) {
                raw_word[index++] = c;
            }

        // If we encounter whitespace and have a raw word to process
        } else if (index > 0) {
            raw_word[index] = '\0';

            // Process the raw word token for insertion
            if (!process_raw_word(raw_word)) {
                fclose(stream);
                unload();
                return false;
            }

            // Reset index for the next raw word
            index = 0;
        }
    }

    // If there's a raw word left to process at EOF
    if (index > 0) {

        // Null-terminate the raw word
        raw_word[index] = '\0';

        // Process the final raw word token
        if (!process_raw_word(raw_word)) {
            fclose(stream);
            unload();
            return false;
        }
    }

    // Close the file after processing
    fclose(stream);
    return true;
}

// Function unloads hash table from memory, returning true if successful, else false
bool unload(void) {

    // Iterate through each bucket in the hash table
    for (int i = 0; i < BUCKETS; i++)
    {
        // Traverse the linked list at the bucket
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

    // Reset global counters
    unique_word_count = 0;
    total_words = 0;
    return true;
}

// Function hashes word to a number (djb2 algorithm)
unsigned int hash(const char *word) {
    
    // Initialize hash value and character variable
    unsigned long hash = 5381;
    int c;

    // Process each character in the word
    while ((c = *word++)) {

        // Update hash using djb2 formula
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    // Return the hash value modulo the number of buckets
    return hash % BUCKETS;
}

// Function checks if a word exists in the hash table
int check(const char *word)
{
    // Hash the lowercase word to find the bucket
    unsigned int hash_index = hash(word);

    // Traverse the linked list at the bucket
    node *crawler = hash_table[hash_index];
    while (crawler != NULL)
    {

        // Compare words (case-insensitive)
        if (strcasecmp(crawler->word, word) == 0)
        {
            return crawler->count; // Word found, return its count
        }

        // Move to the next node
        crawler = crawler->next;
    }

    return 0; // Word not found
}
