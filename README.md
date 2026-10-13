# mong-CLI

A lightweight C++ command-line interface (CLI) for managing MongoDB databases, user authentication, and blog posts with local directory synchronization.

---

## Overview

**mong-CLI** (executable: `blogwriter`) provides a fast and streamlined CLI for developers, writers, and administrators working with MongoDB. It wraps `mongosh` commands with intuitive syntax, automates local workspace folder mirroring in `mongoDATABASE/`, manages database-level users, and tracks blog files both on the filesystem and in MongoDB collections.

---

## Features

- **Automated Database Creation**: Create new MongoDB databases and initialize default collections using `mongosh`.
- **Local Directory Synchronization**: Automatically maintains corresponding database directories under `mongoDATABASE/` to organize files locally.
- **Database Existence Verification**: Fast dual-check verification across both the local filesystem and live MongoDB instances.
- **Role-Based Admin Setup**: Directly create or update `dbOwner` credentials for specific databases with proper string escaping.
- **Blog & File Management**: Create Markdown and text files inside database workspaces and automatically register metadata in MongoDB.
- **Built-in ASCII Branding**: Includes customized ASCII art banner displayed via `-o`.
- **Flexible CLI Arguments**: Supports hyphenated flags (e.g., `-make`, `-check`), spaced syntax (e.g., `- check`, `- set admin`), and standard subcommands (e.g., `make`, `check`).

---

## Project Structure

```text
mong-CLI/
├── cli_main.cpp               # Main entry point and CLI router
├── logo.txt                   # ASCII banner art
├── README.md                  # Project documentation
├── cli-cmd.md                 # CLI command reference documentation
├── cli-main.md                # Symlink to cli-cmd.md
├── blogwriter                 # Compiled executable binary
├── mongoDATABASE/             # Root directory storing database-mapped files
├── mongoDATABSE               # Symlink to mongoDATABASE for compatibility
├── mongosh_cmd/               # Core command implementations
│   ├── checkDBS.h             # Header for database verification
│   ├── check_dbs.cpp          # Database verification logic
│   ├── create_file.h          # Header for file creation
│   ├── createFile.h           # Compatibility header for file creation
│   ├── create_file.cpp        # File creation and MongoDB registration
│   ├── makeDBS.h              # Header for database creation
│   ├── makeDBS.cpp            # Database creation logic via mongosh
│   ├── setadmin.h             # Header for admin user configuration
│   └── setadmin.cpp           # Admin user setup logic via mongosh
└── agy.info                   # Project task tracking and specification
```

---

## Prerequisites

Before building and running **mong-CLI**, ensure the following tools are installed:

- **C++ Compiler**: `g++` or `clang++` with C++17 support (`std::filesystem`).
- **MongoDB**: Active MongoDB service (local or remote).
- **MongoDB Shell (`mongosh`)**: Installed and accessible in your system `PATH`.

---

## Build Instructions

Compile the source code into the `blogwriter` executable using `g++`:

```bash
g++ -std=c++17 -O2 \
    cli_main.cpp \
    mongosh_cmd/makeDBS.cpp \
    mongosh_cmd/check_dbs.cpp \
    mongosh_cmd/setadmin.cpp \
    mongosh_cmd/create_file.cpp \
    -o blogwriter
```

Make the binary executable if needed:

```bash
chmod +x blogwriter
```

---

## Command Reference

### Summary Table

| Command | Description | Status |
| :--- | :--- | :--- |
| `blogwriter -o` / `--open` | Display ASCII logo banner and list available commands | Implemented |
| `blogwriter -h` / `--help` | Display command usage and syntax help | Implemented |
| `blogwriter make <dbs>` / `-make <dbs>` | Create a new MongoDB database and directory | Implemented |
| `blogwriter check <dbs>` / `- check <dbs>` | Check whether a database exists | Implemented |
| `blogwriter - set admin <user> pass <pass> for <dbs>` | Create or update database owner credentials | Implemented |
| `blogwriter -in <dbs> create <file>` | Create a file in a database and record in MongoDB | Implemented |
| `blogwriter -in <dbs> open <file>` | Open database file in editor (`nano`) | Planned |
| `blogwriter - show <file> from <dbs>` | Display file content from a database | Planned |
| `blogwriter -connect <dbs> to <repo>` | Move/link database to a repository | Planned |

---

## Detailed Usage & Examples

### 1. Display Logo and Help

Display the official ASCII banner and commands:
```bash
./blogwriter -o
```

Display only the command reference:
```bash
./blogwriter -h
```

---

### 2. Create a Database

Creates a new MongoDB database by initializing a default `blogs` collection via `mongosh`, and creates `mongoDATABASE/<dbs_name>/`:

```bash
./blogwriter make tech_blog
# or
./blogwriter -make tech_blog
```

---

### 3. Check Database Existence

Verifies if a database exists either on the MongoDB server or in the local `mongoDATABASE` directory:

```bash
./blogwriter check tech_blog
# or
./blogwriter - check tech_blog
```

**Output:**
```text
Database 'tech_blog' exists.
```
*(Returns exit code `0` if found, `1` if not found).*

---

### 4. Configure Database Admin Credentials

Creates a new user with `dbOwner` permissions on the targeted database (or updates password if the user already exists):

```bash
./blogwriter - set admin john pass SecretPass123 for tech_blog
# or shorthand positional syntax:
./blogwriter set admin john SecretPass123 tech_blog
```

**Output:**
```text
Setting admin credentials for database 'tech_blog' using mongosh...
Admin credentials for database 'tech_blog' successfully set.
```

---

### 5. Create a Blog File in a Database

Creates a file inside the database directory (`mongoDATABASE/<dbs>/<file>`) and registers an entry `{ filename: '<file>', createdAt: new Date() }` into the database's `blogs` collection:

```bash
./blogwriter -in tech_blog create my_first_post.md
# or using the crearte alias:
./blogwriter -in tech_blog crearte my_first_post.md
```

**Execution Behavior:**
- Checks if database exists:
  - If database does not exist: prints `database not found`.
  - If database exists: prints `database found`.
    - If file already exists: prints `already exists`.
    - If file does not exist: creates file on disk, logs it into MongoDB, and exits successfully.

---

## Roadmap

- [x] Database creation via `mongosh`
- [x] Database existence verification
- [x] Admin credential assignment (`dbOwner`)
- [x] Database file creation and collection logging
- [ ] Direct file editing in terminal (`-in <dbs> open <file>` using `nano`)
- [ ] Display file contents from database (`- show <file> from <dbs>`)
- [ ] Remote repository connection and synchronization (`-connect <dbs> to <repo>`)

---

## License

This project is open-source. Feel free to use, modify, and contribute.