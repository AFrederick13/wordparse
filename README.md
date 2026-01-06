# Word Parse: Text Indexer Using C

This C program "wordparse" loads text files into memory, counts individual words in the file, and provides an interface for querying word frequencies.

## Tech Stack

- **Core Engine:** C
- **Data Structures:** Custom Hash Table with Singly Linked Lists for collision handling.

TODO:
- **Web Bridge:** Python & Flask
- **Frontend:** HTML, CSS, JavaScript
- **Deployment:** Docker & Render/Railway

## Key Technical Features

- **Hashing:** Implementation of the **djb2** hash algorithm to ensure high distribution and minimize collisions.
- **Efficient Parsing:** Custom tokenizer that handles alphanumeric characters, hyphens, and apostrophes while normalizing text to lowercase.
- **Manual Memory Management:** Strict adherence to `malloc` and `free` cycles, verified with **Valgrind** to ensure zero memory leaks.

## Project Structure

```text
wordparse/
├── include/
│   └── hashtable.h       # Header file with struct definitions and prototypes
├── src/
│   ├── hashtable.c       # Hash table implementation and file processing logic
│   └── main.c            # Main entry point and CLI loop
├── tests/
│   └── CrimeandPunishment.txt # Sample text file for testing
└── README.md
```

## Usage

```bash
# Compile
make

# Run
./wordparse tests/CrimeandPunishment.txt
```

## Stats

Ran with 1.7 GB text file:

--- Hash Table Health Report ---
Total words in file: 207673991
Total Unique Words: 10327324
Total Buckets:      1000003
Occupied Buckets:  999970
Load Factor:       10.33
Longest Chain:     28 nodes
