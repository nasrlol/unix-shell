# Custom C Shell

A simple Unix-like shell implemented in C with basic file system operations, directory management, and file manipulation commands.

## Table of Contents
- [About](#about)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Available Commands](#available-commands)
- [File Structure](#file-structure)
- [Known Issues](#known-issues)
- [Contributing](#contributing)
- [Author](#author)
- [License](#license)

## About

This project is a custom shell written in C, designed to mimic basic Unix shell functionalities. It supports file operations, directory manipulation, and essential shell commands like `ls`, `cd`, `cp`, `mv`, `rm`, and more.  

The goal of this project is to enhance understanding of:
- System calls
- File handling
- Process management in C

## Installation

1. Clone the repository:
   ```sh
   git clone https://github.com/nasrlol/unixshell
   cd unixshell
   ```

2. Compile the shell:
   ```sh
   gcc -o custom_shell main.c
   ```

3. Run the shell:
   ```sh
   ./custom_shell
   ```

## Usage

After running the shell (`./custom_shell`), you can execute the following commands:

### Available Commands

| Command                      | Description                                      |
|------------------------------|--------------------------------------------------|
| `ls [directory]`             | List files in a directory                        |
| `mkdir [directory_name]`     | Create a new directory                           |
| `rmdir [directory_name]`     | Recursively delete a directory and its contents |
| `rm [file_name]`             | Delete a file                                   |
| `cp [source] [destination]`  | Copy files from source to destination           |
| `mv [source] [destination]`  | Move files from source to destination           |
| `chown [file]`               | Change file ownership (permissions)             |
| `pwd`                        | Print the current working directory             |
| `cd [directory]`             | Change the working directory                    |
| `clr`                        | Clear the terminal                              |
| `echo [message]`             | Display a message                               |
| `exit`                       | Exit the shell                                  |

## File Structure

```
 unixshell
 ├── main.c          # Main source file for the shell
 ├── shell.h         # Header file (definitions & prototypes)
 ├── commands.c      # Functions handling shell commands
 ├── utils.c         # Utility functions
 ├── README.md       # Documentation
 ├── LICENSE         # License information
```
## License

This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details.
