# blogwriter CLI Documentation

A lightweight command-line interface (CLI) for managing blog entries, databases, and displaying the blogwriter identity.

---

## Command Name
`blogwriter`

---

## Syntax

```bash
blogwriter [command / options]
```

Or when running the local binary:

```bash
./blogwriter [command / options]
```

---

## Available Commands

| Command | Description | Status |
| :--- | :--- | :--- |
| `blogwriter -o` | Open and show the logo text once + show commands | Implemented |
| `blogwriter -h` | Show only available commands | Implemented |
| `blogwriter -make <dbs_name>` / `blogwriter make <dbs_name>` | Make a new MongoDB database using `mongosh` | Implemented |
| `blogwriter -in <dbs> create <file_name>` / `crearte` | Create a file `<file_name>` in database `<dbs>` | Implemented |
| `blogwriter -in <dbs_name> open <file_name>` | Open the file `<file_name>` from dbs `<dbs_name>` in nano | Planned |
| `blogwriter -connect <dbs_name> to <repository_name>` | Move dbs to the repository `<repository_name>` | Planned |
| `blogwriter - show <file_name> from <dbs_name>` | Shows file `<file_name>` from the dbs `<dbs_name>` | Planned |
| `blogwriter - check <dbs_name>` / `blogwriter check <dbs_name>` | Verifies `<dbs_name>` exists | Implemented |
| `blogwriter - set admin <admin_name> pass <admin_pass> for <dbs_name>` | Sets admin credentials for database `<dbs_name>` | Implemented |

---

## Usage Examples

### 1. Show Logo & Commands (`-o`)
Displays the official ASCII banner for BlogWriter from `logo.txt` followed by the list of available commands:
```bash
./blogwriter -o
```

### 2. View Available Commands (`-h`)
Shows only the available commands without the banner:
```bash
./blogwriter -h
```

### 3. Create a MongoDB Database (`-make` / `make`)
Creates a new MongoDB database using `mongosh` and initializes its corresponding directory inside `mongoDATABASE/` (also accessible via `mongoDATABSE/`):
```bash
./blogwriter make my_tech_blog
# or
./blogwriter -make my_tech_blog
```

### 4. Verify Database Existence (`- check` / `check`)
Checks if a MongoDB database exists:
```bash
./blogwriter - check my_tech_blog
# or
./blogwriter check my_tech_blog
```

### 5. Set Admin Credentials (`- set admin`)
Sets the administrator username and password for a database:
```bash
./blogwriter - set admin myadmin pass secretPass123 for my_tech_blog
# or
./blogwriter set admin myadmin pass secretPass123 for my_tech_blog
```

### 6. Create File in Database (`-in <dbs> create <file_name>`)
Creates a new file `<file_name>` inside the database `<dbs_name>` directory:
```bash
./blogwriter -in my_tech_blog create my_post.md
# or (using the crearte alias)
./blogwriter -in my_tech_blog crearte my_post.md
```
- If the database exists: outputs `database found`.
  - If the file already exists: outputs `already exists`.
  - If the file does not exist: creates the file inside `mongoDATABASE/<dbs_name>/<file_name>` and registers it in MongoDB if `mongosh` is available.
- If the database does not exist: outputs `database not found`.

---

## Compilation

To compile the C++ source code into the executable binary:

```bash
g++ -std=c++17 -O2 cli_main.cpp mongosh_cmd/makeDBS.cpp mongosh_cmd/check_dbs.cpp mongosh_cmd/setadmin.cpp mongosh_cmd/create_file.cpp -o blogwriter
```

---

## Project Structure

- **`cli_main.cpp`**: C++ main entry point implementing CLI routing and logo display logic.
- **`mongosh_cmd/`**: Directory containing MongoDB command implementations.
  - **`makeDBS.h`**: Header file for database creation command.
  - **`makeDBS.cpp`**: Implementation of `make <dbs_name>` command using `mongosh`.
  - **`checkDBS.h`**: Header file for database verification command.
  - **`check_dbs.cpp`**: Implementation of `- check <dbs_name>` command.
  - **`setadmin.h`**: Header file for setting admin credentials.
  - **`setadmin.cpp`**: Implementation of `- set admin` command using `mongosh`.
  - **`create_file.h`**: Header file for creating files inside databases.
  - **`create_file.cpp`**: Implementation of `-in <dbs> create <file_name>` command.
- **`logo.txt`**: ASCII art banner representing the BlogWriter title.
- **`mongoDATABASE/`**: Directory reserved for MongoDB databases / repositories.
- **`cli-cmd.md`**: Command documentation and usage instructions (also mirrored at `cli-main.md`).
- **`blogwriter`**: The compiled executable binary.
- **`agy.info`**: Project specification and instructions.
