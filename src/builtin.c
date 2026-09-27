#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "builtin.h"

int is_builtin(const char *cmd) {
    if (cmd == NULL) return 0;
    if (strcmp(cmd, "cd") == 0 || strcmp(cmd, "pwd") == 0 ||
        strcmp(cmd, "echo") == 0 || strcmp(cmd, "exit") == 0) {
        return 1;
    }
    return 0;
}

void execute_builtin(command_t *cmd) {
    if (cmd->argc == 0) return;

    if (strcmp(cmd->argv[0], "cd") == 0) {
        const char *dir;
        if (cmd->argc == 1) {
            dir = getenv("HOME");
        } else if (cmd->argc == 2) {
            dir = cmd->argv[1];
        } else {
            fprintf(stderr, "cd: too many arguments\n");
            return;
        }
        if (dir == NULL || chdir(dir) != 0) {
            perror("cd");
        }
    }
    else if (strcmp(cmd->argv[0], "pwd") == 0) {
        if (cmd->argc > 1) {
            fprintf(stderr, "pwd: too many arguments\n");
            return;
        }
        char buffer[1024];
        if (getcwd(buffer, sizeof(buffer)) == NULL) {
            perror("pwd");
        } else {
            printf("%s\n", buffer);
        }
    }
    else if (strcmp(cmd->argv[0], "echo") == 0) {
        for (int i = 1; i < cmd->argc; i++) {
            printf("%s", cmd->argv[i]);
            if (i < cmd->argc - 1) {
                printf(" ");
            }
        }
        printf("\n");
    }
    else if (strcmp(cmd->argv[0], "exit") == 0) {
        if (cmd->argc > 1) {
            fprintf(stderr, "exit: too many arguments\n");
            return;
        }
        exit(0);
    }
}
