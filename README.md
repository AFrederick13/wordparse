# Word Parse: Text Indexer Using C

wordparse loads text files into memory, counting individual words, and providing an efficient interface for querying word frequencies.

## The Mission

To build a tool that can process and index text files (like historical manuscripts or large datasets) using data structures built in C, making the data searchable via a user-friendly web UI.

## Tech Stack

- **Core Engine:** C
- **Data Structures:** Custom Hash Table with Singly Linked Lists for collision handling.
- **Web Bridge:** Python & Flask
- **Frontend:** HTML, CSS, JavaScript
- **Deployment:** Docker & Render/Railway

## Key Technical Features

- **Custom Hashing:** Implementation of the **djb2** hash algorithm to ensure high distribution and minimize collisions.
- **Efficient Parsing:** Custom tokenizer that handles alphanumeric characters, hyphens, and apostrophes while normalizing text to lowercase.
- **Manual Memory Management:** Strict adherence to `malloc` and `free` cycles, verified with **Valgrind** to ensure zero memory leaks.
- **Inter-process Communication:** Utilizing Python's `subprocess` module to bridge high-level web requests with low-level binary execution.
- **Containerization:** A multi-stage Docker build that compiles the C source code and serves the Flask API in a unified, platform-independent environment.

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
