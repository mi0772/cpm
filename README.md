# CPM - C Password Manager

**CPM** is a simple and secure command-line password manager written entirely in C. It stores passwords locally using binary files and no external dependencies.

This project is designed for developers and power users who prefer full control over their data, dislike browser-based password managers, and want to work close to the metal.

## Features

- Master password required at startup
- Add new entries with label, email/username, and password
- Search for an entry by label
- List all stored entries
- Index-based binary storage for efficient lookup
- All operations performed locally
- No dynamic memory leaks
- No external libraries or frameworks

## Data Structure

Entries are stored in two binary files:

- `vault.dat` — stores the actual password entries
- `vault.idx` — stores an index containing hash and offset of each entry

A password entry is represented as:

```c
typedef struct {
    char *key;
    char *identifier;
    char *password;
} vault_entry;
