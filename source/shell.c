#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define AUTHOR "ABDELLAH EL MORABIT"

#define MAX_HISTORY 100
#define MAX_COMMAND_LENGTH 255 // not setting a  macro for the argument because they are included in the command
#define MAX_PATH_LENGTH 1024

struct command {
    char *com;
    char *arg;
};

struct exec_command {
    const char *cmd;

    void (*func)(const char *);
};

struct split_arg {
    const char *source;
    const char *destination;
};

// code prototype functions
struct command split_command(char *input);

void exec_command(char *input);

struct exec_command CommandsList[] = {
    {"ls", list},
    {"mkdir", make_dir},
    {"rmdir", remove_dir},
    {"rm", remove_file},
    {"cp", copy_files},
    {"mv", move_files},
    {"chown", change_ownership},
    {"pwd", print_cdirectory},
    {"cd", change_directory},
    {"clr", clear},
    {"mkfile", make_file},
    {"echo", echo},
    {"exit", exit_}
};

// Code functions
struct command split_command(char *input) {
    struct command user_command;

    const char *command = strtok(input, " ");
    const char *argument = strtok(NULL, " ");

    user_command.com = strdup(command);

    if (argument != NULL) {
        user_command.arg = strdup(argument);
    } else {
        user_command.arg = NULL;
    }
    return user_command;
}

struct split_arg split_argument(struct command argument) {
    struct split_arg new_argument = {};

    char *source = strtok(argument.arg, " ");
    char *destination = strtok(NULL, " ");

    if (source != NULL) {
        new_argument.source = source;
    }
    if (destination != NULL) {
        new_argument.destination = destination;
    }
    return new_argument;
}

int main(void) {
    clear("");
    // ReSharper disable once CppDFAEndlessLoop
    while (1) {
        printf("\n$ ");

        char *input = malloc(sizeof(char) * MAX_COMMAND_LENGTH);
        fgets(input, MAX_COMMAND_LENGTH, stdin);
        exec_command(input);
        free(input);
    }
}

void exec_command(char *input) {
    const struct command user_command = split_command(input);

    user_command.com[strcspn(user_command.com, "\n")] = '\0';
    if (user_command.arg != NULL)
        user_command.arg[strcspn(user_command.arg, "\n")] = '\0';

    for (int i = 0; i < sizeof(CommandsList) / sizeof(CommandsList[0]); i++) {
        if (strcmp(user_command.com, CommandsList[i].cmd) == 0) {
            CommandsList[i].func(user_command.arg);
            break;
        }
    }
    free(user_command.com);
    free(user_command.arg);
}

// ls command
void list(const char *path) {
    struct dirent *entry;
    if (path == NULL) {
        path = ".";
    }
    DIR *dP = opendir(path);

    // check if the directory got opened successfully
    if (dP == NULL) {
        perror("failed to open directory");
        return;
    }

    // Print the folder|directory name
    while ((entry = readdir(dP)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    closedir(dP);
}

// extra function to check if a file is a directory or another type of file, so return 0 if directory, else return 1;
int if_directory(const char *path) {
    struct stat info;

    if (stat(path, &info) != 0) {
        perror("failed to get info stat about the given file");
    };

    return S_ISDIR(info.st_mode) ? 0 : 1;
}

void remove_dir(const char *path) {
    DIR *dP = opendir(path);

    if (dP == NULL) {
        perror("failed to open the directory to delete the files recursively");
        return;
    } else
    {
        printf("opened the directory");
    }
    struct dirent *entry;

    while ((entry = readdir(dP)) != NULL) {
        printf("%s\n", entry->d_name);

        char full_path[MAX_PATH_LENGTH];
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        if (if_directory(full_path) == 0) {
            remove_dir(full_path);
        } else if (remove(full_path) != 0) {
            perror("failed to remove the file 003");
        }
    }
    closedir(dP);

    if (rmdir(path) != 0) {
        perror("failed to remove the directory");
    } else {
        printf("successfully removed the directory");
    }
}


void make_dir(const char *path) {
    if (mkdir(path, 0755) != 0)
        perror("failed to make directory");
}

void remove_file(const char *path) {
    if (remove(path) != 0) {
        perror("failed to delete file");
    }
}

void make_file(const char *path) {
    const FILE *new_file = fopen(path, "w");
    if (new_file == NULL) {
        perror("failed to create the new file");
    }
}

void change_ownership(const char *path) {
    if (chmod(path,S_IRWXU) != 0) {
        perror("failed to get ownership of the file");
    } else {
        printf("file permissions updated successfully");
    }
}

void copy_files(const char *arg) {
    // split the argument into the source and destination
    // make a copy of the source file in the destination file
    int c;
    struct split_arg argument = split_argument((struct command){.arg = (char *) arg});

    FILE *pold_file = fopen(argument.source, "r");
    if (pold_file == NULL) {
        perror("cant open the source file for reading (copy failed) ");
        return;
    }

    FILE *pnew_file = fopen(argument.destination, "w");
    if (pnew_file == NULL) {
        perror("cant open the destination file for reading (copy failed)");
        return;
    }

    while ((c = fgetc(pold_file)) != EOF) {
        fputc(c, pnew_file);
    }
    fclose(pnew_file);
    fclose(pold_file);
}

void move_files(const char *arg) {
    struct split_arg argument = split_argument((struct command){.arg = (char *) arg});

    if (remove(argument.source) != 0) {
        perror("failed while trying to move the file");
    }
    // Create a file
    FILE *pFile = fopen(argument.destination, "w");

    // Close the file
    fclose(pFile);
}

void clear(const char *arg) {
    printf("\033[2J");
    printf("\033[3J");
    printf("\033[H");
}

void echo(const char *arg) {
    printf("%s\n", arg);
}

void print_cdirectory(const char *arg) {
    char *current_working_directory = getcwd(NULL, 0);
    printf("%s", current_working_directory);
}

void change_directory(const char *path) {
    if (path == NULL) {
        printf("No path found\n");
    } else if (chdir(path) != 0)
        perror("path not found");
}

void exit_(const char *arg) {
    exit(0);
}
